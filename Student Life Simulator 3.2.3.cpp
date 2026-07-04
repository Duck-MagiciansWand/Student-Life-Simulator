#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <map>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <cstdlib>  // 用于 system() 打开网页

using json = nlohmann::json;
namespace fs = std::filesystem;

// ============================================================
// 0. 鸭子系统 (v3.2.2 完整保留)
// ============================================================

enum class DuckRarity {
    Common, Uncommon, Epic, Legendary
};

std::string getRarityName(DuckRarity r) {
    switch(r) {
        case DuckRarity::Common: return "普通";
        case DuckRarity::Uncommon: return "稀有";
        case DuckRarity::Epic: return "史诗";
        case DuckRarity::Legendary: return "传说";
        default: return "未知";
    }
}

struct Duck {
    std::string id;
    std::string name;
    std::string emoji;
    DuckRarity rarity;
    std::string location;
    bool collected = false;
    bool isBreedable = true;
    std::vector<std::string> breedsWith;
    int generation = 0;
    std::string parent1 = "";
    std::string parent2 = "";
};

class DuckCollection {
private:
    std::vector<Duck> ducks;
    std::vector<std::string> collectedIds;
    std::map<std::string, int> breedCount;
    std::mt19937 rng;
    
public:
    DuckCollection() {
        rng.seed(std::time(nullptr));
        initDucks();
    }
    
    void initDucks() {
        // 普通鸭子 (6只)
        ducks.push_back({"duck_001", "小黄", "🦆", DuckRarity::Common, "湖中心", false, true, {"duck_002", "duck_003", "duck_004"}});
        ducks.push_back({"duck_002", "大花", "🦆", DuckRarity::Common, "湖西岸", false, true, {"duck_001", "duck_003", "duck_005"}});
        ducks.push_back({"duck_003", "斑斑", "🦆", DuckRarity::Common, "湖东岸", false, true, {"duck_001", "duck_002", "duck_006"}});
        ducks.push_back({"duck_004", "雪球", "🦆", DuckRarity::Common, "图书馆旁", false, true, {"duck_001", "duck_005", "duck_006"}});
        ducks.push_back({"duck_005", "黑豆", "🦆", DuckRarity::Common, "食堂后面", false, true, {"duck_002", "duck_004", "duck_006"}});
        ducks.push_back({"duck_006", "灰灰", "🦆", DuckRarity::Common, "宿舍楼下", false, true, {"duck_003", "duck_004", "duck_005"}});
        // 稀有鸭子 (4只)
        ducks.push_back({"duck_007", "金毛", "🦆", DuckRarity::Uncommon, "教学楼前", false, true, {"duck_008", "duck_009"}});
        ducks.push_back({"duck_008", "白鹭", "🦆", DuckRarity::Uncommon, "湖边草丛", false, true, {"duck_007", "duck_010"}});
        ducks.push_back({"duck_009", "小橘", "🦆", DuckRarity::Uncommon, "花园角落", false, true, {"duck_007", "duck_010"}});
        ducks.push_back({"duck_010", "鸭哥", "🦆", DuckRarity::Uncommon, "隐藏", false, true, {"duck_007", "duck_008", "duck_009"}});
        // 史诗鸭子 (3只)
        ducks.push_back({"duck_011", "星河鸭", "🌌", DuckRarity::Epic, "繁殖获得", false, true, {"duck_012", "duck_013"}});
        ducks.push_back({"duck_012", "极光鸭", "🌈", DuckRarity::Epic, "繁殖获得", false, true, {"duck_011", "duck_013"}});
        ducks.push_back({"duck_013", "机械鸭", "🤖", DuckRarity::Epic, "繁殖获得", false, true, {"duck_011", "duck_012"}});
        // 传说鸭子 (3只)
        ducks.push_back({"duck_014", "彩虹鸭", "🌈", DuckRarity::Legendary, "繁殖获得", false, false, {}});
        ducks.push_back({"duck_015", "幽灵鸭", "👻", DuckRarity::Legendary, "繁殖获得", false, false, {}});
        ducks.push_back({"duck_016", "凤凰鸭", "🔥", DuckRarity::Legendary, "繁殖获得", false, false, {}});
    }
    
    Duck* getDuck(const std::string& id) {
        for (auto& d : ducks) if (d.id == id) return &d;
        return nullptr;
    }
    
    bool collect(const std::string& id) {
        auto* d = getDuck(id);
        if (!d || d->collected) return false;
        d->collected = true;
        collectedIds.push_back(id);
        return true;
    }
    
    bool canBreed(const std::string& id1, const std::string& id2) {
        auto* d1 = getDuck(id1);
        auto* d2 = getDuck(id2);
        if (!d1 || !d2 || !d1->collected || !d2->collected) return false;
        if (!d1->isBreedable || !d2->isBreedable || id1 == id2) return false;
        if (d1->rarity == DuckRarity::Legendary || d2->rarity == DuckRarity::Legendary) return false;
        for (const auto& id : d1->breedsWith) if (id == id2) return true;
        return false;
    }
    
    std::string breed(const std::string& id1, const std::string& id2) {
        if (!canBreed(id1, id2)) return "";
        auto* d1 = getDuck(id1);
        auto* d2 = getDuck(id2);
        
        DuckRarity targetRarity = DuckRarity::Common;
        std::uniform_int_distribution<int> dist(0, 100);
        int roll = dist(rng);
        
        if (d1->rarity == DuckRarity::Epic && d2->rarity == DuckRarity::Epic) {
            targetRarity = (roll < 5) ? DuckRarity::Legendary : DuckRarity::Epic;
        } else if (d1->rarity == DuckRarity::Epic || d2->rarity == DuckRarity::Epic) {
            targetRarity = (roll < 30) ? DuckRarity::Epic : DuckRarity::Uncommon;
        } else if (d1->rarity == DuckRarity::Uncommon && d2->rarity == DuckRarity::Uncommon) {
            targetRarity = (roll < 10) ? DuckRarity::Epic : DuckRarity::Uncommon;
        } else {
            targetRarity = (roll < 20) ? DuckRarity::Uncommon : DuckRarity::Common;
        }
        
        for (auto& d : ducks) {
            if (!d.collected && d.rarity == targetRarity && d.location == "繁殖获得") {
                d.collected = true;
                d.generation = std::max(d1->generation, d2->generation) + 1;
                d.parent1 = id1;
                d.parent2 = id2;
                collectedIds.push_back(d.id);
                breedCount[d.id]++;
                return d.id;
            }
        }
        return "";
    }
    
    std::vector<Duck> getCollected() const {
        std::vector<Duck> result;
        for (const auto& d : ducks) if (d.collected) result.push_back(d);
        return result;
    }
    int getCollectedCount() const { return collectedIds.size(); }
    int getTotalCount() const { return ducks.size(); }
    
    json toJson() const {
        json j;
        for (const auto& d : ducks) {
            j[d.id] = {{"collected", d.collected}, {"generation", d.generation}, 
                       {"parent1", d.parent1}, {"parent2", d.parent2}};
        }
        j["breed_count"] = breedCount;
        return j;
    }
    void fromJson(const json& j) {
        for (auto& d : ducks) {
            if (j.contains(d.id)) {
                d.collected = j[d.id]["collected"].get<bool>();
                d.generation = j[d.id].value("generation", 0);
                d.parent1 = j[d.id].value("parent1", "");
                d.parent2 = j[d.id].value("parent2", "");
                if (d.collected) collectedIds.push_back(d.id);
            }
        }
        if (j.contains("breed_count")) {
            breedCount = j["breed_count"].get<std::map<std::string, int>>();
        }
    }
};

// ============================================================
// 1. 云朵系统 (v3.2.3 新增)
// ============================================================

class CloudSystem {
private:
    bool specialCloudVisible = false;
    bool cloudTriggered = false;
    float cloudX = 0;
    float cloudY = 0;
    std::mt19937 rng;
    float timer = 0;
    bool isNight = false;
    
public:
    CloudSystem() {
        rng.seed(std::time(nullptr));
        cloudX = 100 + (rng() % 800);
        cloudY = 50 + (rng() % 150);
    }
    
    void update(float deltaTime, bool sleeping) {
        timer += deltaTime;
        
        // 白天/夜晚切换（简化：每30秒切换一次）
        int cycle = static_cast<int>(timer / 30) % 2;
        isNight = (cycle == 1);
        
        // 夜晚且有50%概率出现特殊云朵
        if (isNight && !cloudTriggered) {
            std::uniform_int_distribution<int> dist(0, 100);
            if (dist(rng) < 50) {  // 50% 概率
                specialCloudVisible = true;
                // 云朵位置随机移动
                cloudX = 50 + (rng() % 900);
                cloudY = 30 + (rng() % 100);
            }
        }
        
        // 睡眠时触发云朵事件
        if (sleeping && specialCloudVisible && !cloudTriggered) {
            cloudTriggered = true;
            specialCloudVisible = false;
        }
    }
    
    bool isSpecialCloudVisible() const { return specialCloudVisible; }
    bool isCloudTriggered() const { return cloudTriggered; }
    bool isNightTime() const { return isNight; }
    float getCloudX() const { return cloudX; }
    float getCloudY() const { return cloudY; }
    
    void triggerCloud() {
        cloudTriggered = true;
        specialCloudVisible = false;
    }
    
    void resetCloud() {
        cloudTriggered = false;
        specialCloudVisible = false;
    }
    
    json toJson() const {
        return {{"cloud_triggered", cloudTriggered}, {"timer", timer}};
    }
    void fromJson(const json& j) {
        cloudTriggered = j.value("cloud_triggered", false);
        timer = j.value("timer", 0.0f);
    }
};

// ============================================================
// 2. 情感状态系统
// ============================================================
enum class Emotion {
    Happy, Sad, Excited, Lonely, Loved, Anxious, Peaceful
};

std::string getEmoji(Emotion e) {
    switch(e) {
        case Emotion::Happy: return "😊";
        case Emotion::Sad: return "😢";
        case Emotion::Excited: return "🤩";
        case Emotion::Lonely: return "😔";
        case Emotion::Loved: return "🥰";
        case Emotion::Anxious: return "😰";
        case Emotion::Peaceful: return "😌";
        default: return "😐";
    }
}

class EmotionalState {
private:
    Emotion currentEmotion = Emotion::Peaceful;
    int loneliness = 30;
    int stability = 70;
    std::vector<std::string> memory;
public:
    void setEmotion(Emotion e) { currentEmotion = e; }
    Emotion getEmotion() const { return currentEmotion; }
    int getLoneliness() const { return loneliness; }
    int getStability() const { return stability; }
    void modifyLoneliness(int delta) { loneliness = std::clamp(loneliness + delta, 0, 100); }
    void modifyStability(int delta) { stability = std::clamp(stability + delta, 0, 100); }
    void addMemory(const std::string& event) {
        memory.push_back(event);
        if (memory.size() > 50) memory.erase(memory.begin());
    }
    json toJson() const {
        return {{"emotion", static_cast<int>(currentEmotion)}, {"loneliness", loneliness}, 
                {"stability", stability}, {"memory", memory}};
    }
    void fromJson(const json& j) {
        currentEmotion = static_cast<Emotion>(j.value("emotion", 0));
        loneliness = j.value("loneliness", 30);
        stability = j.value("stability", 70);
        memory = j.value("memory", std::vector<std::string>());
    }
};

// ============================================================
// 3. Stats - 核心属性
// ============================================================
class Stats {
private:
    std::string name;
    int age;
    std::string major;
    int semester = 1;
    double gpa = 0.0;
    int energy = 80;
    int happiness = 70;
    int money = 500;
    int eventCount = 0;
    int dayCount = 0;
    std::map<std::string, int> buffs;
    std::map<std::string, json> storyChoices;
    EmotionalState emotion;
    DuckCollection ducks;

public:
    Stats() {}
    
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getMajor() const { return major; }
    int getSemester() const { return semester; }
    double getGPA() const { return gpa; }
    int getEnergy() const { return energy; }
    int getHappiness() const { return happiness; }
    int getMoney() const { return money; }
    int getEventCount() const { return eventCount; }
    int getDayCount() const { return dayCount; }
    EmotionalState& getEmotion() { return emotion; }
    DuckCollection& getDucks() { return ducks; }
    
    void setName(const std::string& n) { name = n; }
    void setGPA(double g) { gpa = std::clamp(g, 0.0, 4.0); }
    void setMoney(int m) { money = std::max(0, m); }
    
    void modifyEnergy(int delta) { energy = std::clamp(energy + delta, 0, 100); }
    void modifyHappiness(int delta) { 
        happiness = std::clamp(happiness + delta, 0, 100);
        if (happiness > 80) emotion.setEmotion(Emotion::Happy);
        else if (happiness < 30) emotion.setEmotion(Emotion::Sad);
    }
    void modifyMoney(int delta) { money = std::max(0, money + delta); }
    void incrementEventCount() { eventCount++; }
    void nextDay() { dayCount++; }
    void nextSemester() { semester++; }
    
    void addBuff(const std::string& name, int days) { buffs[name] = days; }
    bool hasBuff(const std::string& name) const {
        return buffs.find(name) != buffs.end() && buffs.at(name) > 0;
    }
    void updateBuffs() {
        for (auto it = buffs.begin(); it != buffs.end();) {
            it->second--;
            if (it->second <= 0) it = buffs.erase(it);
            else ++it;
        }
    }
    int getBuffBonus(const std::string& type) const {
        if (hasBuff("学习效率+20%") && type == "study") return 20;
        if (hasBuff("精力恢复+50%") && type == "rest") return 50;
        return 0;
    }
    
    json toJson() const {
        return {{"name", name}, {"age", age}, {"major", major}, {"semester", semester},
                {"gpa", gpa}, {"energy", energy}, {"happiness", happiness}, {"money", money},
                {"event_count", eventCount}, {"day_count", dayCount}, {"buffs", buffs},
                {"story_choices", storyChoices}, {"emotion", emotion.toJson()},
                {"ducks", ducks.toJson()}};
    }
    void fromJson(const json& j) {
        name = j.value("name", "");
        age = j.value("age", 18);
        major = j.value("major", "");
        semester = j.value("semester", 1);
        gpa = j.value("gpa", 0.0);
        energy = j.value("energy", 80);
        happiness = j.value("happiness", 70);
        money = j.value("money", 500);
        eventCount = j.value("event_count", 0);
        dayCount = j.value("day_count", 0);
        if (j.contains("buffs")) buffs = j["buffs"].get<std::map<std::string, int>>();
        if (j.contains("story_choices")) storyChoices = j["story_choices"].get<std::map<std::string, json>>();
        if (j.contains("emotion")) emotion.fromJson(j["emotion"]);
        if (j.contains("ducks")) ducks.fromJson(j["ducks"]);
    }
};

// ============================================================
// 4. Course / CourseManager (简化)
// ============================================================
class Course {
private:
    std::string name;
    int credit;
    int score = 0;
    std::string grade = "F";
    bool isPassed = false;
public:
    Course() = default;
    Course(std::string n, int c) : name(n), credit(c) {}
    void setScore(int s) {
        score = std::clamp(s, 0, 100);
        if (score >= 90) grade = "A";
        else if (score >= 80) grade = "B";
        else if (score >= 70) grade = "C";
        else if (score >= 60) grade = "D";
        else grade = "F";
        isPassed = (grade != "F");
    }
    std::string getName() const { return name; }
    int getCredit() const { return credit; }
    int getScore() const { return score; }
    std::string getGrade() const { return grade; }
    bool getIsPassed() const { return isPassed; }
    json toJson() const { return {{"name", name}, {"credit", credit}, {"score", score}, {"grade", grade}, {"passed", isPassed}}; }
    void fromJson(const json& j) {
        name = j.value("name", "");
        credit = j.value("credit", 3);
        score = j.value("score", 0);
        grade = j.value("grade", "F");
        isPassed = j.value("passed", false);
    }
};

class CourseManager {
private:
    std::vector<Course> courses;
    std::vector<Course> availableCourses;
    std::mt19937 rng;
    int totalCoursesTaken = 0;
public:
    CourseManager() {
        rng.seed(std::time(nullptr));
        availableCourses = {
            {"高等数学", 4}, {"C++编程", 3}, {"数据结构", 3},
            {"操作系统", 3}, {"计算机网络", 3}, {"数据库原理", 3},
            {"软件工程", 2}, {"人工智能", 4}, {"机器学习", 3},
            {"英语", 2}, {"离散数学", 3}, {"编译原理", 3}
        };
    }
    std::vector<Course> getAvailableCourses() const { return availableCourses; }
    std::vector<Course> getCourses() const { return courses; }
    int getCourseCount() const { return courses.size(); }
    int getTotalCoursesTaken() const { return totalCoursesTaken; }
    bool takeCourse(const std::string& name) {
        for (auto& c : availableCourses) {
            if (c.getName() == name) {
                for (const auto& enrolled : courses) {
                    if (enrolled.getName() == name) return false;
                }
                courses.push_back(c);
                totalCoursesTaken++;
                return true;
            }
        }
        return false;
    }
    void studyForExam(const std::string& name, int hours, Stats& stats) {
        for (auto& course : courses) {
            if (course.getName() == name) {
                std::uniform_int_distribution<int> baseDist(50, 70);
                int baseScore = baseDist(rng);
                int bonus = std::min(hours * 3, 30);
                if (stats.hasBuff("学习效率+20%")) bonus = static_cast<int>(bonus * 1.2);
                if (stats.getEnergy() < 30) bonus = static_cast<int>(bonus * 0.5);
                int finalScore = std::min(baseScore + bonus, 100);
                course.setScore(finalScore);
                stats.modifyEnergy(-hours * 2);
                stats.modifyHappiness(3);
                std::cout << "📊 " << name << " 成绩: " << finalScore << " (" << course.getGrade() << ")\n";
                return;
            }
        }
        std::cout << "❌ 未找到课程: " << name << "\n";
    }
    void examWeek(Stats& stats) {
        std::cout << "\n===== 📝 考试周 =====\n";
        for (auto& course : courses) {
            if (course.getScore() == 0) {
                std::uniform_int_distribution<int> dist(1, 3);
                int hours = dist(rng);
                studyForExam(course.getName(), hours, stats);
            }
        }
    }
    double calculateGPA() const {
        if (courses.empty()) return 0.0;
        double totalPoints = 0.0;
        int totalCredits = 0;
        for (const auto& course : courses) {
            std::string grade = course.getGrade();
            double points = 0.0;
            if (grade == "A") points = 4.0;
            else if (grade == "B") points = 3.0;
            else if (grade == "C") points = 2.0;
            else if (grade == "D") points = 1.0;
            totalPoints += points * course.getCredit();
            totalCredits += course.getCredit();
        }
        return totalCredits > 0 ? totalPoints / totalCredits : 0.0;
    }
    json toJson() const {
        json j;
        for (const auto& c : courses) j.push_back(c.toJson());
        j["total_taken"] = totalCoursesTaken;
        return j;
    }
    void fromJson(const json& j) {
        courses.clear();
        if (j.is_array()) {
            for (const auto& item : j) {
                Course c;
                c.fromJson(item);
                courses.push_back(c);
            }
        }
        totalCoursesTaken = j.value("total_taken", courses.size());
    }
};

// ============================================================
// 5. SkillManager / NPC / PortalSystem / AchievementManager (简化)
// ============================================================
class SkillManager {
private:
    std::vector<std::string> skills;
public:
    void unlockSkill(const std::string& name) {
        if (std::find(skills.begin(), skills.end(), name) == skills.end()) {
            skills.push_back(name);
            std::cout << "🎯 获得新技能: " << name << "！\n";
        }
    }
    bool hasSkill(const std::string& name) const {
        return std::find(skills.begin(), skills.end(), name) != skills.end();
    }
    std::vector<std::string> getSkills() const { return skills; }
    int getSkillCount() const { return skills.size(); }
    json toJson() const { return skills; }
    void fromJson(const json& j) { skills = j.get<std::vector<std::string>>(); }
};

class NPC {
private:
    std::string name;
    int favorability = 0;
    std::vector<std::string> dialogues;
    std::mt19937 rng;
public:
    NPC(const std::string& n) : name(n) {
        rng.seed(std::time(nullptr) + std::hash<std::string>{}(n));
        if (name == "学姐") dialogues = {"你好呀！需要帮忙吗？", "学习要劳逸结合哦！", "加油，我看好你！"};
        else if (name == "室友") dialogues = {"打游戏吗？", "食堂新开了家店，一起去？", "兄弟，该交作业了！"};
        else if (name == "导师") dialogues = {"最近学习怎么样？", "你的潜力很大！", "毕业后有什么打算？"};
        else if (name == "鸭哥") dialogues = {"嘎！你找到了我！", "嘎嘎！继续收集鸭子吧！", "嘎！图书馆里有秘密！"};
    }
    std::string getName() const { return name; }
    int getFavorability() const { return favorability; }
    void interact(Stats& stats, CourseManager& cm) {
        std::uniform_int_distribution<int> dist(0, dialogues.size() - 1);
        std::cout << "💬 " << name << ": \"" << dialogues[dist(rng)] << "\"\n";
        int increase = 5 + (std::rand() % 10);
        favorability = std::min(100, favorability + increase);
        std::cout << "❤️ " << name << " 好感度 +" << increase << " (当前: " << favorability << ")\n";
        stats.modifyHappiness(3);
        stats.getEmotion().modifyLoneliness(-3);
    }
    json toJson() const { return {{"name", name}, {"favorability", favorability}}; }
    void fromJson(const json& j) {
        name = j.value("name", "");
        favorability = j.value("favorability", 0);
    }
};

class NPCManager {
private:
    std::vector<NPC> npcs;
public:
    NPCManager() {
        npcs.push_back(NPC("学姐"));
        npcs.push_back(NPC("室友"));
        npcs.push_back(NPC("导师"));
        npcs.push_back(NPC("鸭哥"));
    }
    std::vector<NPC>& getNPCs() { return npcs; }
    void interactWithNPC(int index, Stats& stats, CourseManager& cm) {
        if (index >= 0 && index < npcs.size()) npcs[index].interact(stats, cm);
    }
    json toJson() const {
        json j;
        for (const auto& npc : npcs) j.push_back(npc.toJson());
        return j;
    }
    void fromJson(const json& j) {
        for (size_t i = 0; i < j.size() && i < npcs.size(); i++) npcs[i].fromJson(j[i]);
    }
};

// ============================================================
// 6. 传送门系统 (v3.2.2)
// ============================================================
enum class PortalType { Toilet, Secret, Anniversary };

struct Portal {
    std::string id;
    std::string name;
    PortalType type;
    sf::Vector2f position;
    bool active = false;
    float activationTime = 0;
    std::string destination;
    std::string description;
};

class PortalSystem {
private:
    std::vector<Portal> portals;
    std::mt19937 rng;
    float timer = 0;
    bool anniversaryPortalActive = false;
public:
    PortalSystem() {
        rng.seed(std::time(nullptr));
        portals.push_back({"portal_toilet", "🚽 神秘厕所传送门", PortalType::Toilet, 
                          {450, 350}, false, 0, "anniversary_room", "学校厕所里出现了一个神秘的传送门！按 F 进入。"});
        portals.push_back({"portal_secret", "🔮 秘密传送门", PortalType::Secret,
                          {200, 200}, false, 0, "secret_room", "一个隐藏的传送门，只有真正的探索者才能发现。"});
    }
    
    void update(float deltaTime, const DuckCollection& ducks) {
        timer += deltaTime;
        if (timer > 30 + (rng() % 30)) {
            portals[0].active = true;
            portals[0].position = {300 + (rng() % 400), 200 + (rng() % 300)};
            portals[0].activationTime = timer;
            timer = 0;
        }
        if (portals[0].active && timer - portals[0].activationTime > 10) {
            portals[0].active = false;
        }
        if (ducks.getCollectedCount() >= 10 && !anniversaryPortalActive) {
            anniversaryPortalActive = true;
            portals.push_back({"portal_anniversary", "🎂 一周年庆典传送门", PortalType::Anniversary,
                              {500, 300}, true, 0, "magicianswand_anniversary", "欢迎来到 MagiciansWand 一周年庆典！"});
        }
    }
    
    Portal* getActivePortalAt(sf::Vector2f pos) {
        for (auto& p : portals) {
            if (!p.active) continue;
            float dx = pos.x - p.position.x;
            float dy = pos.y - p.position.y;
            if (dx*dx + dy*dy < 2500) return &p;
        }
        return nullptr;
    }
    
    bool isAnniversaryActive() const { return anniversaryPortalActive; }
    
    json toJson() const {
        json j;
        for (const auto& p : portals) {
            j[p.id] = {{"active", p.active}, {"position_x", p.position.x}, {"position_y", p.position.y}};
        }
        j["anniversary_active"] = anniversaryPortalActive;
        return j;
    }
    void fromJson(const json& j) {
        for (auto& p : portals) {
            if (j.contains(p.id)) {
                p.active = j[p.id]["active"].get<bool>();
                p.position.x = j[p.id]["position_x"].get<float>();
                p.position.y = j[p.id]["position_y"].get<float>();
            }
        }
        if (j.contains("anniversary_active")) {
            anniversaryPortalActive = j["anniversary_active"].get<bool>();
        }
    }
};

// ============================================================
// 7. 周年庆场景
// ============================================================
class AnniversaryRoom {
private:
    bool visited = false;
    int celebrationCount = 0;
public:
    bool enter(Stats& stats) {
        if (visited) {
            std::cout << "🎂 你又回来了！庆典还在继续！\n";
            celebrationCount++;
            return true;
        }
        visited = true;
        celebrationCount++;
        stats.modifyHappiness(30);
        stats.getEmotion().setEmotion(Emotion::Excited);
        stats.getEmotion().addMemory("参加了MagiciansWand一周年庆典");
        std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
        std::cout << "║           🎂🎉 MAGICIANSWAND 一周年庆典 🎉🎂           ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════╣\n";
        std::cout << "║  🎂 欢迎来到 MagiciansWand 一周年庆典！                 ║\n";
        std::cout << "║  🎉 感谢你成为我们的一员！                             ║\n";
        std::cout << "║  🌟 过去一年，我们一起成长！                           ║\n";
        std::cout << "║  🚀 未来一年，我们一起飞得更高！                       ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════╝\n";
        std::cout << "\n🎁 你获得了 500 元礼金！\n";
        stats.modifyMoney(500);
        return true;
    }
    bool isVisited() const { return visited; }
    json toJson() const { return {{"visited", visited}, {"count", celebrationCount}}; }
    void fromJson(const json& j) {
        visited = j.value("visited", false);
        celebrationCount = j.value("count", 0);
    }
};

// ============================================================
// 8. 成就系统 (v3.2.3 新增云朵成就)
// ============================================================
struct Achievement {
    std::string id, name, description, icon;
    bool unlocked = false;
    std::function<bool(const json&)> condition;
    Achievement(const std::string& i, const std::string& n, const std::string& d, 
                const std::string& ic, std::function<bool(const json&)> cond)
        : id(i), name(n), description(d), icon(ic), condition(cond) {}
};

class AchievementManager {
private:
    std::vector<Achievement> achievements;
    std::vector<std::string> unlockedIds;
public:
    AchievementManager() { initAchievements(); }
    
    void initAchievements() {
        achievements = {
            {"first_course", "📚 初入学术", "选修第一门课程", "📚", 
             [](const json& data) { return data["courses"].size() >= 1; }},
            {"study_hard", "💪 学霸之路", "一门课程考到90分以上", "💪",
             [](const json& data) { for (auto& c : data["courses"]) if (c["score"].get<int>() >= 90) return true; return false; }},
            {"gpa_35", "🌟 优秀毕业生", "GPA达到3.5以上", "🌟",
             [](const json& data) { return data["gpa"].get<double>() >= 3.5; }},
            {"duck_collector_3", "🦆 初级鸭迷", "收集3只鸭子", "🦆",
             [](const json& data) { return data["ducks_collected"].get<int>() >= 3; }},
            {"duck_collector_6", "🦆 中级鸭迷", "收集6只鸭子", "🦆",
             [](const json& data) { return data["ducks_collected"].get<int>() >= 6; }},
            {"duck_collector_9", "🦆 高级鸭迷", "收集9只鸭子", "🦆",
             [](const json& data) { return data["ducks_collected"].get<int>() >= 9; }},
            {"duck_king", "🏆 鸭王之王", "收集全部16只鸭子", "🏆",
             [](const json& data) { return data["ducks_collected"].get<int>() >= 16; }},
            // v3.2.3 新增成就
            {"cloud_seeker", "☁️ 追云者", "在夜晚看到特殊云朵", "☁️",
             [](const json& data) { return data["cloud_seen"].get<bool>(); }},
            {"caellab_visitor", "🔗 虚舟访客", "通过云朵打开CaelLab网站", "🔗",
             [](const json& data) { return data["caellab_visited"].get<bool>(); }}
        };
    }
    
    void checkAchievements(const json& data, std::vector<std::string>& newUnlocks) {
        for (auto& a : achievements) {
            if (!a.unlocked && a.condition(data)) {
                a.unlocked = true;
                unlockedIds.push_back(a.id);
                newUnlocks.push_back(a.icon + " " + a.name + " - " + a.description);
            }
        }
    }
    
    std::vector<Achievement> getAchievements() const { return achievements; }
    int getUnlockedCount() const { return unlockedIds.size(); }
    int getTotalCount() const { return achievements.size(); }
    json toJson() const {
        json j;
        for (const auto& a : achievements) j[a.id] = a.unlocked;
        return j;
    }
    void fromJson(const json& j) {
        unlockedIds.clear();
        for (auto& a : achievements) {
            if (j.contains(a.id)) {
                a.unlocked = j[a.id].get<bool>();
                if (a.unlocked) unlockedIds.push_back(a.id);
            }
        }
    }
};

// ============================================================
// 9. SaveManager
// ============================================================
class SaveManager {
private:
    std::string saveDirectory;
    std::string currentSlot = "autosave";
public:
    SaveManager() {
#ifdef _WIN32
        saveDirectory = std::string(getenv("APPDATA")) + "\\StudentSimulator\\saves\\";
#elif __APPLE__
        saveDirectory = std::string(getenv("HOME")) + "/Library/Application Support/StudentSimulator/saves/";
#else
        saveDirectory = std::string(getenv("HOME")) + "/.student_simulator/saves/";
#endif
        fs::create_directories(saveDirectory);
    }
    
    bool save(const Stats& stats, const CourseManager& cm, const SkillManager& sm,
              const NPCManager& npcm, const AchievementManager& am,
              const PortalSystem& portals, const CloudSystem& clouds) {
        json data = {
            {"stats", stats.toJson()}, {"courses", cm.toJson()}, {"skills", sm.toJson()},
            {"npcs", npcm.toJson()}, {"achievements", am.toJson()},
            {"portals", portals.toJson()}, {"clouds", clouds.toJson()},
            {"save_time", std::time(nullptr)}, {"version", "3.2.3"}
        };
        std::string filename = saveDirectory + currentSlot + ".json";
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << data.dump(4);
        file.close();
        std::cout << "💾 存档成功\n";
        return true;
    }
    
    bool load(Stats& stats, CourseManager& cm, SkillManager& sm,
              NPCManager& npcm, AchievementManager& am,
              PortalSystem& portals, CloudSystem& clouds) {
        std::string filename = saveDirectory + currentSlot + ".json";
        if (!fs::exists(filename)) return false;
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        json data;
        file >> data;
        file.close();
        stats.fromJson(data["stats"]);
        cm.fromJson(data["courses"]);
        sm.fromJson(data["skills"]);
        npcm.fromJson(data["npcs"]);
        if (data.contains("achievements")) am.fromJson(data["achievements"]);
        if (data.contains("portals")) portals.fromJson(data["portals"]);
        if (data.contains("clouds")) clouds.fromJson(data["clouds"]);
        std::cout << "💾 读档成功\n";
        return true;
    }
    
    std::vector<std::string> listSaves() {
        std::vector<std::string> saves;
        for (const auto& entry : fs::directory_iterator(saveDirectory)) {
            if (entry.path().extension() == ".json") saves.push_back(entry.path().stem().string());
        }
        return saves;
    }
};

// ============================================================
// 10. GameManager - 主游戏管理 (v3.2.3)
// ============================================================
class GameManager {
private:
    sf::RenderWindow window;
    
    Stats stats;
    CourseManager courseManager;
    SkillManager skillManager;
    NPCManager npcManager;
    AchievementManager achievementManager;
    SaveManager saveManager;
    PortalSystem portalSystem;
    AnniversaryRoom anniversaryRoom;
    CloudSystem cloudSystem;
    
    std::vector<std::string> messages;
    std::vector<std::string> pendingAchievements;
    
    bool isRunning = true;
    bool isInputting = false;
    bool inAnniversaryScene = false;
    bool isSleeping = false;
    bool cloudEventTriggered = false;
    std::string inputBuffer;
    std::string inputPrompt;
    
    sf::Font font;
    sf::CircleShape playerIcon;
    sf::RectangleShape statusPanel;
    sf::RectangleShape menuPanel;
    sf::RectangleShape messagePanel;
    sf::CircleShape cloudShape;
    sf::Text cloudText;
    
    sf::Clock clock;
    float nightTimer = 0;
    
public:
    GameManager() {
        window.create(sf::VideoMode(1024, 768), "🎓 学生生活模拟器 v3.2.3", sf::Style::Close);
        window.setFramerateLimit(60);
        if (!loadFont()) std::cerr << "⚠️ 字体加载失败\n";
        setupUI();
        initPlayer();
        loadGame();
        
        // 初始化云朵
        cloudShape.setRadius(40);
        cloudShape.setFillColor({200, 200, 255, 180});
        cloudShape.setOutlineColor({255, 255, 255, 200});
        cloudShape.setOutlineThickness(2);
        
        cloudText.setFont(font);
        cloudText.setString("☁️ ?");
        cloudText.setCharacterSize(20);
        cloudText.setFillColor(sf::Color::White);
    }
    
    bool loadFont() {
        std::vector<std::string> paths = {
            "C:\\Windows\\Fonts\\msyh.ttc", "C:\\Windows\\Fonts\\simhei.ttf",
            "C:\\Windows\\Fonts\\arial.ttf", "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
            "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc", "/System/Library/Fonts/PingFang.ttc"
        };
        for (const auto& p : paths) {
            if (fs::exists(p) && font.loadFromFile(p)) return true;
        }
        return font.loadFromFile("");
    }
    
    void setupUI() {
        statusPanel.setSize({300, 280});
        statusPanel.setPosition({10, 10});
        statusPanel.setFillColor({50, 50, 50, 230});
        statusPanel.setOutlineColor(sf::Color::White);
        statusPanel.setOutlineThickness(1);
        
        menuPanel.setSize({300, 380});
        menuPanel.setPosition({10, 300});
        menuPanel.setFillColor({50, 50, 50, 230});
        menuPanel.setOutlineColor(sf::Color::White);
        menuPanel.setOutlineThickness(1);
        
        messagePanel.setSize({700, 150});
        messagePanel.setPosition({320, 600});
        messagePanel.setFillColor({50, 50, 50, 230});
        messagePanel.setOutlineColor(sf::Color::White);
        messagePanel.setOutlineThickness(1);
    }
    
    void initPlayer() {
        playerIcon.setRadius(15);
        playerIcon.setFillColor(sf::Color::Cyan);
        playerIcon.setPosition(400, 300);
        playerIcon.setOutlineColor(sf::Color::White);
        playerIcon.setOutlineThickness(2);
    }
    
    void loadGame() {
        auto saves = saveManager.listSaves();
        if (!saves.empty()) {
            std::cout << "📂 检测到存档\n是否加载? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                if (saveManager.load(stats, courseManager, skillManager, 
                                     npcManager, achievementManager, portalSystem, cloudSystem)) {
                    std::cout << "✅ 加载成功！\n";
                    checkAchievements();
                }
            }
        }
        if (stats.getName().empty()) createCharacter();
        addMessage("🎓 学生生活模拟器 v3.2.3");
        addMessage("☁️ 新功能: 特殊云朵 | 夜晚睡觉触发");
        addMessage("🌙 夜晚睡觉有50%概率看到特殊云朵");
        addMessage("🏠 方向键移动 | Enter交互 | 1-9菜单");
    }
    
    void createCharacter() {
        std::string name, major;
        int age;
        std::cout << "\n🎓 创建角色\n请输入姓名: ";
        std::cin >> name;
        std::cout << "请输入年龄: ";
        std::cin >> age;
        std::cout << "请输入专业: ";
        std::cin >> major;
        stats.setName(name);
        addMessage("🎓 欢迎 " + name + " 来到大学！");
    }
    
    void run() {
        while (window.isOpen() && isRunning) {
            handleEvents();
            update();
            render();
        }
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                saveManager.save(stats, courseManager, skillManager, 
                                npcManager, achievementManager, portalSystem, cloudSystem);
                window.close();
                isRunning = false;
            }
            if (event.type == sf::Event::KeyPressed) handleKeyPress(event.key.code);
            if (event.type == sf::Event::TextEntered && isInputting) {
                if (event.text.unicode == 13) processInput();
                else if (event.text.unicode == 8) { if (!inputBuffer.empty()) inputBuffer.pop_back(); }
                else if (event.text.unicode < 128) inputBuffer += static_cast<char>(event.text.unicode);
            }
        }
    }
    
    void handleKeyPress(sf::Keyboard::Key key) {
        if (isInputting) return;
        
        float speed = 5.0f;
        auto pos = playerIcon.getPosition();
        if (key == sf::Keyboard::Up) pos.y -= speed;
        if (key == sf::Keyboard::Down) pos.y += speed;
        if (key == sf::Keyboard::Left) pos.x -= speed;
        if (key == sf::Keyboard::Right) pos.x += speed;
        pos.x = std::clamp(pos.x, 20.0f, 1000.0f);
        pos.y = std::clamp(pos.y, 20.0f, 740.0f);
        playerIcon.setPosition(pos);
        
        // 传送门交互
        if (key == sf::Keyboard::F) {
            auto* portal = portalSystem.getActivePortalAt(pos);
            if (portal) {
                if (portal->type == PortalType::Anniversary) {
                    inAnniversaryScene = true;
                    anniversaryRoom.enter(stats);
                    checkAchievements();
                    addMessage("🎂 进入周年庆场景！");
                } else if (portal->type == PortalType::Toilet) {
                    addMessage("🚽 你穿过传送门来到了一个神秘的地方！");
                    portal->active = false;
                    std::uniform_int_distribution<int> dist(0, 2);
                    int reward = dist(clock.getElapsedTime().asMilliseconds() % 1000);
                    if (reward == 0) { stats.modifyMoney(100); addMessage("💰 发现 100 元！"); }
                    else if (reward == 1) { stats.modifyHappiness(10); addMessage("😊 心情变好了！"); }
                    else { stats.modifyEnergy(20); addMessage("⚡ 精力恢复！"); }
                    checkAchievements();
                }
            }
        }
        
        // 菜单
        if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num9) {
            executeMenuOption(key - sf::Keyboard::Num1 + 1);
        }
        
        // 快捷键
        if (key == sf::Keyboard::S) {
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, portalSystem, cloudSystem);
            addMessage("💾 存档成功");
        }
        if (key == sf::Keyboard::L) {
            if (saveManager.load(stats, courseManager, skillManager, 
                                 npcManager, achievementManager, portalSystem, cloudSystem)) {
                addMessage("💾 读档成功");
                checkAchievements();
            }
        }
        if (key == sf::Keyboard::Escape) {
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, portalSystem, cloudSystem);
            window.close();
            isRunning = false;
        }
    }
    
    void executeMenuOption(int option) {
        switch (option) {
            case 1: showStatus(); break;
            case 2: showCourses(); break;
            case 3: studyMenu(); break;
            case 4: interactNPC(); break;
            case 5: examWeek(); break;
            case 6: nextSemester(); break;
            case 7: sleep(); break;      // 休息 → 睡眠 (触发云朵)
            case 8: slackOff(); break;
            case 9: graduate(); break;
            default: addMessage("❌ 无效选项");
        }
        saveManager.save(stats, courseManager, skillManager, 
                        npcManager, achievementManager, portalSystem, cloudSystem);
    }
    
    void showStatus() {
        auto& emo = stats.getEmotion();
        std::stringstream ss;
        ss << "📊 " << stats.getName() << " | 学期: " << stats.getSemester()
           << " | GPA: " << std::fixed << std::setprecision(2) << stats.getGPA()
           << " | 课程: " << courseManager.getCourseCount()
           << " | 技能: " << skillManager.getSkillCount()
           << " | 💰 " << stats.getMoney()
           << "\n🦆 鸭子: " << stats.getDucks().getCollectedCount() 
           << "/" << stats.getDucks().getTotalCount()
           << " | 😊 " << getEmoji(emo.getEmotion());
        addMessage(ss.str());
    }
    
    void showCourses() {
        auto available = courseManager.getAvailableCourses();
        if (available.empty()) { addMessage("❌ 没有更多课程可选"); return; }
        std::cout << "\n📚 可选课程:\n";
        for (size_t i = 0; i < available.size(); i++) {
            std::cout << "  " << i+1 << ". " << available[i].getName() 
                      << " (" << available[i].getCredit() << "学分)\n";
        }
        std::cout << "选择 (0取消): ";
        int choice; std::cin >> choice;
        if (choice > 0 && choice <= available.size()) {
            if (courseManager.takeCourse(available[choice-1].getName())) {
                addMessage("📖 选修了: " + available[choice-1].getName());
                stats.modifyEnergy(-5);
                checkAchievements();
            }
        }
    }
    
    void studyMenu() {
        auto courses = courseManager.getCourses();
        if (courses.empty()) { addMessage("⚠️ 请先选修课程！"); return; }
        std::cout << "\n📝 选择课程:\n";
        for (size_t i = 0; i < courses.size(); i++) {
            std::cout << "  " << i+1 << ". " << courses[i].getName() 
                      << (courses[i].getScore() > 0 ? " (已考)" : " (未考)") << "\n";
        }
        std::cout << "选择 (0取消): ";
        int choice; std::cin >> choice;
        if (choice > 0 && choice <= courses.size()) {
            int hours; std::cout << "学习几个小时? (1-5): ";
            std::cin >> hours; hours = std::clamp(hours, 1, 5);
            courseManager.studyForExam(courses[choice-1].getName(), hours, stats);
            stats.setGPA(courseManager.calculateGPA());
            addMessage("📝 学习了 " + courses[choice-1].getName());
            if (courses[choice-1].getScore() >= 85) {
                std::vector<std::string> skills = {"编程能力", "数学思维", "英语应用", "数据分析"};
                std::uniform_int_distribution<int> dist(0, skills.size()-1);
                std::mt19937 rng(std::time(nullptr));
                skillManager.unlockSkill(skills[dist(rng)]);
                checkAchievements();
            }
        }
    }
    
    void interactNPC() {
        auto& npcs = npcManager.getNPCs();
        std::cout << "\n💬 NPC列表:\n";
        for (size_t i = 0; i < npcs.size(); i++) {
            std::cout << "  " << i+1 << ". " << npcs[i].getName() 
                      << " (好感度: " << npcs[i].getFavorability() << ")\n";
        }
        std::cout << "选择 (0取消): ";
        int choice; std::cin >> choice;
        if (choice > 0 && choice <= npcs.size()) {
            npcManager.interactWithNPC(choice-1, stats, courseManager);
            addMessage("💬 与 " + npcs[choice-1].getName() + " 互动");
            checkAchievements();
        }
    }
    
    void examWeek() {
        if (courseManager.getCourses().empty()) { addMessage("⚠️ 请先选修课程！"); return; }
        courseManager.examWeek(stats);
        stats.setGPA(courseManager.calculateGPA());
        addMessage("📝 考试周结束！");
        checkAchievements();
    }
    
    void nextSemester() {
        stats.nextSemester();
        stats.modifyEnergy(20);
        stats.modifyHappiness(10);
        stats.updateBuffs();
        stats.setGPA(courseManager.calculateGPA());
        addMessage("📅 进入第 " + std::to_string(stats.getSemester()) + " 学期");
        checkAchievements();
    }
    
    // ===== 睡眠系统 (v3.2.3 核心) =====
    void sleep() {
        isSleeping = true;
        int bonus = stats.hasBuff("精力恢复+50%") ? 60 : 40;
        stats.modifyEnergy(bonus);
        stats.nextDay();
        stats.getEmotion().modifyLoneliness(-5);
        addMessage("😴 睡了一觉，精力 +" + std::to_string(bonus));
        
        // 检查云朵事件
        if (!cloudSystem.isCloudTriggered()) {
            // 模拟夜晚 + 50%概率
            std::uniform_int_distribution<int> dist(0, 100);
            if (dist(std::mt19937(std::time(nullptr))) < 50) {
                // 触发云朵事件！
                cloudSystem.triggerCloud();
                cloudEventTriggered = true;
                triggerCloudEvent();
            }
        }
        isSleeping = false;
    }
    
    void triggerCloudEvent() {
        std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    ☁️ 你看到了一朵特殊的云 ☁️              ║\n";
        std::cout << "╠══════════════════════════════════════════════════════════╣\n";
        std::cout << "║  🌙 夜空中，一朵发光的云缓缓飘过...                      ║\n";
        std::cout << "║  ✨ 云朵上浮现出一行字:                                 ║\n";
        std::cout << "║                                                         ║\n";
        std::cout << "║     \"MagiciansWand 的合作伙伴 —— 虚舟实验室 (CaelLab)\"   ║\n";
        std::cout << "║                                                         ║\n";
        std::cout << "║  🔗 正在打开: https://www.caellab.com/                  ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════╝\n";
        
        addMessage("☁️ 你看到了特殊云朵！正在打开 CaelLab...");
        
        // 打开网页
        std::string url = "https://www.caellab.com/";
#ifdef _WIN32
        std::string cmd = "start " + url;
#elif __APPLE__
        std::string cmd = "open " + url;
#else
        std::string cmd = "xdg-open " + url;
#endif
        system(cmd.c_str());
        
        // 记录成就
        checkAchievements();
        
        // 奖励
        stats.modifyHappiness(10);
        stats.getEmotion().addMemory("看到了特殊云朵，访问了CaelLab");
        addMessage("🎁 你获得了 '追云者' 成就！");
    }
    
    void slackOff() {
        stats.modifyHappiness(5);
        stats.modifyEnergy(-5);
        stats.nextDay();
        addMessage("🎮 摸鱼中... 快乐+5 精力-5");
    }
    
    void graduate() {
        if (stats.getSemester() >= 8 && stats.getGPA() >= 2.0 && courseManager.getCourseCount() >= 20) {
            addMessage("🎉🎉🎉 恭喜毕业！！！");
            addMessage("📊 最终GPA: " + std::to_string(stats.getGPA()));
            addMessage("🦆 鸭子收集: " + std::to_string(stats.getDucks().getCollectedCount()) + "只");
            addMessage("🏆 成就解锁: " + std::to_string(achievementManager.getUnlockedCount()) + "个");
            isRunning = false;
        } else {
            std::string reason;
            if (stats.getSemester() < 8) reason += "学期不足(" + std::to_string(stats.getSemester()) + "/8) ";
            if (stats.getGPA() < 2.0) reason += "GPA不足(" + std::to_string(stats.getGPA()) + "/2.0) ";
            if (courseManager.getCourseCount() < 20) reason += "课程不足(" + std::to_string(courseManager.getCourseCount()) + "/20)";
            addMessage("❌ 不满足毕业条件: " + reason);
        }
    }
    
    void checkAchievements() {
        json data = {
            {"courses", courseManager.toJson()},
            {"gpa", stats.getGPA()},
            {"money", stats.getMoney()},
            {"semester", stats.getSemester()},
            {"ducks_collected", stats.getDucks().getCollectedCount()},
            {"breed_count", 0},
            {"portal_used", false},
            {"anniversary_visited", anniversaryRoom.isVisited()},
            {"cloud_seen", cloudSystem.isCloudTriggered()},
            {"caellab_visited", cloudSystem.isCloudTriggered()} // 触发即算访问
        };
        std::vector<std::string> newUnlocks;
        achievementManager.checkAchievements(data, newUnlocks);
        for (const auto& unlock : newUnlocks) {
            pendingAchievements.push_back(unlock);
            addMessage("🏆 解锁成就: " + unlock);
        }
    }
    
    void addMessage(const std::string& msg) {
        messages.push_back(msg);
        if (messages.size() > 20) messages.erase(messages.begin());
        std::cout << msg << "\n";
    }
    
    void update() {
        float delta = clock.restart().asSeconds();
        portalSystem.update(delta, stats.getDucks());
        cloudSystem.update(delta, isSleeping);
        
        if (stats.getEnergy() <= 0) { addMessage("😵 精力耗尽..."); isRunning = false; }
        if (stats.getHappiness() <= 0) { addMessage("😢 太不开心了..."); isRunning = false; }
        stats.updateBuffs();
        
        static int frame = 0;
        if (++frame % 1200 == 0) {
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, portalSystem, cloudSystem);
        }
    }
    
    void render() {
        window.clear(cloudSystem.isNightTime() ? sf::Color(10, 10, 30) : sf::Color(34, 139, 34));
        
        // 夜晚星星
        if (cloudSystem.isNightTime()) {
            for (int i = 0; i < 50; i++) {
                sf::CircleShape star(1 + (i % 2));
                star.setPosition((i * 37) % 1024, (i * 23) % 768);
                star.setFillColor({255, 255, 255, 100 + (i % 100)});
                window.draw(star);
            }
        }
        
        // 绘制特殊云朵
        if (cloudSystem.isSpecialCloudVisible()) {
            cloudShape.setPosition(cloudSystem.getCloudX(), cloudSystem.getCloudY());
            cloudShape.setFillColor({200, 200, 255, 180});
            window.draw(cloudShape);
            
            // 云朵发光效果
            sf::CircleShape glow(60);
            glow.setPosition(cloudSystem.getCloudX() - 20, cloudSystem.getCloudY() - 20);
            glow.setFillColor({200, 200, 255, 50});
            window.draw(glow);
            
            cloudText.setString("☁️ ?");
            cloudText.setPosition(cloudSystem.getCloudX() + 5, cloudSystem.getCloudY() - 10);
            window.draw(cloudText);
        }
        
        // 网格 (白天)
        if (!cloudSystem.isNightTime()) {
            for (int i = 0; i < 10; i++) {
                sf::RectangleShape line({1000, 1});
                line.setPosition(10, 100 + i * 60);
                line.setFillColor({255, 255, 255, 30});
                window.draw(line);
            }
        }
        
        // 传送门
        auto* portal = portalSystem.getActivePortalAt(playerIcon.getPosition());
        if (portal) {
            sf::CircleShape portalGlow(40);
            portalGlow.setPosition(portal->position.x - 40, portal->position.y - 40);
            portalGlow.setFillColor({255, 0, 255, 100});
            portalGlow.setOutlineColor({255, 255, 255, 150});
            portalGlow.setOutlineThickness(2);
            window.draw(portalGlow);
            
            sf::Text portalText;
            portalText.setFont(font);
            portalText.setString("🚽 传送门 (按F)");
            portalText.setCharacterSize(14);
            portalText.setFillColor(sf::Color::White);
            portalText.setPosition(portal->position.x - 40, portal->position.y - 70);
            window.draw(portalText);
        }
        
        // UI
        window.draw(statusPanel);
        drawStatusInfo();
        
        window.draw(menuPanel);
        drawMenu();
        
        window.draw(messagePanel);
        drawMessages();
        
        // 成就弹窗
        if (!pendingAchievements.empty()) {
            sf::RectangleShape popup({680, 40});
            popup.setPosition(330, 560);
            popup.setFillColor({255, 215, 0, 200});
            popup.setOutlineColor(sf::Color::White);
            popup.setOutlineThickness(1);
            window.draw(popup);
            
            sf::Text text;
            text.setFont(font);
            text.setString("🏆 " + pendingAchievements[0]);
            text.setCharacterSize(16);
            text.setFillColor(sf::Color::Black);
            text.setPosition(340, 565);
            window.draw(text);
            pendingAchievements.erase(pendingAchievements.begin());
        }
        
        // 玩家
        window.draw(playerIcon);
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString("🎓 " + stats.getName());
        nameText.setCharacterSize(14);
        nameText.setFillColor(sf::Color::White);
        auto pos = playerIcon.getPosition();
        nameText.setPosition(pos.x - 20, pos.y - 30);
        window.draw(nameText);
        
        // 底部提示
        sf::Text hint;
        hint.setFont(font);
        hint.setString(cloudSystem.isNightTime() ? "🌙 夜晚 | 方向键移动 | Enter交互 | 7睡觉" : 
                       "方向键移动 | Enter交互 | 1-9菜单 | S存档 | L读档 | 7睡觉");
        hint.setCharacterSize(12);
        hint.setFillColor(sf::Color::White);
        hint.setPosition(320, 755);
        window.draw(hint);
        
        // 周年庆场景
        if (inAnniversaryScene) {
            drawAnniversaryScene();
        }
        
        window.display();
    }
    
    void drawStatusInfo() {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
        
        auto& emo = stats.getEmotion();
        std::stringstream ss;
        ss << "📊 " << stats.getName() << "\n"
           << "📅 学期: " << stats.getSemester() << "\n"
           << "📈 GPA: " << std::fixed << std::setprecision(2) << stats.getGPA() << "\n"
           << "📚 课程: " << courseManager.getCourseCount() << "门\n"
           << "⭐ 技能: " << skillManager.getSkillCount() << "个\n"
           << "💰 " << stats.getMoney() << "元\n"
           << "🦆 鸭子: " << stats.getDucks().getCollectedCount() 
           << "/" << stats.getDucks().getTotalCount() << "\n"
           << "☁️ 云朵: " << (cloudSystem.isCloudTriggered() ? "✅ 已见" : "❌ 未触发") << "\n"
           << "😊 " << getEmoji(emo.getEmotion());
        text.setString(ss.str());
        text.setPosition(20, 20);
        window.draw(text);
        
        drawProgressBar(20, 240, 200, 10, stats.getEnergy(), sf::Color::Green, "精力");
        drawProgressBar(20, 265, 200, 10, stats.getHappiness(), sf::Color::Yellow, "快乐");
    }
    
    void drawProgressBar(float x, float y, float width, float height, int value, sf::Color color, const std::string& label) {
        sf::RectangleShape bg({width, height});
        bg.setPosition(x, y);
        bg.setFillColor({80, 80, 80});
        window.draw(bg);
        float fill = std::clamp(value / 100.0f, 0.0f, 1.0f);
        sf::RectangleShape bar({width * fill, height});
        bar.setPosition(x, y);
        bar.setFillColor(color);
        window.draw(bar);
        sf::Text text;
        text.setFont(font);
        text.setString(label + ": " + std::to_string(value) + "%");
        text.setCharacterSize(11);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y - 14);
        window.draw(text);
    }
    
    void drawMenu() {
        std::vector<std::string> items = {
            "1. 📊 状态", "2. 📖 选课", "3. 📝 学习",
            "4. 💬 NPC", "5. 📝 考试周", "6. 📅 新学期",
            "7. 😴 睡觉", "8. 🎮 摸鱼", "9. 🎓 毕业"
        };
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::White);
        for (size_t i = 0; i < items.size(); i++) {
            text.setString(items[i]);
            text.setPosition(20, 310 + i * 35);
            window.draw(text);
        }
        sf::Text hint;
        hint.setFont(font);
        hint.setString("☁️ 夜晚睡觉有50%概率看到特殊云朵");
        hint.setCharacterSize(13);
        hint.setFillColor(sf::Color::Yellow);
        hint.setPosition(20, 630);
        window.draw(hint);
    }
    
    void drawMessages() {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(13);
        text.setFillColor(sf::Color::White);
        int start = std::max(0, (int)messages.size() - 6);
        for (int i = start; i < (int)messages.size(); i++) {
            text.setString(messages[i]);
            text.setPosition(330, 610 + (i - start) * 22);
            window.draw(text);
        }
    }
    
    void drawAnniversaryScene() {
        sf::RectangleShape overlay({1024, 768});
        overlay.setPosition(0, 0);
        overlay.setFillColor({0, 0, 0, 200});
        window.draw(overlay);
        
        sf::RectangleShape box({800, 400});
        box.setPosition(112, 184);
        box.setFillColor({40, 40, 40, 240});
        box.setOutlineColor({255, 215, 0, 255});
        box.setOutlineThickness(3);
        window.draw(box);
        
        sf::Text title;
        title.setFont(font);
        title.setString("🎂🎉 MagiciansWand 一周年庆典 🎉🎂");
        title.setCharacterSize(32);
        title.setFillColor({255, 215, 0, 255});
        title.setPosition(130, 220);
        window.draw(title);
        
        sf::Text desc;
        desc.setFont(font);
        desc.setString("感谢你成为 MagiciansWand 的一员！\n"
                       "过去一年，我们一起成长，一起创造！\n"
                       "未来一年，我们将飞得更高！\n\n"
                       "🎁 你获得了 500 元礼金\n"
                       "🏆 解锁成就: 周年庆嘉宾\n"
                       "\n点击任意键继续...");
        desc.setCharacterSize(20);
        desc.setFillColor(sf::Color::White);
        desc.setPosition(130, 300);
        window.draw(desc);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::AnyKey)) {
            inAnniversaryScene = false;
        }
    }
    
    void processInput() {
        isInputting = false;
        if (!inputBuffer.empty()) {
            stats.setName(inputBuffer);
            addMessage("✅ 名字已改为: " + inputBuffer);
        }
        inputBuffer.clear();
    }
};

// ============================================================
// 11. main
// ============================================================
int main() {
    std::srand(std::time(nullptr));
    std::cout << "🎓 学生生活模拟器 v3.2.3\n";
    std::cout << "========================================\n";
    std::cout << "✨ v3.2.3 新功能:\n";
    std::cout << "  ☁️ 特殊云朵 (夜晚睡觉50%概率触发)\n";
    std::cout << "  🔗 虚舟实验室 (CaelLab) 联动\n";
    std::cout << "  🌙 昼夜循环效果\n";
    std::cout << "  🏆 新成就: 追云者 / 虚舟访客\n";
    std::cout << "========================================\n\n";
    
    try {
        GameManager game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "❌ 程序异常: " << e.what() << "\n";
        std::cout << "按任意键退出...\n";
        std::cin.get();
        return 1;
    }
    
    std::cout << "游戏已结束！\n";
    return 0;
}
