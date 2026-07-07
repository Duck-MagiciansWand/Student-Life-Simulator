# 🎓 学生生活模拟器 v4.0.1

### *一款基于 C++17 和 OpenGL 的 3D 校园生活模拟游戏*

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-red.svg)](https://www.opengl.org/)
[![GLFW](https://img.shields.io/badge/GLFW-3.3-orange.svg)](https://www.glfw.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-orange.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()
[![Version](https://img.shields.io/badge/Version-4.0.1-brightgreen.svg)]()
[![Status](https://img.shields.io/badge/Status-Stable-success.svg)]()

---

## 📖 项目简介

**Student Life Simulator v4.0.1** 是系列的**首个 3D 版本**。使用 OpenGL + GLFW + GLM 构建，将校园生活模拟带入三维世界。玩家可以在 3D 校园中自由漫游，体验全新的沉浸式视角。

**v4.0.1（"3D 探索"更新）** 在 v4.0 基础上进行了全面优化：修复了鸭子索引硬编码、完善了碰撞检测、改进了 Phong 光照模型、添加了 FPS 显示和奔跑功能。

---

## ✨ v4.0.1 核心亮点

| 新增内容 | 说明 |
| :--- | :--- |
| 🎮 **3D 自由漫游** | WASD 移动，鼠标控制 360° 视角，自由探索校园 |
| 🏛️ **3D 校园建筑** | 宿舍、图书馆、教室、食堂、体育馆 5 栋建筑 |
| 🦆 **3D 动态鸭子** | 5 只鸭子湖中游动，带呼吸动画（独立容器管理） |
| 🌳 **3D 树木** | 24 棵树木点缀校园各处，增强绿化氛围 |
| 💡 **Phong 光照** | 完整的环境光、漫反射、镜面反射光照模型 |
| 🌙 **昼夜循环** | 自动切换白天/夜晚，影响光照和背景色 |
| 🚀 **奔跑功能** | Shift 键 2 倍速度移动 |
| 📊 **FPS 显示** | 窗口标题实时显示帧率 |
| 🧱 **碰撞检测** | 建筑碰撞 + 世界边界限制，防止穿模 |
| 🎯 **跳跃系统** | 空格键跳跃，带物理重力模拟 |

---

## 🎯 核心特色

| 特色 | 说明 |
| :--- | :--- |
| 🏗️ **3D 场景** | 使用 OpenGL 构建的完整 3D 校园世界 |
| 🎮 **自由探索** | 在校园中自由漫游，无固定路线限制 |
| 🦆 **动态生物** | 鸭子会在湖中游动，带呼吸动画 |
| ☀️ **动态光照** | 太阳光方向、强度可调，昼夜自动切换 |
| 💻 **高性能** | 优化的渲染管线，稳定 60 FPS |
| 🔧 **可扩展** | 面向对象设计，易于添加新内容 |
| 🦆 **鸭子收集** | 在 3D 场景中追逐收集鸭子（后续扩展） |
| 🏆 **成就系统** | 3D 版本新增探索相关成就 |

---

## 🏗️ 系统架构

### 类结构设计

```text
┌─────────────────────────────────────────────────────────────────┐
│                       Game3D                                     │
│  - 游戏主循环 / 事件处理 / 渲染 / 物理更新                       │
│  - 场景管理 / 碰撞检测 / 光照控制                                │
└──────────────────────────┬──────────────────────────────────────┘
                           │
           ┌───────────────┼───────────────┐
           ▼               ▼               ▼
   ┌───────────────┐ ┌───────────────┐ ┌───────────────┐
   │    Shader     │ │     Mesh      │ │   GameObject  │
   │  着色器编译   │ │  顶点/索引管理 │ │  游戏对象容器 │
   └───────────────┘ └───────────────┘ └───────────────┘
           │               │               │
           └───────────────┼───────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                       Geometry                                   │
│  - 几何体生成 (Box / Sphere / Cylinder)                         │
└─────────────────────────────────────────────────────────────────┘
```

### 技术栈

| 技术 | 版本 | 用途 |
| :--- | :--- | :--- |
| **C++** | C++17 | 核心开发语言 |
| **OpenGL** | 3.3 | 图形渲染 API |
| **GLFW** | 3.3 | 窗口管理、输入处理 |
| **GLEW** | 2.1 | OpenGL 扩展加载 |
| **GLM** | 0.9.9 | 数学运算、矩阵变换 |

---

## 🦆 3D 场景内容

| 物体 | 数量 | 颜色 | 位置 |
| :--- | :--- | :--- | :--- |
| 地面 | 1 | 绿色 (0.3, 0.6, 0.2) | 中心 |
| 宿舍 | 1 | 棕色 (0.8, 0.5, 0.3) | (-8, 1.5, -8) |
| 图书馆 | 1 | 蓝紫色 (0.5, 0.5, 0.8) | (-8, 1.5, 8) |
| 教室 | 1 | 米色 (0.8, 0.6, 0.4) | (8, 1.5, 8) |
| 食堂 | 1 | 橙色 (0.9, 0.6, 0.3) | (8, 1.5, -8) |
| 体育馆 | 1 | 浅绿 (0.6, 0.8, 0.6) | (0, 2, -12) |
| 树木 | 24 | 绿+棕 | 校园各处 |
| 鸭子 | 5 | 黄色 (1.0, 0.8, 0.2) | 湖中区域 |
| 路灯 | 8 | 灰+金 | 道路两侧 |

---

## 🎮 游戏指南

### 操作说明

| 按键 | 功能 | 引入版本 |
| :--- | :--- | :--- |
| `W A S D` | 移动角色 | v4.0 |
| `鼠标移动` | 控制 360° 视角 | v4.0 |
| `Shift` | 奔跑 (2倍速度) | v4.0.1 |
| `空格` | 跳跃 (带重力) | v4.0.1 |
| `鼠标滚轮` | 缩放视野 (30°-90°) | v4.0 |
| `ESC` | 退出游戏 | v4.0 |

### 视角控制

| 操作 | 效果 |
| :--- | :--- |
| 上下移动鼠标 | 俯仰角 (-89° ~ 89°) |
| 左右移动鼠标 | 偏航角 (无限制) |
| 鼠标滚轮 | 视野缩放 (30° ~ 90°) |

### 物理系统

| 属性 | 值 | 说明 |
| :--- | :--- | :--- |
| 重力 | -0.02 单位/帧 | 下落加速度 |
| 跳跃速度 | 0.25 单位/帧 | 起跳初速度 |
| 行走速度 | 0.3 单位/帧 | 普通移动速度 |
| 奔跑速度 | 0.6 单位/帧 | Shift 加速 (2倍) |
| 最大行走距离 | 22 单位 | 世界边界限制 |

---

## 🚀 快速开始

### 环境配置

#### Windows (MinGW + vcpkg)
```bash
# 1. 安装 vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat

# 2. 安装依赖
vcpkg install glfw3 glew glm

# 3. 设置环境变量
set VCPKG_ROOT=C:\vcpkg
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install g++ make libglfw3-dev libglew-dev libglm-dev
```

#### macOS
```bash
brew install glfw glew glm
```

### 编译与运行

#### Windows (MinGW)
```bash
g++ -o student_simulator_3d main.cpp -std=c++17 \
    -I C:\vcpkg\installed\x64-windows\include \
    -L C:\vcpkg\installed\x64-windows\lib \
    -lglfw3 -lglew32 -lopengl32 -lgdi32

# 运行
student_simulator_3d.exe
```

#### Linux
```bash
g++ -o student_simulator_3d main.cpp -std=c++17 \
    -lglfw -lGLEW -lGL

# 运行
./student_simulator_3d
```

#### macOS
```bash
g++ -o student_simulator_3d main.cpp -std=c++17 \
    -lglfw -lGLEW -framework OpenGL

# 运行
./student_simulator_3d
```

### 使用 CMake（推荐）

创建 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.15)
project(StudentSimulator3D VERSION 4.0.1)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(student_simulator_3d main.cpp)

target_link_libraries(student_simulator_3d
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)
```

编译：
```bash
mkdir build && cd build
cmake ..
cmake --build .
./student_simulator_3d
```

---

## 📁 项目结构

```text
Student-Life-Simulator-3D/
├── main.cpp                 # 游戏主程序 (单文件)
├── CMakeLists.txt           # CMake 构建配置
├── README.md                # 项目说明文档
├── LICENSE                  # Apache-2.0 许可证
├── .gitignore               # Git 忽略文件
├── screenshots/             # (建议) 游戏截图
│   ├── campus_day.png       # 白天校园
│   ├── campus_night.png     # 夜晚校园
│   └── gameplay.png         # 游戏玩法
└── saves/                   # 存档目录
    └── autosave.json
```

---

## 📝 更新日志

### v4.0.1 (2026-07-05) - 3D 改进版
- 🐛 **修复鸭子索引硬编码**：使用专用容器管理鸭子对象
- 🧱 **新增碰撞检测**：建筑碰撞 + 世界边界限制
- 💡 **改进光照系统**：完整 Phong 光照模型 (环境光+漫反射+镜面反射)
- 📊 **新增 FPS 显示**：窗口标题实时显示帧率
- 🚀 **新增奔跑功能**：Shift 键 2 倍速度
- 🔄 **窗口自适应**：正确处理窗口大小变化
- 🎯 **新增跳跃系统**：带物理重力模拟
- 🏆 **新增成就**："3D 探索者"

### v4.0.0 (2026-07-05) - 3D 首发版
- 🎮 **3D 自由漫游**：WASD 移动，鼠标控制视角
- 🏛️ **3D 校园建筑**：5 栋建筑 (宿舍、图书馆、教室、食堂、体育馆)
- 🦆 **3D 动态鸭子**：5 只鸭子游动动画
- 🌳 **3D 树木**：24 棵树木
- 🌙 **昼夜循环**：自动切换白天/夜晚
- 💾 **JSON 存档**：位置、进度保存
- 🎨 **着色器系统**：基础光照 + 雾效

### v3.2.4 (2026-07-04) - 探索与边界更新
- 🏫 9 大场景系统
- 🚪 校门外出机制
- 📍 场景切换菜单
- 🏆 新增成就："探险家"、"越狱者"

### v3.2.3 (2026-07-04) - 云朵与星空更新
- ☁️ 云朵系统 + CaelLab 联动
- 🌙 昼夜循环
- 🏆 新增成就："追云者"、"虚舟访客"

### v3.2.2 (2026-07-04) - 鸭子纪元更新
- 🦆 16 种鸭子 + 繁殖系统
- 🚽 传送门彩蛋
- 🎂 周年庆场景
- 🏆 新增成就："繁殖大师"、"传送门旅人"、"周年庆嘉宾"、"鸭王之王"

### v3.2.1 (2026-07-03) - 鸭子与彩蛋更新
- 🦆 校园湖与 10 只鸭子收集
- 🦆 NPC "鸭哥"
- 📖 《鸭科动物大全》
- 🏆 新增成就："初级鸭迷"、"中级鸭迷"、"高级鸭迷"

### v3.2.0 (2026-07-03) - 恋爱与探索更新
- 💕 恋爱系统
- 🔍 探索系统
- 🎭 情感状态
- 📖 故事分支
- 🏆 新增成就："初恋"、"心碎"、"探索者"、"探索大师"、"浪漫主义者"

### v3.1.0 (2026-06-28) - 故事与成就更新
- 📖 故事主线 (5 个核心剧情事件)
- 🏆 成就系统 (10+ 个成就)
- 💬 增强 NPC 交互
- 📊 数据统计
- 🏆 新增成就："初入学术"、"学霸之路"、"优秀毕业生"、"社交达人"、"小富翁"、"事件收藏家"、"提前毕业"、"完美成绩"

### v3.0.0 (2026-06-27) - 架构重构
- 🎲 随机事件系统 (10+ 种事件)
- 💬 NPC 好感度系统 (3 个 NPC)
- 💾 JSON 格式存档
- 🏛️ 多态建筑交互
- 🔧 代码架构重构

### v2.0.0 (2026-06-26) - 图形化版本
- ✅ SFML 2D 图形界面
- ✅ 可视化校园地图
- ✅ 5 个可交互建筑

### v1.0.0 (2026-06-26) - 初始版本
- ✅ 控制台版本
- ✅ 课程/考试系统
- ✅ 毕业条件

---

## 🤝 如何贡献

1. **Fork** 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 **Pull Request**

### 代码风格
- **类名**：驼峰命名法，如 `Game3D`
- **函数名**：驼峰命名法，如 `handleInput`
- **变量名**：下划线命名法，如 `player_pos`
- **常量**：全大写，如 `MAX_PLAYERS`
- **注释**：为复杂逻辑添加清晰注释

---

## 📄 许可证

本项目采用 **Apache License 2.0**。

```text
Copyright © 2026 Magician's Wand (Duck, MLC, Z)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

---

## 👨‍💻 项目团队

| 角色 | 成员 |
| :--- | :--- |
| **开发人员** | Duck, MLC, Z |
| **产品策划** | Duck |
| **项目组织** | [MagiciansWand](https://magicianswand.cn) |
| **支持机构** | [虚舟实验室 (CaelLab)](https://www.caellab.com/) |

---

## 🙏 致谢

- [**GLFW**](https://www.glfw.org/) - 窗口管理库
- [**GLEW**](https://glew.sourceforge.net/) - OpenGL 扩展库
- [**GLM**](https://glm.g-truc.net/) - 数学库
- [**虚舟实验室 (CaelLab)**](https://www.caellab.com/) - 合作伙伴与联动支持
- 所有贡献者和玩家

---

## ⭐ Star History

如果这个项目对你有帮助，请点亮右上角的 Star ⭐！

[![Star History Chart](https://api.star-history.com/svg?repos=Duck-MagiciansWand/Student-Life-Simulator&type=Date)](https://star-history.com/#Duck-MagiciansWand/Student-Life-Simulator&Date)

---

**祝你大学生活愉快！🏛️🎉**

****注：3D建模正在制作中请稍等****
---
> **📬 加入我们：MagiciansWand@126.com**  
> *发送邮件标题【贡献】或【合作】，你会收到：*  
> *- PDA语言入门指南*  
> *- 团队协作规范*  
> *- 下一个版本的开发计划*
> **QQ：2603419175**
> **微信：mlc201513**

Copyright © 2026 Magician's Wand

