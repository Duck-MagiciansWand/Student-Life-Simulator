// ============================================================
// Student Life Simulator v4.0.1 - 3D 版本 (改进版)
// 使用 OpenGL + GLFW + GLM
// 改进内容：
//   ✅ 修复鸭子索引硬编码
//   ✅ 完善碰撞检测
//   ✅ 改进光照系统 (Phong模型)
//   ✅ FPS 显示
//   ✅ 奔跑功能 (Shift)
//   ✅ 窗口自适应
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <ctime>
#include <map>
#include <memory>
#include <algorithm>

// OpenGL 头文件
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ============================================================
// 1. 着色器类
// ============================================================
class Shader {
private:
    unsigned int programID;
    
    std::string loadShaderSource(const std::string& filepath) {
        if (filepath == "vertex.glsl") {
            return R"(
                #version 330 core
                layout(location = 0) in vec3 aPos;
                layout(location = 1) in vec3 aColor;
                layout(location = 2) in vec2 aTexCoord;
                layout(location = 3) in vec3 aNormal;
                
                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;
                
                out vec3 color;
                out vec2 texCoord;
                out vec3 Normal;
                out vec3 FragPos;
                
                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                    FragPos = vec3(model * vec4(aPos, 1.0));
                    Normal = mat3(transpose(inverse(model))) * aNormal;
                    color = aColor;
                    texCoord = aTexCoord;
                }
            )";
        } else if (filepath == "fragment.glsl") {
            return R"(
                #version 330 core
                in vec3 color;
                in vec2 texCoord;
                in vec3 Normal;
                in vec3 FragPos;
                
                uniform vec3 lightDir;
                uniform vec3 viewPos;
                uniform float time;
                uniform bool isNight;
                
                out vec4 FragColor;
                
                void main() {
                    // 环境光
                    float ambient = isNight ? 0.15 : 0.3;
                    
                    // 漫反射
                    vec3 norm = normalize(Normal);
                    vec3 lightDirNorm = normalize(lightDir);
                    float diff = max(dot(norm, lightDirNorm), 0.0);
                    float diffuse = 0.7 * diff;
                    
                    // 镜面反射
                    float specular = 0.0;
                    vec3 viewDir = normalize(viewPos - FragPos);
                    vec3 reflectDir = reflect(-lightDirNorm, norm);
                    specular = pow(max(dot(viewDir, reflectDir), 0.0), 32) * 0.3;
                    
                    vec3 finalColor = color * (ambient + diffuse + specular);
                    FragColor = vec4(finalColor, 1.0);
                }
            )";
        }
        return "";
    }
    
    unsigned int compileShader(unsigned int type, const std::string& source) {
        unsigned int shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "❌ 着色器编译失败: " << infoLog << std::endl;
        }
        return shader;
    }
    
public:
    Shader() : programID(0) {}
    
    void load(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexSource = loadShaderSource(vertexPath);
        std::string fragmentSource = loadShaderSource(fragmentPath);
        
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);
        
        int success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "❌ 着色器链接失败: " << infoLog << std::endl;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    
    void use() { glUseProgram(programID); }
    unsigned int getID() const { return programID; }
    
    void setMat4(const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    
    void setVec3(const std::string& name, const glm::vec3& vec) {
        glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(vec));
    }
    
    void setFloat(const std::string& name, float val) {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), val);
    }
    
    void setBool(const std::string& name, bool val) {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), val ? 1 : 0);
    }
};

// ============================================================
// 2. 顶点结构
// ============================================================
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

// ============================================================
// 3. 网格类
// ============================================================
class Mesh {
private:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
public:
    Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds)
        : vertices(verts), indices(inds) {
        setupMesh();
    }
    
    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        // 位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        // 颜色
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        
        // UV
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);
        
        // 法线
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);
        
        glBindVertexArray(0);
    }
    
    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

// ============================================================
// 4. 几何体生成器
// ============================================================
class Geometry {
public:
    static Mesh* createBox(float width, float height, float depth, glm::vec3 color) {
        float hw = width / 2, hh = height / 2, hd = depth / 2;
        
        // 法线
        glm::vec3 nFront = glm::vec3(0, 0, 1);
        glm::vec3 nBack = glm::vec3(0, 0, -1);
        glm::vec3 nLeft = glm::vec3(-1, 0, 0);
        glm::vec3 nRight = glm::vec3(1, 0, 0);
        glm::vec3 nTop = glm::vec3(0, 1, 0);
        glm::vec3 nBottom = glm::vec3(0, -1, 0);
        
        std::vector<Vertex> verts = {
            // 前面
            {{-hw, -hh,  hd}, color, {0,0}, nFront}, {{ hw, -hh,  hd}, color, {1,0}, nFront},
            {{ hw,  hh,  hd}, color, {1,1}, nFront}, {{-hw,  hh,  hd}, color, {0,1}, nFront},
            // 后面
            {{ hw, -hh, -hd}, color, {0,0}, nBack}, {{-hw, -hh, -hd}, color, {1,0}, nBack},
            {{-hw,  hh, -hd}, color, {1,1}, nBack}, {{ hw,  hh, -hd}, color, {0,1}, nBack},
            // 左面
            {{-hw, -hh, -hd}, color, {0,0}, nLeft}, {{-hw, -hh,  hd}, color, {1,0}, nLeft},
            {{-hw,  hh,  hd}, color, {1,1}, nLeft}, {{-hw,  hh, -hd}, color, {0,1}, nLeft},
            // 右面
            {{ hw, -hh,  hd}, color, {0,0}, nRight}, {{ hw, -hh, -hd}, color, {1,0}, nRight},
            {{ hw,  hh, -hd}, color, {1,1}, nRight}, {{ hw,  hh,  hd}, color, {0,1}, nRight},
            // 顶面
            {{-hw,  hh, -hd}, color, {0,0}, nTop}, {{ hw,  hh, -hd}, color, {1,0}, nTop},
            {{ hw,  hh,  hd}, color, {1,1}, nTop}, {{-hw,  hh,  hd}, color, {0,1}, nTop},
            // 底面
            {{-hw, -hh,  hd}, color, {0,0}, nBottom}, {{ hw, -hh,  hd}, color, {1,0}, nBottom},
            {{ hw, -hh, -hd}, color, {1,1}, nBottom}, {{-hw, -hh, -hd}, color, {0,1}, nBottom}
        };
        
        std::vector<unsigned int> indices = {
            0,1,2, 0,2,3,    4,5,6, 4,6,7,
            8,9,10, 8,10,11, 12,13,14, 12,14,15,
            16,17,18, 16,18,19, 20,21,22, 20,22,23
        };
        
        return new Mesh(verts, indices);
    }
    
    static Mesh* createSphere(float radius, int segments, glm::vec3 color) {
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        
        for (int i = 0; i <= segments; i++) {
            float theta = i * M_PI / segments;
            for (int j = 0; j <= segments; j++) {
                float phi = j * 2 * M_PI / segments;
                float x = radius * sin(theta) * cos(phi);
                float y = radius * cos(theta);
                float z = radius * sin(theta) * sin(phi);
                glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
                verts.push_back({{x, y, z}, color, {0,0}, normal});
            }
        }
        
        for (int i = 0; i < segments; i++) {
            for (int j = 0; j < segments; j++) {
                int a = i * (segments + 1) + j;
                int b = a + segments + 1;
                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(a + 1);
                indices.push_back(a + 1);
                indices.push_back(b);
                indices.push_back(b + 1);
            }
        }
        
        return new Mesh(verts, indices);
    }
    
    static Mesh* createCylinder(float radius, float height, int segments, glm::vec3 color) {
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        
        float hh = height / 2;
        
        for (int i = 0; i <= segments; i++) {
            float angle = i * 2 * M_PI / segments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));
            verts.push_back({{x, -hh, z}, color, {0,0}, normal});
            verts.push_back({{x,  hh, z}, color, {0,0}, normal});
        }
        
        for (int i = 0; i < segments; i++) {
            int a = i * 2;
            int b = a + 2;
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(a + 1);
            indices.push_back(a + 1);
            indices.push_back(b);
            indices.push_back(b + 1);
        }
        
        return new Mesh(verts, indices);
    }
};

// ============================================================
// 5. 游戏对象类
// ============================================================
class GameObject {
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Mesh* mesh;
    glm::vec3 color;
    bool active;
    
public:
    GameObject() : position(0,0,0), rotation(0,0,0), scale(1,1,1), 
                   mesh(nullptr), color(1,1,1), active(true) {}
    
    GameObject(Mesh* m, glm::vec3 pos, glm::vec3 col) 
        : position(pos), rotation(0,0,0), scale(1,1,1), mesh(m), color(col), active(true) {}
    
    void setPosition(const glm::vec3& p) { position = p; }
    void setRotation(const glm::vec3& r) { rotation = r; }
    void setScale(const glm::vec3& s) { scale = s; }
    void setActive(bool a) { active = a; }
    
    glm::vec3 getPosition() const { return position; }
    Mesh* getMesh() const { return mesh; }
    bool isActive() const { return active; }
    
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, glm::vec3(1,0,0));
        model = glm::rotate(model, rotation.y, glm::vec3(0,1,0));
        model = glm::rotate(model, rotation.z, glm::vec3(0,0,1));
        model = glm::scale(model, scale);
        return model;
    }
    
    void draw(Shader& shader) {
        if (!mesh || !active) return;
        shader.setMat4("model", getModelMatrix());
        mesh->draw();
    }
};

// ============================================================
// 6. 3D 游戏主类
// ============================================================
class Game3D {
private:
    GLFWwindow* window;
    int width = 1024, height = 768;
    
    Shader shader;
    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<GameObject*> ducks;      // 鸭子引用 (v4.0.1 修复)
    std::vector<GameObject*> buildings;  // 建筑引用 (碰撞检测)
    
    // 摄像机
    glm::vec3 cameraPos = glm::vec3(20, 15, 25);
    glm::vec3 cameraFront = glm::vec3(-0.5, -0.3, -0.7);
    glm::vec3 cameraUp = glm::vec3(0, 1, 0);
    float yaw = -135.0f;
    float pitch = -20.0f;
    float fov = 60.0f;
    
    // 玩家
    glm::vec3 playerPos = glm::vec3(0, 0.5, 0);
    float playerSpeed = 0.3f;
    float gravity = -0.02f;
    float velocityY = 0;
    bool isGrounded = true;
    
    // 鼠标
    double lastX = 0, lastY = 0;
    bool firstMouse = true;
    
    // 时间
    float time = 0;
    bool isNight = false;
    
    // FPS
    double lastFPSTime = 0;
    int frameCount = 0;
    int currentFPS = 60;
    
    // 随机
    std::mt19937 rng;
    
    // 碰撞检测
    float worldBoundary = 22.0f;
    
public:
    Game3D() : rng(std::time(nullptr)) {
        initGLFW();
        initGLEW();
        initShaders();
        buildWorld();
        setupCallbacks();
        std::cout << "✅ 游戏初始化完成\n";
    }
    
    ~Game3D() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    void initGLFW() {
        if (!glfwInit()) {
            std::cerr << "❌ GLFW 初始化失败\n";
            exit(1);
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        window = glfwCreateWindow(width, height, "🎓 学生生活模拟器 v4.0.1 - 3D", nullptr, nullptr);
        if (!window) {
            std::cerr << "❌ 窗口创建失败\n";
            glfwTerminate();
            exit(1);
        }
        
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetScrollCallback(window, scrollCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetWindowUserPointer(window, this);
    }
    
    void initGLEW() {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "❌ GLEW 初始化失败\n";
            exit(1);
        }
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.2f, 0.6f, 0.8f, 1.0f);
    }
    
    void initShaders() {
        shader.load("vertex.glsl", "fragment.glsl");
    }
    
    void buildWorld() {
        // ---- 地面 ----
        auto* groundMesh = Geometry::createBox(50, 0.5, 50, glm::vec3(0.3, 0.6, 0.2));
        meshes.push_back(std::unique_ptr<Mesh>(groundMesh));
        auto* ground = new GameObject(groundMesh, glm::vec3(0, -0.25, 0), glm::vec3(0.3, 0.6, 0.2));
        objects.push_back(std::unique_ptr<GameObject>(ground));
        
        // ---- 建筑 ----
        auto* buildingMesh = Geometry::createBox(4, 3, 3, glm::vec3(0.8, 0.7, 0.6));
        meshes.push_back(std::unique_ptr<Mesh>(buildingMesh));
        
        // 建筑数据: 位置, 缩放, 颜色, 名称
        struct BuildingData {
            glm::vec3 pos;
            glm::vec3 scale;
            glm::vec3 color;
            std::string name;
        };
        
        std::vector<BuildingData> buildingData = {
            {{-8, 1.5, -8}, {1.2, 1.0, 1.0}, {0.8, 0.5, 0.3}, "宿舍"},
            {{-8, 1.5, 8},  {1.5, 1.2, 1.0}, {0.5, 0.5, 0.8}, "图书馆"},
            {{8, 1.5, 8},   {1.3, 1.0, 1.3}, {0.8, 0.6, 0.4}, "教室"},
            {{8, 1.5, -8},  {1.4, 0.8, 1.2}, {0.9, 0.6, 0.3}, "食堂"}
        };
        
        for (const auto& data : buildingData) {
            auto* building = new GameObject(buildingMesh, data.pos, data.color);
            building->setScale(data.scale);
            objects.push_back(std::unique_ptr<GameObject>(building));
            buildings.push_back(building);
        }
        
        // ---- 体育馆 (大建筑) ----
        auto* gymMesh = Geometry::createBox(6, 4, 4, glm::vec3(0.6, 0.8, 0.6));
        meshes.push_back(std::unique_ptr<Mesh>(gymMesh));
        auto* gym = new GameObject(gymMesh, glm::vec3(0, 2, -12), glm::vec3(0.6, 0.8, 0.6));
        objects.push_back(std::unique_ptr<GameObject>(gym));
        buildings.push_back(gym);
        
        // ---- 树木 ----
        auto* treeTrunk = Geometry::createCylinder(0.3, 2, 8, glm::vec3(0.5, 0.3, 0.1));
        meshes.push_back(std::unique_ptr<Mesh>(treeTrunk));
        auto* treeTop = Geometry::createSphere(1.5, 8, glm::vec3(0.1, 0.6, 0.1));
        meshes.push_back(std::unique_ptr<Mesh>(treeTop));
        
        std::vector<glm::vec3> treePositions = {
            {-4, 0, -5}, {-4, 0, 5}, {4, 0, -5}, {4, 0, 5},
            {-10, 0, 0}, {10, 0, 0}, {0, 0, -15}, {0, 0, 15},
            {-14, 0, -4}, {-14, 0, 4}, {14, 0, -4}, {14, 0, 4}
        };
        
        for (const auto& pos : treePositions) {
            auto* trunk = new GameObject(treeTrunk, pos + glm::vec3(0, 1, 0), glm::vec3(0.5, 0.3, 0.1));
            objects.push_back(std::unique_ptr<GameObject>(trunk));
            auto* top = new GameObject(treeTop, pos + glm::vec3(0, 2.8, 0), glm::vec3(0.1, 0.6, 0.1));
            objects.push_back(std::unique_ptr<GameObject>(top));
        }
        
        // ---- 鸭子 (v4.0.1: 使用专用容器) ----
        auto* duckMesh = Geometry::createSphere(0.5, 8, glm::vec3(1.0, 0.8, 0.2));
        meshes.push_back(std::unique_ptr<Mesh>(duckMesh));
        
        std::vector<glm::vec3> duckPositions = {
            {-3, 0.5, -3}, {-1.5, 0.5, -4}, {0, 0.5, -2}, {1.5, 0.5, -4}, {3, 0.5, -3}
        };
        
        for (const auto& pos : duckPositions) {
            auto* duck = new GameObject(duckMesh, pos, glm::vec3(1.0, 0.8, 0.2));
            objects.push_back(std::unique_ptr<GameObject>(duck));
            ducks.push_back(duck);  // 存储引用
        }
        
        // ---- 路灯 ----
        auto* poleMesh = Geometry::createCylinder(0.1, 2, 8, glm::vec3(0.3, 0.3, 0.3));
        meshes.push_back(std::unique_ptr<Mesh>(poleMesh));
        auto* lightMesh = Geometry::createSphere(0.3, 8, glm::vec3(1.0, 1.0, 0.8));
        meshes.push_back(std::unique_ptr<Mesh>(lightMesh));
        
        std::vector<glm::vec3> lightPositions = {
            {-6, 0, -6}, {-6, 0, 6}, {6, 0, -6}, {6, 0, 6}
        };
        for (const auto& pos : lightPositions) {
            auto* pole = new GameObject(poleMesh, pos + glm::vec3(0, 1, 0), glm::vec3(0.3, 0.3, 0.3));
            objects.push_back(std::unique_ptr<GameObject>(pole));
            auto* light = new GameObject(lightMesh, pos + glm::vec3(0, 2.3, 0), glm::vec3(1.0, 1.0, 0.8));
            objects.push_back(std::unique_ptr<GameObject>(light));
        }
        
        std::cout << "🌍 世界构建完成: " << objects.size() << " 个物体\n";
        std::cout << "  🦆 " << ducks.size() << " 只鸭子\n";
        std::cout << "  🏛️ " << buildings.size() << " 栋建筑\n";
    }
    
    void setupCallbacks() {
        glfwSetKeyCallback(window, keyCallback);
    }
    
    // ----- 静态回调函数 -----
    static void framebufferSizeCallback(GLFWwindow* window, int w, int h) {
        glViewport(0, 0, w, h);
        auto* game = static_cast<Game3D*>(glfwGetWindowUserPointer(window));
        if (game) {
            game->width = w;
            game->height = h;
        }
    }
    
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* game = static_cast<Game3D*>(glfwGetWindowUserPointer(window));
        if (!game) return;
        
        if (game->firstMouse) {
            game->lastX = xpos;
            game->lastY = ypos;
            game->firstMouse = false;
        }
        
        float xOffset = xpos - game->lastX;
        float yOffset = game->lastY - ypos;
        game->lastX = xpos;
        game->lastY = ypos;
        
        float sensitivity = 0.1f;
        game->yaw += xOffset * sensitivity;
        game->pitch += yOffset * sensitivity;
        game->pitch = std::clamp(game->pitch, -89.0f, 89.0f);
        
        glm::vec3 front;
        front.x = cos(glm::radians(game->yaw)) * cos(glm::radians(game->pitch));
        front.y = sin(glm::radians(game->pitch));
        front.z = sin(glm::radians(game->yaw)) * cos(glm::radians(game->pitch));
        game->cameraFront = glm::normalize(front);
    }
    
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* game = static_cast<Game3D*>(glfwGetWindowUserPointer(window));
        if (!game) return;
        game->fov -= yoffset * 2;
        game->fov = std::clamp(game->fov, 30.0f, 90.0f);
    }
    
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* game = static_cast<Game3D*>(glfwGetWindowUserPointer(window));
        if (!game) return;
        
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
    
    // ----- 碰撞检测 (v4.0.1 新增) -----
    bool checkCollision(const glm::vec3& pos) {
        // 世界边界
        if (std::abs(pos.x) > worldBoundary || std::abs(pos.z) > worldBoundary) {
            return true;
        }
        
        // 建筑碰撞
        float playerRadius = 0.8f;
        for (auto* building : buildings) {
            glm::vec3 bPos = building->getPosition();
            float dx = pos.x - bPos.x;
            float dz = pos.z - bPos.z;
            float dist = sqrt(dx*dx + dz*dz);
            
            // 粗略的圆形碰撞
            float buildingRadius = 2.5f;
            if (dist < buildingRadius + playerRadius) {
                return true;
            }
        }
        
        return false;
    }
    
    // ----- 输入处理 (v4.0.1 增强) -----
    void handleInput() {
        float speed = playerSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            speed *= 2.0f;  // 奔跑
        }
        
        glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
        glm::vec3 forward = glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z));
        
        glm::vec3 moveDelta(0);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDelta += forward;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDelta -= forward;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDelta -= right;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDelta += right;
        
        if (glm::length(moveDelta) > 0) {
            moveDelta = glm::normalize(moveDelta) * speed;
            glm::vec3 newPos = playerPos + moveDelta;
            
            // 分别检测X和Z轴，实现沿墙滑动
            glm::vec3 testPos = playerPos;
            testPos.x = newPos.x;
            if (!checkCollision(testPos)) {
                playerPos.x = newPos.x;
            }
            testPos.z = newPos.z;
            if (!checkCollision(testPos)) {
                playerPos.z = newPos.z;
            }
        }
        
        // 跳跃
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
            velocityY = 0.25f;
            isGrounded = false;
        }
        
        // 重力
        velocityY += gravity;
        playerPos.y += velocityY;
        if (playerPos.y <= 0.5f) {
            playerPos.y = 0.5f;
            velocityY = 0;
            isGrounded = true;
        }
        
        // 摄像机跟随
        cameraPos = playerPos + glm::vec3(15, 10, 15);
    }
    
    // ----- 更新 (v4.0.1 改进) -----
    void update() {
        float delta = 0.016f;  // 约60FPS
        time += delta;
        isNight = (int(time / 30) % 2 == 1);
        
        // ---- 更新鸭子 (使用专用容器) ----
        for (int i = 0; i < (int)ducks.size(); i++) {
            float offset = i * 0.5f;
            float x = -3 + i * 1.5f + sin(time * 0.5f + offset) * 0.5f;
            float z = -3 + i * 0.5f + cos(time * 0.5f + offset * 0.7f) * 0.5f;
            float y = 0.5f + sin(time * 2 + i) * 0.1f;
            ducks[i]->setPosition(glm::vec3(x, y, z));
        }
        
        // ---- 更新FPS ----
        frameCount++;
        double currentTime = glfwGetTime();
        if (currentTime - lastFPSTime >= 1.0) {
            currentFPS = frameCount;
            frameCount = 0;
            lastFPSTime = currentTime;
            
            std::string title = "🎓 学生生活模拟器 v4.0.1 - FPS: " + 
                                std::to_string(currentFPS) + 
                                (isNight ? " 🌙 夜晚" : " ☀️ 白天");
            glfwSetWindowTitle(window, title.c_str());
        }
        
        // ---- 昼夜切换 ----
        if (isNight) {
            glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
        } else {
            glClearColor(0.2f, 0.6f, 0.8f, 1.0f);
        }
    }
    
    // ----- 渲染 -----
    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        
        // 投影矩阵
        float aspect = (float)width / height;
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        
        // 视图矩阵
        glm::mat4 view = glm::lookAt(cameraPos, playerPos, cameraUp);
        shader.setMat4("view", view);
        
        // 光照
        glm::vec3 lightDir = isNight ? glm::vec3(-0.2f, 0.1f, -0.3f) : glm::vec3(0.5f, 1.0f, 0.3f);
        shader.setVec3("lightDir", lightDir);
        shader.setVec3("viewPos", cameraPos);
        shader.setFloat("time", time);
        shader.setBool("isNight", isNight);
        
        // 绘制所有物体
        for (auto& obj : objects) {
            obj->draw(shader);
        }
        
        glfwSwapBuffers(window);
    }
    
    // ----- 主循环 -----
    void run() {
        std::cout << "🎮 游戏运行中...\n";
        std::cout << "   WASD 移动 | 鼠标视角 | Shift 奔跑\n";
        std::cout << "   空格 跳跃 | ESC 退出\n\n";
        
        while (!glfwWindowShouldClose(window)) {
            handleInput();
            update();
            render();
            glfwPollEvents();
        }
    }
};

// ============================================================
// 7. main
// ============================================================
int main() {
    std::cout << "╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║      🎓 学生生活模拟器 v4.0.1 - 3D 版本             ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════╣\n";
    std::cout << "║  ✨ 改进内容:                                        ║\n";
    std::cout << "║    ✅ 修复鸭子索引硬编码                             ║\n";
    std::cout << "║    ✅ 完善碰撞检测                                   ║\n";
    std::cout << "║    ✅ 改进光照系统 (Phong模型)                       ║\n";
    std::cout << "║    ✅ FPS 显示                                       ║\n";
    std::cout << "║    ✅ 奔跑功能 (Shift)                               ║\n";
    std::cout << "║    ✅ 窗口自适应                                     ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";
    
    try {
        Game3D game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "❌ 程序异常: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "\n游戏已结束！\n";
    return 0;
}
