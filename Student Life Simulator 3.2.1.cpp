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

using json = nlohmann::json;
namespace fs = std::filesystem;

// ============================================================
// 1. 鸭子系统 (v3.2.1 新增)
// ============================================================
struct Duck {
    std::string id;
    std::string name;
    std::string description;
    std::string location;
    bool found = false;
    float x, y;  // 鸭子在地图上的位置
    
    Duck(const std::string& i, const std::string& n, const std::string& d, 
         const std::string& loc, float px, float py)
        : id(i), name(n), description(d), location(loc), x(px), y(py) {}
};

class DuckManager {
private:
    std::vector<Duck> ducks;
    int foundCount = 0;
    std::vector<std::string> foundIds;
    
public:
    DuckManager() {
        initDucks();
    }
    
    void initDucks() {
        ducks = {
            {"duck_001", "小黄", "一只黄色的小鸭子，非常可爱。", "湖中心", 400, 350},
            {"duck_002", "大花", "身上有花纹的鸭子，看起来很神气。", "湖西岸", 280, 380},
            {"duck_003", "斑斑", "斑点鸭，总是独自游来游去。", "湖东岸", 520, 370},
            {"duck_004", "雪球", "白色的鸭子，像一团雪球。", "图书馆旁", 260, 460},
            {"duck_005", "黑豆", "黑色的鸭子，眼睛像两颗黑豆。", "食堂附近", 560, 460},
            {"duck_006", "小橙", "橙色的喙，非常醒目。", "教学楼旁", 420, 280},
            {"duck_007", "灰灰", "灰色的鸭子，不太起眼。", "宿舍旁", 160, 480},
            {"duck_008", "彩虹", "羽毛有七种颜色，传说它能带来好运。", "健身房旁", 720, 480},
            {"duck_009", "小不点", "最小的鸭子，总是跟在妈妈后面。", "湖心岛", 400, 320},
            {"duck_010", "鸭哥", "神秘的鸭子，据说它是鸭群的首领。", "隐藏位置", 620, 200}
        };
    }
    
    bool findDuck(const std::string& id) {
        for (auto& duck : ducks) {
            if (duck.id == id && !duck.found) {
                duck.found = true;
                foundIds.push_back(id);
                foundCount++;
                std::cout << "🦆 发现鸭子: " << duck.name << "！(" << foundCount << "/10)\n";
                return true;
            }
        }
        return false;
    }
    
    bool findDuckByPosition(float x, float y) {
        for (auto& duck : ducks) {
            float dx = duck.x - x;
            float dy = duck.y - y;
            if (dx*dx + dy*dy < 2500 && !duck.found) {  // 半径50以内
                duck.found = true;
                foundIds.push_back(duck.id);
                foundCount++;
                std::cout << "🦆 发现鸭子: " << duck.name << "！(" << foundCount << "/10)\n";
                return true;
            }
        }
        return false;
    }
    
    bool isAllFound() const { return foundCount >= 10; }
    int getFoundCount() const { return foundCount; }
    int getTotalCount() const { return ducks.size(); }
    std::vector<Duck> getDucks() const { return ducks; }
    std::vector<std::string> getFoundIds() const { return foundIds; }
    
    json toJson() const {
        json j;
        for (const auto& d : ducks) {
            j[d.id] = d.found;
        }
        return j;
    }
    
    void fromJson(const json& j) {
        foundCount = 0;
        foundIds.clear();
        for (auto& d : ducks) {
            if (j.contains(d.id)) {
                d.found = j[d.id].get<bool>();
                if (d.found) {
                    foundCount++;
                    foundIds.push_back(d.id);
                }
            }
        }
    }
};

// ============================================================
// 2. 彩蛋系统 (v3.2.1 新增)
// ============================================================
struct EasterEgg {
    std::string id;
    std::string name;
    std::string description;
    bool found = false;
    std::function<void(Stats&, EmotionalState&)> effect;
};

class EasterEggManager {
private:
    std::vector<EasterEgg> eggs;
    int foundCount = 0;
    
public:
    EasterEggManager() {
        initEggs();
    }
    
    void initEggs() {
        eggs = {
            {
                "egg_duck_book",
                "📖 《鸭科动物大全》",
                "你在图书馆发现了一本关于鸭子的书，里面夹着一张纸条：\n"
                "'所有鸭子都在等你——鸭哥'",
                [](Stats& stats, EmotionalState& emotion) {
                    stats.modifyHappiness(10);
                    emotion.addMemory("📖 阅读了《鸭科动物大全》，对鸭子有了更深的了解");
                    std::cout << "📖 你获得了一个隐藏线索！\n";
                }
            },
            {
                "egg_developer_note",
                "💌 开发者留言",
                "你在代码的注释里发现了一行隐藏文字：\n"
                "'如果你看到了这条消息，说明你是个真正的探索者！'",
                [](Stats& stats, EmotionalState& emotion) {
                    stats.modifyHappiness(15);
                    stats.getEmotion().setEmotion(Emotion::Excited);
                    std::cout << "💌 你发现了开发者的秘密留言！\n";
                }
            },
            {
                "egg_duck_trail",
                "🦆 鸭子的足迹",
                "你发现了一串鸭子的脚印，沿着湖岸一路延伸...",
                [](Stats& stats, EmotionalState& emotion) {
                    stats.modifyHappiness(5);
                    emotion.addMemory("🦆 跟着鸭子的足迹走了一圈校园");
                }
            }
        };
    }
    
    bool findEgg(const std::string& id, Stats& stats, EmotionalState& emotion) {
        for (auto& egg : eggs) {
            if (egg.id == id && !egg.found) {
                egg.found = true;
                foundCount++;
                std::cout << "🥚 发现彩蛋: " << egg.name << "\n";
                egg.effect(stats, emotion);
                return true;
            }
        }
        return false;
    }
    
    bool findRandomEgg(Stats& stats, EmotionalState& emotion) {
        std::vector<EasterEgg*> unfound;
        for (auto& egg : eggs) {
            if (!egg.found) unfound.push_back(&egg);
        }
        if (unfound.empty()) return false;
        
        std::uniform_int_distribution<int> dist(0, unfound.size() - 1);
        std::mt19937 rng(std::time(nullptr));
        auto& egg = unfound[dist(rng)];
        return findEgg(egg->id, stats, emotion);
    }
    
    int getFoundCount() const { return foundCount; }
    int getTotalCount() const { return eggs.size(); }
    
    json toJson() const {
        json j;
        for (const auto& e : eggs) {
            j[e.id] = e.found;
        }
        return j;
    }
    
    void fromJson(const json& j) {
        foundCount = 0;
        for (auto& e : eggs) {
            if (j.contains(e.id)) {
                e.found = j[e.id].get<bool>();
                if (e.found) foundCount++;
            }
        }
    }
};

// ============================================================
// 3. 情感状态系统
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
    int emotionalStability = 70;
    std::vector<std::string> memory;

public:
    void setEmotion(Emotion e) {
        currentEmotion = e;
        addMemory("心情变为: " + getEmoji(e));
    }
    
    Emotion getEmotion() const { return currentEmotion; }
    int getLoneliness() const { return loneliness; }
    int getStability() const { return emotionalStability; }

    void modifyLoneliness(int delta) {
        loneliness = std::clamp(loneliness + delta, 0, 100);
        if (loneliness > 70) setEmotion(Emotion::Lonely);
    }

    void modifyStability(int delta) {
        emotionalStability = std::clamp(emotionalStability + delta, 0, 100);
        if (emotionalStability < 30) setEmotion(Emotion::Anxious);
    }

    void addMemory(const std::string& event) {
        memory.push_back(event);
        if (memory.size() > 50) memory.erase(memory.begin());
    }

    std::vector<std::string> getMemory() const { return memory; }

    json toJson() const {
        return {
            {"emotion", static_cast<int>(currentEmotion)},
            {"loneliness", loneliness},
            {"stability", emotionalStability},
            {"memory", memory}
        };
    }

    void fromJson(const json& j) {
        currentEmotion = static_cast<Emotion>(j.value("emotion", 0));
        loneliness = j.value("loneliness", 30);
        stability = j.value("stability", 70);
        memory = j.value("memory", std::vector<std::string>());
    }
};

// ============================================================
// 4. 恋爱系统
// ============================================================
enum class RelationshipStatus {
    Stranger, Acquaintance, Friend, CloseFriend, Crush, Dating, InLove, Broken
};

class Relationship {
private:
    std::string partnerName;
    RelationshipStatus status = RelationshipStatus::Stranger;
    int intimacy = 0;
    int affection = 0;
    std::vector<std::string> memories;
    bool isRomantic = false;

public:
    Relationship(const std::string& name) : partnerName(name) {}

    std::string getName() const { return partnerName; }
    RelationshipStatus getStatus() const { return status; }
    int getIntimacy() const { return intimacy; }
    int getAffection() const { return affection; }
    bool getIsRomantic() const { return isRomantic; }

    std::string getStatusText() const {
        switch(status) {
            case RelationshipStatus::Stranger: return "陌生人";
            case RelationshipStatus::Acquaintance: return "认识";
            case RelationshipStatus::Friend: return "朋友";
            case RelationshipStatus::CloseFriend: return "好朋友";
            case RelationshipStatus::Crush: return "暗恋";
            case RelationshipStatus::Dating: return "恋爱中 💕";
            case RelationshipStatus::InLove: return "深爱 ❤️";
            case RelationshipStatus::Broken: return "已分手 💔";
            default: return "未知";
        }
    }

    void increaseAffection(int amount) {
        affection = std::min(100, affection + amount);
        updateStatus();
    }

    void decreaseAffection(int amount) {
        affection = std::max(0, affection - amount);
        updateStatus();
    }

    void addMemory(const std::string& event) {
        memories.push_back(event);
        if (memories.size() > 20) memories.erase(memories.begin());
    }

    void updateStatus() {
        if (affection >= 80 && isRomantic) {
            status = RelationshipStatus::InLove;
        } else if (affection >= 60 && isRomantic) {
            status = RelationshipStatus::Dating;
        } else if (affection >= 50) {
            status = RelationshipStatus::Crush;
        } else if (affection >= 40) {
            status = RelationshipStatus::CloseFriend;
        } else if (affection >= 25) {
            status = RelationshipStatus::Friend;
        } else if (affection >= 10) {
            status = RelationshipStatus::Acquaintance;
        } else {
            status = RelationshipStatus::Stranger;
        }
    }

    void confess() {
        if (affection >= 50 && !isRomantic) {
            isRomantic = true;
            status = RelationshipStatus::Dating;
            addMemory("💕 表白成功！开始恋爱");
        } else if (affection < 50) {
            addMemory("💔 表白被拒...");
            affection -= 10;
        }
    }

    void breakUp() {
        isRomantic = false;
        status = RelationshipStatus::Broken;
        affection = std::max(0, affection - 30);
        addMemory("💔 分手了");
    }

    json toJson() const {
        return {
            {"partner", partnerName},
            {"status", static_cast<int>(status)},
            {"intimacy", intimacy},
            {"affection", affection},
            {"romantic", isRomantic},
            {"memories", memories}
        };
    }

    void fromJson(const json& j) {
        partnerName = j.value("partner", "");
        status = static_cast<RelationshipStatus>(j.value("status", 0));
        intimacy = j.value("intimacy", 0);
        affection = j.value("affection", 0);
        isRomantic = j.value("romantic", false);
        memories = j.value("memories", std::vector<std::string>());
    }
};

// ============================================================
// 5. Stats - 核心属性
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
    std::vector<Relationship> relationships;

public:
    Stats() {
        relationships.push_back(Relationship("学姐"));
        relationships.push_back(Relationship("室友"));
        relationships.push_back(Relationship("导师"));
        relationships.push_back(Relationship("鸭哥"));  // v3.2.1 新增
    }

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
    std::vector<Relationship>& getRelationships() { return relationships; }

    void setName(const std::string& n) { name = n; }
    void setGPA(double g) { gpa = std::clamp(g, 0.0, 4.0); }
    void setMoney(int m) { money = std::max(0, m); }

    void modifyEnergy(int delta) { 
        energy = std::clamp(energy + delta, 0, 100);
    }
    
    void modifyHappiness(int delta) { 
        happiness = std::clamp(happiness + delta, 0, 100);
        if (happiness > 80) emotion.setEmotion(Emotion::Happy);
        else if (happiness < 30) emotion.setEmotion(Emotion::Sad);
    }
    
    void modifyMoney(int delta) { money = std::max(0, money + delta); }
    void incrementEventCount() { eventCount++; }
    void nextDay() { dayCount++; }
    void nextSemester() { semester++; }

    void setStoryChoice(const std::string& key, const json& value) {
        storyChoices[key] = value;
    }

    void addBuff(const std::string& name, int days) {
        buffs[name] = days;
    }
    
    bool hasBuff(const std::string& name) const {
        return buffs.find(name) != buffs.end() && buffs.at(name) > 0;
    }
    
    void updateBuffs() {
        for (auto it = buffs.begin(); it != buffs.end();) {
            it->second--;
            if (it->second <= 0) {
                it = buffs.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    int getBuffBonus(const std::string& type) const {
        if (hasBuff("学习效率+20%") && type == "study") return 20;
        if (hasBuff("精力恢复+50%") && type == "rest") return 50;
        return 0;
    }

    Relationship* getRelationship(const std::string& name) {
        for (auto& r : relationships) {
            if (r.getName() == name) return &r;
        }
        return nullptr;
    }

    json toJson() const {
        json relJson;
        for (const auto& r : relationships) {
            relJson.push_back(r.toJson());
        }
        return {
            {"name", name},
            {"age", age},
            {"major", major},
            {"semester", semester},
            {"gpa", gpa},
            {"energy", energy},
            {"happiness", happiness},
            {"money", money},
            {"event_count", eventCount},
            {"day_count", dayCount},
            {"buffs", buffs},
            {"story_choices", storyChoices},
            {"emotion", emotion.toJson()},
            {"relationships", relJson}
        };
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
        if (j.contains("buffs")) {
            buffs = j["buffs"].get<std::map<std::string, int>>();
        }
        if (j.contains("story_choices")) {
            storyChoices = j["story_choices"].get<std::map<std::string, json>>();
        }
        if (j.contains("emotion")) {
            emotion.fromJson(j["emotion"]);
        }
        if (j.contains("relationships")) {
            for (size_t i = 0; i < j["relationships"].size() && i < relationships.size(); i++) {
                relationships[i].fromJson(j["relationships"][i]);
            }
        }
    }
};

// ============================================================
// 6. Course - 课程
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

    json toJson() const {
        return {{"name", name}, {"credit", credit}, {"score", score}, {"grade", grade}, {"passed", isPassed}};
    }
    
    void fromJson(const json& j) {
        name = j.value("name", "");
        credit = j.value("credit", 3);
        score = j.value("score", 0);
        grade = j.value("grade", "F");
        isPassed = j.value("passed", false);
    }
};

// ============================================================
// 7. CourseManager - 课程管理
// ============================================================
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
            {"英语", 2}, {"离散数学", 3}, {"编译原理", 3},
            {"鸭科动物学", 2}  // v3.2.1 新增
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
                
                if (stats.hasBuff("学习效率+20%")) {
                    bonus = static_cast<int>(bonus * 1.2);
                }
                
                if (stats.getEnergy() < 30) {
                    bonus = static_cast<int>(bonus * 0.5);
                    std::cout << "⚠️ 精力不足，学习效果减半！\n";
                }
                
                int finalScore = std::min(baseScore + bonus, 100);
                course.setScore(finalScore);
                
                stats.modifyEnergy(-hours * 2);
                stats.modifyHappiness(3);
                
                std::cout << "📊 " << name << " 成绩: " << finalScore 
                          << " (" << course.getGrade() << ")\n";
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
// 8. SkillManager - 技能系统
// ============================================================
class SkillManager {
private:
    std::vector<std::string> skills;
    std::map<std::string, std::string> skillDescriptions;

public:
    SkillManager() {
        skillDescriptions = {
            {"编程能力", "写代码更快更好"},
            {"数学思维", "逻辑推理能力提升"},
            {"英语应用", "阅读外文资料无障碍"},
            {"数据分析", "处理数据更加高效"},
            {"项目管理", "组织协调能力增强"},
            {"演讲能力", "口才与表达出众"},
            {"团队协作", "善于与他人合作"},
            {"学习基础", "学习效率永久+5%"},
            {"恋爱达人", "恋爱中更容易获得好感"},
            {"鸭语精通", "可以和鸭子交流！"}  // v3.2.1 新增
        };
    }

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

// ============================================================
// 9. RandomEvent - 随机事件
// ============================================================
struct RandomEvent {
    std::string title;
    std::string description;
    std::function<void(Stats&, CourseManager&, SkillManager&)> effect;
    int weight;
};

class EventSystem {
private:
    std::vector<RandomEvent> events;
    std::mt19937 rng;

public:
    EventSystem() {
        rng.seed(std::time(nullptr));
        
        events = {
            {
                "📚 捡到笔记",
                "你在图书馆捡到一本高数笔记，学习效率提升！",
                [](Stats& stats, CourseManager& cm, SkillManager& sm) {
                    stats.addBuff("学习效率+20%", 3);
                    stats.modifyHappiness(5);
                    stats.incrementEventCount();
                },
                15
            },
            {
                "🎮 室友通宵",
                "室友通宵打游戏，你的睡眠被打扰了！",
                [](Stats& stats, CourseManager& cm, SkillManager& sm) {
                    stats.modifyEnergy(-15);
                    stats.modifyHappiness(-5);
                    stats.getEmotion().modifyLoneliness(10);
                    stats.incrementEventCount();
                },
                10
            },
            {
                "💼 找到兼职",
                "你找到了一份不错的兼职，赚了一些零花钱！",
                [](Stats& stats, CourseManager& cm, SkillManager& sm) {
                    stats.modifyMoney(300);
                    stats.modifyHappiness(10);
                    stats.incrementEventCount();
                },
                12
            },
            {
                "🏆 竞赛获奖",
                "你参加了编程竞赛并获奖！",
                [](Stats& stats, CourseManager& cm, SkillManager& sm) {
                    stats.modifyHappiness(20);
                    sm.unlockSkill("编程能力");
                    stats.modifyMoney(500);
                    stats.incrementEventCount();
                },
                5
            },
            {
                "🦆 鸭子军团",  // v3.2.1 新增
                "一群鸭子突然出现在你面前，它们似乎在向你示意什么...",
                [](Stats& stats, CourseManager& cm, SkillManager& sm) {
                    stats.modifyHappiness(15);
                    stats.getEmotion().modifyLoneliness(-10);
                    if (!sm.hasSkill("鸭语精通")) {
                        sm.unlockSkill("鸭语精通");
                        std::cout << "🦆 你好像能听懂鸭子在说什么了！\n";
                    }
                    stats.incrementEventCount();
                },
                4
            },
            {
                "🥚 神秘彩蛋",  // v3.2.1 新增
                "你在草丛里发现了一个彩蛋！",
                [](Stats& stats, CourseManager& cm, SkillManager& sm) {
                    stats.modifyHappiness(10);
                    stats.getEmotion().setEmotion(Emotion::Excited);
                    stats.incrementEventCount();
                },
                3
            }
        };
    }

    bool triggerRandomEvent(Stats& stats, CourseManager& cm, SkillManager& sm) {
        int totalWeight = 0;
        for (const auto& e : events) totalWeight += e.weight;
        
        std::uniform_int_distribution<int> dist(0, totalWeight - 1);
        int roll = dist(rng);
        
        int cumulative = 0;
        for (const auto& e : events) {
            cumulative += e.weight;
            if (roll < cumulative) {
                std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                std::cout << "🎲 " << e.title << "\n";
                std::cout << "📖 " << e.description << "\n";
                std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                e.effect(stats, cm, sm);
                return true;
            }
        }
        return false;
    }
};

// ============================================================
// 10. NPC - NPC系统 (v3.2.1 新增鸭哥)
// ============================================================
class NPC {
private:
    std::string name;
    int favorability = 0;
    std::vector<std::string> dialogues;
    std::map<int, std::pair<std::string, std::function<void(Stats&, CourseManager&)>>> rewards;
    std::mt19937 rng;
    int interactionCount = 0;
    std::vector<std::string> specialDialogues;

public:
    NPC(const std::string& n) : name(n) {
        rng.seed(std::time(nullptr) + std::hash<std::string>{}(n));
        initDialogues();
    }

    void initDialogues() {
        if (name == "鸭哥") {  // v3.2.1 新增
            dialogues = {
                "🦆 嘎？",
                "🦆 嘎嘎！",
                "🦆 嘎嘎嘎~",
                "🦆 你看起来是个好人。",
                "🦆 湖里的鸭子们都在等你。",
                "🦆 你有看到《鸭科动物大全》吗？",
                "🦆 找到所有鸭子，会有惊喜哦！"
            };
            specialDialogues = {
                "🦆 嘎！你找到了我！",
                "🦆 你是个真正的鸭子守护者！",
                "🦆 谢谢你照顾我的朋友们。"
            };
            rewards[30] = {"🦆 鸭子的秘密", [](Stats& stats, CourseManager& cm) {
                stats.modifyHappiness(10);
                stats.getEmotion().modifyLoneliness(-10);
                stats.addBuff("鸭子的祝福", 3);
                std::cout << "🦆 你获得了鸭子的祝福！\n";
            }};
            rewards[60] = {"📖 鸭子的智慧", [](Stats& stats, CourseManager& cm) {
                stats.modifyHappiness(15);
                stats.modifyEnergy(10);
                std::cout << "🦆 你领悟了鸭子的智慧！\n";
            }};
            rewards[80] = {"👑 鸭子之王", [](Stats& stats, CourseManager& cm) {
                stats.modifyHappiness(30);
                stats.getEmotion().setEmotion(Emotion::Loved);
                std::cout << "👑 你成为了鸭子们的王！\n";
            }};
        } else if (name == "学姐") {
            dialogues = {
                "你好呀！需要帮忙吗？",
                "学习要劳逸结合哦！",
                "这学期有什么目标吗？",
                "加油，我看好你！",
                "要不要一起去图书馆？",
                "我这里有份复习资料！"
            };
            specialDialogues = {
                "💕 你知道吗？和你聊天总是很开心。",
                "📖 要不要一起去看书？",
                "☕ 我请你喝咖啡吧！"
            };
            rewards[30] = {"📖 复习资料", [](Stats& stats, CourseManager& cm) {
                stats.addBuff("学习效率+20%", 2);
                stats.modifyHappiness(5);
                stats.getEmotion().modifyLoneliness(-5);
            }};
            rewards[60] = {"📝 考试重点", [](Stats& stats, CourseManager& cm) {}};
            rewards[80] = {"🎓 推荐信", [](Stats& stats, CourseManager& cm) {
                stats.modifyHappiness(20);
                stats.getEmotion().modifyLoneliness(-10);
            }};
        } else if (name == "室友") {
            dialogues = {
                "打游戏吗？",
                "食堂新开了家店，一起去？",
                "晚上熬夜复习？我陪你！",
                "兄弟，该交作业了！"
            };
            specialDialogues = {
                "🎮 今晚通宵吗？",
                "🍜 我请客吃饭！",
                "💪 一起去健身房吧！"
            };
            rewards[30] = {"🍜 一起吃饭", [](Stats& stats, CourseManager& cm) {
                stats.modifyEnergy(10);
                stats.modifyHappiness(15);
                stats.getEmotion().modifyLoneliness(-10);
            }};
            rewards[60] = {"📚 组队学习", [](Stats& stats, CourseManager& cm) {
                stats.modifyEnergy(5);
                stats.modifyHappiness(10);
                stats.addBuff("学习效率+20%", 1);
                stats.getEmotion().modifyLoneliness(-5);
            }};
            rewards[80] = {"💪 互相监督", [](Stats& stats, CourseManager& cm) {
                stats.addBuff("学习效率+20%", 3);
                stats.modifyHappiness(10);
                stats.getEmotion().modifyLoneliness(-15);
            }};
        } else if (name == "导师") {
            dialogues = {
                "最近学习怎么样？",
                "有什么问题随时问我。",
                "你的潜力很大！",
                "毕业后有什么打算？"
            };
            specialDialogues = {
                "💡 我这里有份研究机会，有兴趣吗？",
                "📝 你的论文写得不错。",
                "🎯 我觉得你可以考虑深造。"
            };
            rewards[30] = {"💡 学术指导", [](Stats& stats, CourseManager& cm) {
                stats.modifyHappiness(10);
                stats.getEmotion().modifyLoneliness(-5);
            }};
            rewards[60] = {"📜 研究机会", [](Stats& stats, CourseManager& cm) {
                stats.modifyHappiness(15);
                stats.modifyMoney(200);
                stats.getEmotion().modifyLoneliness(-10);
            }};
            rewards[80] = {"🎓 推荐信", [](Stats& stats, CourseManager& cm) {
                stats.modifyHappiness(20);
                stats.getEmotion().modifyLoneliness(-15);
                std::cout << "🎓 获得导师强力推荐信！\n";
            }};
        }
    }

    std::string getName() const { return name; }
    int getFavorability() const { return favorability; }
    int getInteractionCount() const { return interactionCount; }

    void interact(Stats& stats, CourseManager& cm) {
        interactionCount++;
        
        auto* rel = stats.getRelationship(name);
        bool isLoving = rel && (rel->getStatus() == RelationshipStatus::Dating || 
                                rel->getStatus() == RelationshipStatus::InLove);
        
        std::uniform_int_distribution<int> dist(0, 
            isLoving ? specialDialogues.size() + dialogues.size() - 1 : dialogues.size() - 1);
        int idx = dist(rng);
        
        std::string dialogue;
        if (isLoving && idx >= dialogues.size()) {
            dialogue = specialDialogues[idx - dialogues.size()];
        } else {
            dialogue = dialogues[idx % dialogues.size()];
        }
        
        std::cout << "💬 " << name << ": \"" << dialogue << "\"\n";
        
        int increase = 5 + (std::rand() % 10);
        if (isLoving) increase += 5;
        favorability = std::min(100, favorability + increase);
        
        if (rel) {
            rel->increaseAffection(increase);
            if (!rel->getIsRomantic() && favorability >= 50 && name != "鸭哥") {
                rel->confess();
            }
            rel->addMemory(dialogue);
        }
        
        std::cout << "❤️ " << name << " 好感度 +" << increase << " (当前: " << favorability << ")\n";
        
        for (const auto& [threshold, reward] : rewards) {
            if (favorability >= threshold && favorability - increase < threshold) {
                std::cout << "🎁 解锁奖励: " << reward.first << "\n";
                reward.second(stats, cm);
            }
        }
        
        stats.modifyHappiness(3);
        stats.getEmotion().modifyLoneliness(-3);
    }

    void interactSpecial(Stats& stats, CourseManager& cm) {
        auto* rel = stats.getRelationship(name);
        if (rel && favorability >= 50 && name != "鸭哥") {
            rel->confess();
            if (rel->getIsRomantic()) {
                stats.getEmotion().setEmotion(Emotion::Loved);
                stats.modifyHappiness(20);
                std::cout << "💕 你们开始恋爱了！\n";
            }
        } else if (name == "鸭哥") {
            std::cout << "🦆 鸭哥：嘎嘎嘎嘎嘎嘎！（翻译：你是个好人）\n";
            stats.modifyHappiness(5);
        } else {
            std::cout << "💔 好感度不够，表白失败了...\n";
            stats.modifyHappiness(-10);
        }
    }

    json toJson() const {
        return {{"name", name}, {"favorability", favorability}, {"interactions", interactionCount}};
    }
    
    void fromJson(const json& j) {
        name = j.value("name", "");
        favorability = j.value("favorability", 0);
        interactionCount = j.value("interactions", 0);
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
        npcs.push_back(NPC("鸭哥"));  // v3.2.1 新增
    }

    std::vector<NPC>& getNPCs() { return npcs; }

    void interactWithNPC(int index, Stats& stats, CourseManager& cm) {
        if (index >= 0 && index < npcs.size()) {
            npcs[index].interact(stats, cm);
        }
    }

    void interactSpecial(int index, Stats& stats, CourseManager& cm) {
        if (index >= 0 && index < npcs.size()) {
            npcs[index].interactSpecial(stats, cm);
        }
    }

    json toJson() const {
        json j;
        for (const auto& npc : npcs) j.push_back(npc.toJson());
        return j;
    }
    
    void fromJson(const json& j) {
        for (size_t i = 0; i < j.size() && i < npcs.size(); i++) {
            npcs[i].fromJson(j[i]);
        }
    }
};

// ============================================================
// 11. Achievement - 成就系统 (v3.2.1 新增)
// ============================================================
struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    std::string icon;
    bool unlocked = false;
    std::function<bool(const json&)> condition;

    Achievement(const std::string& i, const std::string& n, 
                const std::string& d, const std::string& ic,
                std::function<bool(const json&)> cond)
        : id(i), name(n), description(d), icon(ic), condition(cond) {}
};

class AchievementManager {
private:
    std::vector<Achievement> achievements;
    std::vector<std::string> unlockedIds;
    std::mt19937 rng;

public:
    AchievementManager() {
        rng.seed(std::time(nullptr));
        initAchievements();
    }

    void initAchievements() {
        achievements = {
            {"first_course", "📚 初入学术", "选修第一门课程", "📚", 
             [](const json& data) { return data["courses"].size() >= 1; }},
            
            {"study_hard", "💪 学霸之路", "一门课程考到90分以上", "💪", 
             [](const json& data) { 
                 for (auto& c : data["courses"]) {
                     if (c["score"].get<int>() >= 90) return true;
                 }
                 return false;
             }},
            
            {"all_courses", "🎓 课程大师", "完成所有课程", "🎓", 
             [](const json& data) { return data["courses"].size() >= 10; }},
            
            {"gpa_35", "🌟 优秀毕业生", "GPA达到3.5以上", "🌟", 
             [](const json& data) { return data["gpa"].get<double>() >= 3.5; }},
            
            {"social_butterfly", "🦋 社交达人", "所有NPC好感度达到60以上", "🦋", 
             [](const json& data) { 
                 for (auto& npc : data["npcs"]) {
                     if (npc["favorability"].get<int>() < 60) return false;
                 }
                 return true;
             }},
            
            {"money_maker", "💰 小富翁", "攒够2000元", "💰", 
             [](const json& data) { return data["money"].get<int>() >= 2000; }},
            
            {"event_collector", "🎲 事件收藏家", "触发10次随机事件", "🎲", 
             [](const json& data) { return data["event_count"].get<int>() >= 10; }},
            
            {"graduate_early", "⚡ 提前毕业", "6个学期内毕业", "⚡", 
             [](const json& data) { 
                 return data["graduated"].get<bool>() && data["semester"].get<int>() <= 6;
             }},
            
            {"perfect_score", "🎯 完美成绩", "所有课程都在85分以上", "🎯", 
             [](const json& data) { 
                 for (auto& c : data["courses"]) {
                     if (c["score"].get<int>() < 85) return false;
                 }
                 return true;
             }},
            
            {"first_love", "💕 初恋", "开始一段恋爱关系", "💕",
             [](const json& data) { 
                 for (auto& rel : data["relationships"]) {
                     if (rel["romantic"].get<bool>()) return true;
                 }
                 return false;
             }},
            
            {"heart_breaker", "💔 心碎", "经历一次分手", "💔",
             [](const json& data) { 
                 for (auto& rel : data["relationships"]) {
                     if (rel["status"].get<int>() == 7) return true;
                 }
                 return false;
             }},
            
            {"explorer", "🔍 探索者", "发现3个校园秘密", "🔍",
             [](const json& data) { return data["discoveries"].get<int>() >= 3; }},
            
            {"explorer_master", "🗺️ 探索大师", "发现所有校园秘密", "🗺️",
             [](const json& data) { return data["discoveries_total"].get<int>() >= 6; }},
            
            {"romantic", "🌹 浪漫主义者", "同时与2个NPC保持恋爱关系", "🌹",
             [](const json& data) { 
                 int count = 0;
                 for (auto& rel : data["relationships"]) {
                     if (rel["romantic"].get<bool>()) count++;
                 }
                 return count >= 2;
             }},
            
            // v3.2.1 新增成就
            {"duck_finder", "🦆 鸭子守护者", "找到一只鸭子", "🦆",
             [](const json& data) { return data["ducks_found"].get<int>() >= 1; }},
            
            {"duck_master", "👑 鸭子之王", "找到所有鸭子", "👑",
             [](const json& data) { return data["ducks_found"].get<int>() >= 10; }},
            
            {"duck_whisperer", "🦆 鸭语者", "解锁'鸭语精通'技能", "🦆",
             [](const json& data) { 
                 for (auto& s : data["skills"]) {
                     if (s == "鸭语精通") return true;
                 }
                 return false;
             }},
            
            {"egg_hunter", "🥚 彩蛋猎人", "找到一个彩蛋", "🥚",
             [](const json& data) { return data["eggs_found"].get<int>() >= 1; }}
        };
    }

    void checkAchievements(const json& data, std::vector<std::string>& newUnlocks) {
        for (auto& achievement : achievements) {
            if (!achievement.unlocked && achievement.condition(data)) {
                achievement.unlocked = true;
                unlockedIds.push_back(achievement.id);
                newUnlocks.push_back(achievement.icon + " " + achievement.name + " - " + achievement.description);
            }
        }
    }

    std::vector<Achievement> getAchievements() const { return achievements; }
    int getUnlockedCount() const { return unlockedIds.size(); }
    int getTotalCount() const { return achievements.size(); }

    json toJson() const {
        json j;
        for (const auto& a : achievements) {
            j[a.id] = a.unlocked;
        }
        return j;
    }

    void fromJson(const json& j) {
        unlockedIds.clear();
        for (auto& achievement : achievements) {
            if (j.contains(achievement.id)) {
                achievement.unlocked = j[achievement.id].get<bool>();
                if (achievement.unlocked) {
                    unlockedIds.push_back(achievement.id);
                }
            }
        }
    }
};

// ============================================================
// 12. StorySystem - 故事系统 (v3.2.1 新增鸭子故事)
// ============================================================
struct StoryEvent {
    int semester;
    std::string title;
    std::string description;
    std::vector<std::pair<std::string, std::function<void(json&, Stats&)>>> choices;
    bool triggered = false;
    std::string branch;
};

class StorySystem {
private:
    std::vector<StoryEvent> storyEvents;
    std::vector<std::string> storyLog;
    json storyData;

public:
    StorySystem() {
        initStory();
    }

    void initStory() {
        storyEvents = {
            {1, "🎓 大学新生", 
             "你怀着激动的心情踏入大学校园。一切都是那么新鲜！\n"
             "你决定先做什么？",
             {
                 {"📚 去图书馆看看", [](json& data, Stats& stats) { 
                     data["story_choices"]["first"] = "library";
                     stats.modifyHappiness(5);
                     stats.getEmotion().modifyLoneliness(-5);
                     stats.addBuff("学习效率+20%", 1);
                 }},
                 {"🏠 去宿舍安顿", [](json& data, Stats& stats) { 
                     data["story_choices"]["first"] = "dorm";
                     stats.modifyEnergy(10);
                     stats.getEmotion().modifyLoneliness(-10);
                 }},
                 {"🦆 去湖边看鸭子", [](json& data, Stats& stats) {  // v3.2.1 新增选项
                     data["story_choices"]["first"] = "lake";
                     stats.modifyHappiness(15);
                     stats.getEmotion().modifyLoneliness(-15);
                     stats.getEmotion().addMemory("🦆 第一天就去湖边看了鸭子");
                     std::cout << "🦆 你在湖边看到了一群鸭子，心情变好了！\n";
                 }}
             },
             "first_choice"
            },
            {2, "📖 奇妙的发现", 
             "你在校园里漫无目的地走着，突然看到了什么...\n"
             "那是什么？",
             {
                 {"📚 去图书馆看看", [](json& data, Stats& stats) { 
                     data["story_choices"]["second"] = "library";
                     stats.modifyHappiness(5);
                     stats.getEmotion().modifyLoneliness(-5);
                 }},
                 {"🦆 跟踪一只鸭子", [](json& data, Stats& stats) {  // v3.2.1 新增
                     data["story_choices"]["second"] = "duck_trail";
                     stats.modifyHappiness(10);
                     stats.getEmotion().addMemory("🦆 跟踪一只鸭子，发现了一条秘密小路");
                     std::cout << "🦆 你跟着鸭子发现了一条通往湖心的小路！\n";
                 }}
             },
             "second_choice"
            },
            {3, "📖 《鸭科动物大全》",  // v3.2.1 新增故事
             "你在图书馆的角落里发现了一本神秘书籍...\n"
             "书名是《鸭科动物大全》！",
             {
                 {"📖 认真阅读", [](json& data, Stats& stats) { 
                     data["story_choices"]["duck_book"] = "read";
                     stats.modifyHappiness(15);
                     stats.getEmotion().addMemory("📖 阅读了《鸭科动物大全》");
                     stats.addBuff("鸭子的智慧", 2);
                     std::cout << "📖 你学到了很多关于鸭子的知识！\n";
                 }},
                 {"🦆 带着书去找鸭哥", [](json& data, Stats& stats) { 
                     data["story_choices"]["duck_book"] = "duck_ge";
                     stats.modifyHappiness(20);
                     stats.getEmotion().setEmotion(Emotion::Excited);
                     std::cout << "🦆 你带着书去找鸭哥，鸭哥非常高兴！\n";
                 }}
             },
             "duck_book_story"
            }
        };
    }

    std::vector<StoryEvent> getEventsForSemester(int semester) {
        std::vector<StoryEvent> result;
        for (auto& event : storyEvents) {
            if (event.semester == semester && !event.triggered) {
                result.push_back(event);
            }
        }
        return result;
    }

    void markTriggered(int semester) {
        for (auto& event : storyEvents) {
            if (event.semester == semester) {
                event.triggered = true;
            }
        }
    }

    void addLog(const std::string& entry) {
        storyLog.push_back(entry);
        if (storyLog.size() > 50) storyLog.erase(storyLog.begin());
    }

    std::vector<std::string> getLog() const { return storyLog; }

    json toJson() const {
        json j;
        for (const auto& e : storyEvents) {
            j[std::to_string(e.semester) + "_" + e.branch] = e.triggered;
        }
        j["log"] = storyLog;
        j["data"] = storyData;
        return j;
    }

    void fromJson(const json& j) {
        for (auto& e : storyEvents) {
            std::string key = std::to_string(e.semester) + "_" + e.branch;
            if (j.contains(key)) {
                e.triggered = j[key].get<bool>();
            }
        }
        if (j.contains("log")) {
            storyLog = j["log"].get<std::vector<std::string>>();
        }
        if (j.contains("data")) {
            storyData = j["data"];
        }
    }
};

// ============================================================
// 13. Interactable - 建筑交互 (v3.2.1 新增鸭子发现)
// ============================================================
class Interactable {
public:
    virtual ~Interactable() = default;
    virtual std::string getName() const = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual void interact(Stats& stats, CourseManager& cm, 
                         DuckManager& ducks, EasterEggManager& eggs) = 0;
    virtual sf::Color getColor() const = 0;
    virtual std::string getEmoji() const = 0;
};

class Lake : public Interactable {  // v3.2.1 新增
public:
    std::string getName() const override { return "校园湖"; }
    sf::FloatRect getBounds() const override { return {320, 300, 200, 150}; }
    sf::Color getColor() const override { return sf::Color(30, 144, 255); }
    std::string getEmoji() const override { return "🌊"; }
    
    void interact(Stats& stats, CourseManager& cm, 
                  DuckManager& ducks, EasterEggManager& eggs) override {
        std::cout << "🌊 你来到了校园湖边...\n";
        
        // 发现鸭子
        if (ducks.findDuckByPosition(400, 350)) {
            stats.modifyHappiness(5);
            stats.getEmotion().modifyLoneliness(-5);
        }
        
        // 随机发现鸭子
        std::vector<std::string> duckIds = {
            "duck_001", "duck_002", "duck_003", "duck_006", "duck_008", "duck_009"
        };
        std::uniform_int_distribution<int> dist(0, duckIds.size() - 1);
        std::mt19937 rng(std::time(nullptr));
        if (std::rand() % 100 < 30) {
            ducks.findDuck(duckIds[dist(rng)]);
        }
        
        stats.nextDay();
        stats.getEmotion().modifyLoneliness(-8);
        stats.modifyHappiness(3);
    }
};

class Dormitory : public Interactable {
public:
    std::string getName() const override { return "宿舍"; }
    sf::FloatRect getBounds() const override { return {100, 500, 80, 60}; }
    sf::Color getColor() const override { return sf::Color(139, 69, 19); }
    std::string getEmoji() const override { return "🏠"; }
    
    void interact(Stats& stats, CourseManager& cm, 
                  DuckManager& ducks, EasterEggManager& eggs) override {
        int bonus = stats.hasBuff("精力恢复+50%") ? 60 : 40;
        stats.modifyEnergy(bonus);
        stats.modifyHappiness(5);
        stats.nextDay();
        stats.getEmotion().modifyLoneliness(-5);
        std::cout << "😴 睡了一觉，精力 +" << bonus << "！\n";
        
        if (std::rand() % 100 < 10) {
            ducks.findDuck("duck_007");
        }
    }
};

class Library : public Interactable {
public:
    std::string getName() const override { return "图书馆"; }
    sf::FloatRect getBounds() const override { return {250, 500, 80, 60}; }
    sf::Color getColor() const override { return sf::Color(70, 130, 180); }
    std::string getEmoji() const override { return "📚"; }
    
    void interact(Stats& stats, CourseManager& cm, 
                  DuckManager& ducks, EasterEggManager& eggs) override {
        auto courses = cm.getCourses();
        if (!courses.empty()) {
            std::cout << "📖 在图书馆学习...\n";
            cm.studyForExam(courses[0].getName(), 2, stats);
            stats.nextDay();
            stats.getEmotion().modifyLoneliness(-3);
            
            // 发现《鸭科动物大全》彩蛋
            if (std::rand() % 100 < 20) {
                eggs.findEgg("egg_duck_book", stats, stats.getEmotion());
            }
            
            if (std::rand() % 100 < 15) {
                ducks.findDuck("duck_004");
            }
        } else {
            std::cout << "⚠️ 请先选修课程！\n";
        }
    }
};

class Classroom : public Interactable {
public:
    std::string getName() const override { return "教室"; }
    sf::FloatRect getBounds() const override { return {400, 500, 80, 60}; }
    sf::Color getColor() const override { return sf::Color(255, 140, 0); }
    std::string getEmoji() const override { return "🏫"; }
    
    void interact(Stats& stats, CourseManager& cm, 
                  DuckManager& ducks, EasterEggManager& eggs) override {
        if (!cm.getCourses().empty()) {
            std::cout << "📝 参加考试周...\n";
            cm.examWeek(stats);
            stats.nextDay();
            stats.getEmotion().modifyStability(-5);
        } else {
            std::cout << "⚠️ 请先选修课程！\n";
        }
    }
};

class Canteen : public Interactable {
public:
    std::string getName() const override { return "食堂"; }
    sf::FloatRect getBounds() const override { return {550, 500, 80, 60}; }
    sf::Color getColor() const override { return sf::Color(255, 69, 0); }
    std::string getEmoji() const override { return "🍜"; }
    
    void interact(Stats& stats, CourseManager& cm, 
                  DuckManager& ducks, EasterEggManager& eggs) override {
        stats.modifyEnergy(20);
        stats.modifyHappiness(8);
        stats.modifyMoney(-20);
        stats.nextDay();
        stats.getEmotion().modifyLoneliness(-8);
        std::cout << "🍜 吃了顿饭，精力 +20，快乐 +8，花费 20元\n";
        
        if (std::rand() % 100 < 10) {
            ducks.findDuck("duck_005");
        }
    }
};

class Gym : public Interactable {
public:
    std::string getName() const override { return "健身房"; }
    sf::FloatRect getBounds() const override { return {700, 500, 80, 60}; }
    sf::Color getColor() const override { return sf::Color(50, 205, 50); }
    std::string getEmoji() const override { return "💪"; }
    
    void interact(Stats& stats, CourseManager& cm, 
                  DuckManager& ducks, EasterEggManager& eggs) override {
        stats.modifyEnergy(15);
        stats.modifyHappiness(10);
        stats.nextDay();
        stats.getEmotion().modifyLoneliness(-5);
        std::cout << "💪 运动了一下，精力 +15，快乐 +10\n";
        
        if (std::rand() % 100 < 10) {
            ducks.findDuck("duck_008");
        }
    }
};

// ============================================================
// 14. SaveManager - JSON存档 (v3.2.1 扩展)
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

    void setSlot(const std::string& slot) { currentSlot = slot; }

    bool save(const Stats& stats, const CourseManager& cm, 
              const SkillManager& sm, const NPCManager& npcm,
              const AchievementManager& am, const StorySystem& story,
              const DuckManager& ducks, const EasterEggManager& eggs) {
        json data = {
            {"stats", stats.toJson()},
            {"courses", cm.toJson()},
            {"skills", sm.toJson()},
            {"npcs", npcm.toJson()},
            {"achievements", am.toJson()},
            {"story", story.toJson()},
            {"ducks", ducks.toJson()},
            {"eggs", eggs.toJson()},
            {"save_time", std::time(nullptr)},
            {"version", "3.2.1"}
        };
        
        std::string filename = saveDirectory + currentSlot + ".json";
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << data.dump(4);
        file.close();
        std::cout << "💾 存档成功: " << filename << "\n";
        return true;
    }

    bool load(Stats& stats, CourseManager& cm, 
              SkillManager& sm, NPCManager& npcm,
              AchievementManager& am, StorySystem& story,
              DuckManager& ducks, EasterEggManager& eggs) {
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
        if (data.contains("story")) story.fromJson(data["story"]);
        if (data.contains("ducks")) ducks.fromJson(data["ducks"]);
        if (data.contains("eggs")) eggs.fromJson(data["eggs"]);
        
        std::cout << "💾 读档成功: " << filename << "\n";
        return true;
    }

    std::vector<std::string> listSaves() {
        std::vector<std::string> saves;
        for (const auto& entry : fs::directory_iterator(saveDirectory)) {
            if (entry.path().extension() == ".json") {
                saves.push_back(entry.path().stem().string());
            }
        }
        return saves;
    }
};

// ============================================================
// 15. GameManager - 游戏管理器 (v3.2.1)
// ============================================================
class GameManager {
private:
    sf::RenderWindow window;
    
    Stats stats;
    CourseManager courseManager;
    SkillManager skillManager;
    NPCManager npcManager;
    EventSystem eventSystem;
    AchievementManager achievementManager;
    StorySystem storySystem;
    DuckManager duckManager;
    EasterEggManager eggManager;
    SaveManager saveManager;
    
    std::vector<std::unique_ptr<Interactable>> buildings;
    std::vector<std::string> messages;
    std::vector<std::string> pendingAchievements;
    
    bool isRunning = true;
    bool isInputting = false;
    bool inStoryMode = false;
    int storyChoiceIndex = 0;
    StoryEvent currentStoryEvent;
    std::string inputBuffer;
    std::string inputPrompt;
    
    sf::Font font;
    sf::CircleShape playerIcon;
    sf::RectangleShape statusPanel;
    sf::RectangleShape menuPanel;
    sf::RectangleShape messagePanel;
    
    // v3.2.1 新增：鸭子图标
    std::vector<sf::CircleShape> duckIcons;

public:
    GameManager() {
        window.create(sf::VideoMode(1024, 768), "🎓 学生生活模拟器 v3.2.1 🦆", sf::Style::Close);
        window.setFramerateLimit(60);
        
        if (!loadFont()) {
            std::cerr << "⚠️ 字体加载失败\n";
        }
        
        // v3.2.1: 添加湖和鸭子
        buildings.push_back(std::make_unique<Lake>());
        buildings.push_back(std::make_unique<Dormitory>());
        buildings.push_back(std::make_unique<Library>());
        buildings.push_back(std::make_unique<Classroom>());
        buildings.push_back(std::make_unique<Canteen>());
        buildings.push_back(std::make_unique<Gym>());
        
        setupUI();
        setupDuckIcons();
        
        playerIcon.setRadius(15);
        playerIcon.setFillColor(sf::Color::Cyan);
        playerIcon.setPosition(400, 300);
        playerIcon.setOutlineColor(sf::Color::White);
        playerIcon.setOutlineThickness(2);
        
        messages.push_back("🎓 学生生活模拟器 v3.2.1 🦆");
        messages.push_back("🦆 新功能: 校园湖 | 鸭子收集 | 鸭哥NPC | 彩蛋系统");
        messages.push_back("🏠 方向键移动 | Enter交互 | 1-9菜单");
        messages.push_back("💾 S存档 | L读档 | R改名 | E事件 | A成就 | F恋情");
    }

    bool loadFont() {
        std::vector<std::string> fontPaths = {
            "C:\\Windows\\Fonts\\msyh.ttc",
            "C:\\Windows\\Fonts\\simhei.ttf",
            "C:\\Windows\\Fonts\\arial.ttf",
            "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
            "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc",
            "/System/Library/Fonts/PingFang.ttc"
        };
        
        for (const auto& path : fontPaths) {
            if (fs::exists(path) && font.loadFromFile(path)) {
                return true;
            }
        }
        return font.loadFromFile("");
    }

    void setupUI() {
        statusPanel.setSize({300, 270});
        statusPanel.setPosition({10, 10});
        statusPanel.setFillColor({50, 50, 50, 230});
        statusPanel.setOutlineColor(sf::Color::White);
        statusPanel.setOutlineThickness(1);
        
        menuPanel.setSize({300, 380});
        menuPanel.setPosition({10, 290});
        menuPanel.setFillColor({50, 50, 50, 230});
        menuPanel.setOutlineColor(sf::Color::White);
        menuPanel.setOutlineThickness(1);
        
        messagePanel.setSize({700, 150});
        messagePanel.setPosition({320, 600});
        messagePanel.setFillColor({50, 50, 50, 230});
        messagePanel.setOutlineColor(sf::Color::White);
        messagePanel.setOutlineThickness(1);
    }

    void setupDuckIcons() {
        auto ducks = duckManager.getDucks();
        for (const auto& d : ducks) {
            sf::CircleShape duck(8);
            duck.setFillColor(sf::Color::Yellow);
            duck.setOutlineColor(sf::Color::Orange);
            duck.setOutlineThickness(1);
            duck.setPosition(d.x - 8, d.y - 8);
            duckIcons.push_back(duck);
        }
    }

    void run() {
        auto saves = saveManager.listSaves();
        if (!saves.empty()) {
            std::cout << "📂 检测到存档\n";
            std::cout << "是否加载存档? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                if (saveManager.load(stats, courseManager, skillManager, 
                                     npcManager, achievementManager, storySystem,
                                     duckManager, eggManager)) {
                    std::cout << "✅ 加载成功！\n";
                    checkAchievements();
                }
            }
        }
        
        if (stats.getName().empty()) {
            createCharacter();
        }
        
        while (window.isOpen() && isRunning) {
            handleEvents();
            update();
            render();
        }
    }

    void createCharacter() {
        std::string name, major;
        int age;
        std::cout << "\n🎓 创建角色\n";
        std::cout << "请输入姓名: ";
        std::cin >> name;
        std::cout << "请输入年龄: ";
        std::cin >> age;
        std::cout << "请输入专业: ";
        std::cin >> major;
        
        stats.setName(name);
        addMessage("🎓 欢迎 " + name + " 来到大学！");
        checkAchievements();
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                saveManager.save(stats, courseManager, skillManager, 
                                npcManager, achievementManager, storySystem,
                                duckManager, eggManager);
                window.close();
                isRunning = false;
            }
            
            if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
            
            if (event.type == sf::Event::TextEntered && isInputting) {
                if (event.text.unicode == 13) {
                    processInput();
                } else if (event.text.unicode == 8) {
                    if (!inputBuffer.empty()) inputBuffer.pop_back();
                } else if (event.text.unicode < 128) {
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        if (isInputting) return;
        if (inStoryMode) {
            if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num9) {
                int choice = key - sf::Keyboard::Num1;
                handleStoryChoice(choice);
            }
            return;
        }
        
        float speed = 5.0f;
        auto pos = playerIcon.getPosition();
        if (key == sf::Keyboard::Up) pos.y -= speed;
        if (key == sf::Keyboard::Down) pos.y += speed;
        if (key == sf::Keyboard::Left) pos.x -= speed;
        if (key == sf::Keyboard::Right) pos.x += speed;
        playerIcon.setPosition(pos);
        
        auto p = playerIcon.getPosition();
        p.x = std::clamp(p.x, 20.0f, 1000.0f);
        p.y = std::clamp(p.y, 740.0f, 740.0f);
        playerIcon.setPosition(p);
        
        if (key == sf::Keyboard::Enter) {
            checkBuildingInteraction();
        }
        
        if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num9) {
            executeMenuOption(key - sf::Keyboard::Num1 + 1);
        }
        
        if (key == sf::Keyboard::S) {
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, storySystem,
                            duckManager, eggManager);
            addMessage("💾 存档成功");
        }
        if (key == sf::Keyboard::L) {
            if (saveManager.load(stats, courseManager, skillManager, 
                                 npcManager, achievementManager, storySystem,
                                 duckManager, eggManager)) {
                addMessage("💾 读档成功");
                stats.updateBuffs();
                checkAchievements();
            } else {
                addMessage("❌ 读档失败");
            }
        }
        if (key == sf::Keyboard::R) {
            startInput("请输入新名字: ");
        }
        if (key == sf::Keyboard::E) {
            eventSystem.triggerRandomEvent(stats, courseManager, skillManager);
            addMessage("🎲 触发了随机事件！");
            checkAchievements();
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, storySystem,
                            duckManager, eggManager);
        }
        if (key == sf::Keyboard::A) {
            showAchievements();
        }
        if (key == sf::Keyboard::F) {
            showRelationshipMenu();
        }
        if (key == sf::Keyboard::D) {  // v3.2.1 新增：查看鸭子状态
            showDuckStatus();
        }
        if (key == sf::Keyboard::Escape) {
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, storySystem,
                            duckManager, eggManager);
            window.close();
            isRunning = false;
        }
    }

    void startInput(const std::string& prompt) {
        isInputting = true;
        inputPrompt = prompt;
        inputBuffer.clear();
        addMessage("✏️ " + prompt);
    }

    void processInput() {
        isInputting = false;
        if (!inputBuffer.empty()) {
            stats.setName(inputBuffer);
            addMessage("✅ 名字已改为: " + inputBuffer);
        }
        inputBuffer.clear();
    }

    void handleStoryChoice(int choice) {
        if (choice >= 0 && choice < currentStoryEvent.choices.size()) {
            auto& chosen = currentStoryEvent.choices[choice];
            std::cout << "\n📖 " << chosen.first << "\n";
            chosen.second(storySystem.storyData, stats);
            storySystem.markTriggered(currentStoryEvent.semester);
            storySystem.addLog("第" + std::to_string(currentStoryEvent.semester) + "学期: " + 
                              currentStoryEvent.title + " -> " + chosen.first);
            inStoryMode = false;
            addMessage("📖 故事推进: " + currentStoryEvent.title);
            checkAchievements();
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, storySystem,
                            duckManager, eggManager);
        }
    }

    void checkBuildingInteraction() {
        auto pos = playerIcon.getPosition();
        for (auto& building : buildings) {
            auto bounds = building->getBounds();
            if (bounds.contains(pos.x, pos.y)) {
                std::cout << "\n🏛️ " << building->getEmoji() << " " 
                          << building->getName() << "\n";
                building->interact(stats, courseManager, duckManager, eggManager);
                
                stats.setGPA(courseManager.calculateGPA());
                
                if (stats.getGPA() >= 3.5 && !skillManager.hasSkill("学习基础")) {
                    skillManager.unlockSkill("学习基础");
                }
                
                // v3.2.1：检查鸭子收集成就
                if (duckManager.isAllFound()) {
                    std::cout << "👑 你找到了所有鸭子！你是真正的鸭子之王！\n";
                    stats.getEmotion().setEmotion(Emotion::Excited);
                }
                
                checkStoryEvents();
                checkAchievements();
                saveManager.save(stats, courseManager, skillManager, 
                                npcManager, achievementManager, storySystem,
                                duckManager, eggManager);
                return;
            }
        }
        std::cout << "🚶 在校园散步... (D查看鸭子 | E触发事件 | F恋情)\n";
        stats.nextDay();
    }

    void checkStoryEvents() {
        auto events = storySystem.getEventsForSemester(stats.getSemester());
        for (auto& event : events) {
            currentStoryEvent = event;
            inStoryMode = true;
            std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << "📖 " << event.title << "\n";
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << event.description << "\n\n";
            for (size_t i = 0; i < event.choices.size(); i++) {
                std::cout << "  " << i+1 << ". " << event.choices[i].first << "\n";
            }
            std::cout << "请选择 (1-" << event.choices.size() << "): ";
            return;
        }
    }

    void checkAchievements() {
        json data = {
            {"courses", courseManager.toJson()},
            {"gpa", stats.getGPA()},
            {"money", stats.getMoney()},
            {"semester", stats.getSemester()},
            {"event_count", stats.getEventCount()},
            {"npcs", npcManager.toJson()},
            {"skills", skillManager.toJson()},
            {"relationships", stats.getRelationships()},
            {"ducks_found", duckManager.getFoundCount()},
            {"eggs_found", eggManager.getFoundCount()},
            {"graduated", false}
        };
        
        std::vector<std::string> newUnlocks;
        achievementManager.checkAchievements(data, newUnlocks);
        
        for (const auto& unlock : newUnlocks) {
            pendingAchievements.push_back(unlock);
            addMessage("🏆 解锁成就: " + unlock);
        }
    }

    void showAchievements() {
        auto achievements = achievementManager.getAchievements();
        std::cout << "\n🏆 成就系统 (" << achievementManager.getUnlockedCount() 
                  << "/" << achievementManager.getTotalCount() << ")\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        for (const auto& a : achievements) {
            std::cout << (a.unlocked ? "✅ " : "⬜ ") 
                      << a.icon << " " << a.name << "\n";
            std::cout << "   " << a.description << "\n";
        }
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    }

    void showRelationshipMenu() {
        std::cout << "\n💕 恋爱关系状态\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        for (auto& rel : stats.getRelationships()) {
            std::cout << "👤 " << rel.getName() 
                      << " | 状态: " << rel.getStatusText()
                      << " | 好感度: " << rel.getAffection() << "\n";
            if (rel.getIsRomantic()) {
                std::cout << "   💕 恋爱中！\n";
            }
        }
        std::cout << "\n输入 NPC 名字进行特殊互动: ";
        std::string name;
        std::cin >> name;
        
        auto* rel = stats.getRelationship(name);
        if (rel) {
            if (rel->getIsRomantic()) {
                std::cout << "💔 要分手吗? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    rel->breakUp();
                    stats.getEmotion().setEmotion(Emotion::Sad);
                    addMessage("💔 与 " + name + " 分手了");
                }
            } else if (name != "鸭哥") {
                std::cout << "💕 要表白吗? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    rel->confess();
                    if (rel->getIsRomantic()) {
                        stats.getEmotion().setEmotion(Emotion::Loved);
                        addMessage("💕 与 " + name + " 开始恋爱！");
                    } else {
                        addMessage("💔 表白被拒绝了...");
                    }
                }
            } else {
                std::cout << "🦆 鸭哥：嘎嘎嘎嘎！（翻译：你是个好人，但我们不合适）\n";
            }
            checkAchievements();
        } else {
            std::cout << "❌ 未找到 NPC: " << name << "\n";
        }
    }

    void showDuckStatus() {  // v3.2.1 新增
        std::cout << "\n🦆 鸭子收集进度 (" << duckManager.getFoundCount() 
                  << "/" << duckManager.getTotalCount() << ")\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        auto ducks = duckManager.getDucks();
        for (const auto& d : ducks) {
            std::cout << (d.found ? "✅ " : "⬜ ") 
                      << d.name << " - " << d.location << "\n";
            if (d.found) {
                std::cout << "   📖 " << d.description << "\n";
            }
        }
        
        if (duckManager.isAllFound()) {
            std::cout << "\n👑 恭喜！你找到了所有鸭子！\n";
            std::cout << "🦆 鸭哥向你致敬！\n";
        }
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    }

    void executeMenuOption(int option) {
        switch (option) {
            case 1: {
                auto& emo = stats.getEmotion();
                std::stringstream ss;
                ss << "📊 " << stats.getName() 
                   << " | 学期: " << stats.getSemester()
                   << " | GPA: " << std::fixed << std::setprecision(2) << stats.getGPA()
                   << " | 课程: " << courseManager.getCourseCount()
                   << " | 技能: " << skillManager.getSkillCount()
                   << " | 💰 " << stats.getMoney()
                   << " | 天: " << stats.getDayCount()
                   << "\n😊 心情: " << getEmoji(emo.getEmotion())
                   << " | 孤独感: " << emo.getLoneliness() << "%"
                   << " | 稳定性: " << emo.getStability() << "%"
                   << "\n🦆 鸭子: " << duckManager.getFoundCount() 
                   << "/" << duckManager.getTotalCount()
                   << " | 🥚 彩蛋: " << eggManager.getFoundCount()
                   << "/" << eggManager.getTotalCount();
                addMessage(ss.str());
                break;
            }
            case 2: {
                auto available = courseManager.getAvailableCourses();
                if (available.empty()) {
                    addMessage("❌ 没有更多课程可选");
                    break;
                }
                std::cout << "\n📚 可选课程:\n";
                for (size_t i = 0; i < available.size(); i++) {
                    std::cout << "  " << i + 1 << ". " << available[i].getName() 
                              << " (" << available[i].getCredit() << "学分)\n";
                }
                std::cout << "选择课程编号 (0取消): ";
                int choice;
                std::cin >> choice;
                if (choice > 0 && choice <= available.size()) {
                    if (courseManager.takeCourse(available[choice-1].getName())) {
                        addMessage("📖 选修了: " + available[choice-1].getName());
                        stats.modifyEnergy(-5);
                        checkAchievements();
                    } else {
                        addMessage("⚠️ 已选修该课程");
                    }
                }
                break;
            }
            case 3: {
                auto courses = courseManager.getCourses();
                if (courses.empty()) {
                    addMessage("⚠️ 请先选修课程！");
                    break;
                }
                std::cout << "\n📝 选择要学习的课程:\n";
                for (size_t i = 0; i < courses.size(); i++) {
                    std::cout << "  " << i + 1 << ". " << courses[i].getName() 
                              << (courses[i].getScore() > 0 ? " (已考)" : " (未考)") << "\n";
                }
                std::cout << "选择课程编号 (0取消): ";
                int choice;
                std::cin >> choice;
                if (choice > 0 && choice <= courses.size()) {
                    int hours;
                    std::cout << "学习几个小时? (1-5): ";
                    std::cin >> hours;
                    hours = std::clamp(hours, 1, 5);
                    courseManager.studyForExam(courses[choice-1].getName(), hours, stats);
                    stats.setGPA(courseManager.calculateGPA());
                    addMessage("📝 学习了 " + courses[choice-1].getName());
                    
                    if (courses[choice-1].getScore() >= 85) {
                        std::vector<std::string> possible = {
                            "编程能力", "数学思维", "英语应用", "数据分析", "项目管理"
                        };
                        std::uniform_int_distribution<int> dist(0, possible.size()-1);
                        std::mt19937 rng(std::time(nullptr));
                        skillManager.unlockSkill(possible[dist(rng)]);
                        checkAchievements();
                    }
                }
                break;
            }
            case 4: {
                auto& npcs = npcManager.getNPCs();
                std::cout << "\n💬 选择要互动的NPC:\n";
                for (size_t i = 0; i < npcs.size(); i++) {
                    auto* rel = stats.getRelationship(npcs[i].getName());
                    std::cout << "  " << i + 1 << ". " << npcs[i].getName() 
                              << " (好感度: " << npcs[i].getFavorability() << ")"
                              << (rel && rel->getIsRomantic() ? " 💕" : "") << "\n";
                }
                std::cout << "选择NPC (0取消): ";
                int choice;
                std::cin >> choice;
                if (choice > 0 && choice <= npcs.size()) {
                    npcManager.interactWithNPC(choice-1, stats, courseManager);
                    addMessage("💬 与 " + npcs[choice-1].getName() + " 互动");
                    stats.setGPA(courseManager.calculateGPA());
                    checkAchievements();
                    saveManager.save(stats, courseManager, skillManager, 
                                    npcManager, achievementManager, storySystem,
                                    duckManager, eggManager);
                }
                break;
            }
            case 5: {
                if (courseManager.getCourses().empty()) {
                    addMessage("⚠️ 请先选修课程！");
                } else {
                    courseManager.examWeek(stats);
                    stats.setGPA(courseManager.calculateGPA());
                    addMessage("📝 考试周结束！");
                    checkAchievements();
                    saveManager.save(stats, courseManager, skillManager, 
                                    npcManager, achievementManager, storySystem,
                                    duckManager, eggManager);
                }
                break;
            }
            case 6: {
                stats.nextSemester();
                stats.modifyEnergy(20);
                stats.modifyHappiness(10);
                stats.updateBuffs();
                stats.getEmotion().modifyLoneliness(-10);
                stats.setGPA(courseManager.calculateGPA());
                addMessage("📅 进入第 " + std::to_string(stats.getSemester()) + " 学期");
                
                checkStoryEvents();
                
                saveManager.save(stats, courseManager, skillManager, 
                                npcManager, achievementManager, storySystem,
                                duckManager, eggManager);
                break;
            }
            case 7: {
                int bonus = stats.hasBuff("精力恢复+50%") ? 60 : 40;
                stats.modifyEnergy(bonus);
                stats.nextDay();
                stats.getEmotion().modifyLoneliness(-5);
                addMessage("😴 休息了一天，精力 +" + std::to_string(bonus));
                break;
            }
            case 8: {
                stats.modifyHappiness(5);
                stats.modifyEnergy(-5);
                stats.nextDay();
                stats.getEmotion().modifyLoneliness(5);
                addMessage("🎮 摸鱼中... 快乐+5 精力-5");
                break;
            }
            case 9: {
                if (stats.getSemester() >= 8 && 
                    stats.getGPA() >= 2.0 && 
                    courseManager.getCourseCount() >= 20) {
                    addMessage("🎉🎉🎉 恭喜毕业！！！");
                    addMessage("📊 最终GPA: " + std::to_string(stats.getGPA()));
                    addMessage("🏆 掌握技能: " + std::to_string(skillManager.getSkillCount()) + "个");
                    addMessage("🏅 成就解锁: " + std::to_string(achievementManager.getUnlockedCount()) + "个");
                    addMessage("🦆 鸭子收集: " + std::to_string(duckManager.getFoundCount()) + "/10");
                    addMessage("🥚 彩蛋发现: " + std::to_string(eggManager.getFoundCount()) + "个");
                    isRunning = false;
                } else {
                    std::string reason;
                    if (stats.getSemester() < 8) 
                        reason += "学期数不足(" + std::to_string(stats.getSemester()) + "/8) ";
                    if (stats.getGPA() < 2.0) 
                        reason += "GPA不足(" + std::to_string(stats.getGPA()) + "/2.0) ";
                    if (courseManager.getCourseCount() < 20) 
                        reason += "课程数不足(" + std::to_string(courseManager.getCourseCount()) + "/20)";
                    addMessage("❌ 不满足毕业条件: " + reason);
                }
                break;
            }
            default:
                addMessage("❌ 无效选项");
        }
        stats.setGPA(courseManager.calculateGPA());
        saveManager.save(stats, courseManager, skillManager, 
                        npcManager, achievementManager, storySystem,
                        duckManager, eggManager);
    }

    void addMessage(const std::string& msg) {
        messages.push_back(msg);
        if (messages.size() > 20) messages.erase(messages.begin());
        std::cout << msg << "\n";
    }

    void update() {
        if (stats.getEnergy() <= 0) {
            addMessage("😵 精力耗尽，累倒了...");
            isRunning = false;
        }
        if (stats.getHappiness() <= 0) {
            addMessage("😢 太不开心了，退学了...");
            isRunning = false;
        }
        
        stats.updateBuffs();
        
        static int frameCount = 0;
        if (++frameCount % 1200 == 0) {
            saveManager.save(stats, courseManager, skillManager, 
                            npcManager, achievementManager, storySystem,
                            duckManager, eggManager);
        }
    }

    void render() {
        window.clear(sf::Color(34, 139, 34));
        
        // 绘制湖 (v3.2.1 新增)
        sf::CircleShape lake(120);
        lake.setPosition(300, 280);
        lake.setFillColor(sf::Color(30, 144, 255, 180));
        lake.setOutlineColor(sf::Color::White);
        lake.setOutlineThickness(1);
        window.draw(lake);
        
        // 绘制鸭子图标
        auto ducks = duckManager.getDucks();
        for (size_t i = 0; i < ducks.size() && i < duckIcons.size(); i++) {
            if (!ducks[i].found) {
                duckIcons[i].setPosition(ducks[i].x - 8, ducks[i].y - 8);
                window.draw(duckIcons[i]);
            }
        }
        
        // 网格
        for (int i = 0; i < 10; i++) {
            sf::RectangleShape line({1000, 1});
            line.setPosition(10, 100 + i * 60);
            line.setFillColor({255, 255, 255, 30});
            window.draw(line);
        }
        
        // 建筑
        for (auto& building : buildings) {
            sf::RectangleShape rect(building->getBounds().getSize());
            rect.setPosition(building->getBounds().getPosition());
            rect.setFillColor(building->getColor());
            rect.setOutlineColor(sf::Color::White);
            rect.setOutlineThickness(1);
            window.draw(rect);
            
            sf::Text text;
            text.setFont(font);
            text.setString(building->getEmoji() + " " + building->getName());
            text.setCharacterSize(14);
            text.setFillColor(sf::Color::White);
            text.setPosition(building->getBounds().left + 5, 
                            building->getBounds().top + building->getBounds().height + 5);
            window.draw(text);
        }
        
        window.draw(statusPanel);
        drawStatusInfo();
        
        window.draw(menuPanel);
        drawMenu();
        
        window.draw(messagePanel);
        drawMessages();
        
        if (!pendingAchievements.empty()) {
            drawAchievementPopup();
        }
        
        window.draw(playerIcon);
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString("🎓 " + stats.getName());
        nameText.setCharacterSize(14);
        nameText.setFillColor(sf::Color::White);
        auto pos = playerIcon.getPosition();
        nameText.setPosition(pos.x - 20, pos.y - 30);
        window.draw(nameText);
        
        sf::Text hint;
        hint.setFont(font);
        hint.setString("方向键 | Enter交互 | 1-9菜单 | S存档 | L读档 | D鸭子 | E事件 | A成就 | F恋情");
        hint.setCharacterSize(12);
        hint.setFillColor(sf::Color::White);
        hint.setPosition(320, 755);
        window.draw(hint);
        
        if (inStoryMode) {
            drawStoryUI();
        }
        
        if (isInputting) {
            sf::RectangleShape inputBox({400, 30});
            inputBox.setPosition(320, 720);
            inputBox.setFillColor({0, 0, 0, 200});
            inputBox.setOutlineColor(sf::Color::White);
            inputBox.setOutlineThickness(1);
            window.draw(inputBox);
            
            sf::Text inputText;
            inputText.setFont(font);
            inputText.setString("> " + inputBuffer + "_");
            inputText.setCharacterSize(16);
            inputText.setFillColor(sf::Color::White);
            inputText.setPosition(325, 722);
            window.draw(inputText);
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
           << "🏅 成就: " << achievementManager.getUnlockedCount() 
           << "/" << achievementManager.getTotalCount() << "\n"
           << "🦆 鸭子: " << duckManager.getFoundCount() 
           << "/" << duckManager.getTotalCount() << "\n"
           << "🥚 彩蛋: " << eggManager.getFoundCount()
           << "/" << eggManager.getTotalCount();
        text.setString(ss.str());
        text.setPosition(20, 20);
        window.draw(text);
        
        drawProgressBar(20, 240, 200, 10, stats.getEnergy(), sf::Color::Green, "精力");
        drawProgressBar(20, 265, 200, 10, stats.getHappiness(), sf::Color::Yellow, "快乐");
        
        sf::Text emotionText;
        emotionText.setFont(font);
        emotionText.setString("😊 " + getEmoji(emo.getEmotion()) + " 孤独: " + 
                              std::to_string(emo.getLoneliness()) + "%");
        emotionText.setCharacterSize(12);
        emotionText.setFillColor(sf::Color::Cyan);
        emotionText.setPosition(20, 290);
        window.draw(emotionText);
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
            "7. 😴 休息", "8. 🎮 摸鱼", "9. 🎓 毕业"
        };
        
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        
        for (size_t i = 0; i < items.size(); i++) {
            text.setString(items[i]);
            text.setPosition(20, 300 + i * 35);
            window.draw(text);
        }
        
        sf::Text hint;
        hint.setFont(font);
        hint.setString("🦆 D=鸭子  🎲 E=事件  🏆 A=成就  💕 F=恋情");
        hint.setCharacterSize(13);
        hint.setFillColor(sf::Color::Yellow);
        hint.setPosition(20, 620);
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

    void drawAchievementPopup() {
        if (pendingAchievements.empty()) return;
        
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

    void drawStoryUI() {
        sf::RectangleShape overlay({1024, 768});
        overlay.setPosition(0, 0);
        overlay.setFillColor({0, 0, 0, 180});
        window.draw(overlay);
        
        sf::RectangleShape box({800, 400});
        box.setPosition(112, 184);
        box.setFillColor({40, 40, 40, 240});
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2);
        window.draw(box);
        
        sf::Text title;
        title.setFont(font);
        title.setString(currentStoryEvent.title);
        title.setCharacterSize(28);
        title.setFillColor(sf::Color::Yellow);
        title.setPosition(130, 200);
        window.draw(title);
        
        sf::Text desc;
        desc.setFont(font);
        desc.setString(currentStoryEvent.description);
        desc.setCharacterSize(18);
        desc.setFillColor(sf::Color::White);
        desc.setPosition(130, 250);
        window.draw(desc);
        
        for (size_t i = 0; i < currentStoryEvent.choices.size(); i++) {
            sf::Text choice;
            choice.setFont(font);
            choice.setString(std::to_string(i+1) + ". " + currentStoryEvent.choices[i].first);
            choice.setCharacterSize(18);
            choice.setFillColor(sf::Color::Cyan);
            choice.setPosition(130, 350 + i * 40);
            window.draw(choice);
        }
        
        sf::Text hint;
        hint.setFont(font);
        hint.setString("按数字键 1-" + std::to_string(currentStoryEvent.choices.size()) + " 选择");
        hint.setCharacterSize(16);
        hint.setFillColor(sf::Color::White);
        hint.setPosition(130, 510);
        window.draw(hint);
    }
};

// ============================================================
// 16. main
// ============================================================
int main() {
    std::srand(std::time(nullptr));
    
    std::cout << "🎓 学生生活模拟器 v3.2.1 🦆\n";
    std::cout << "========================================\n";
    std::cout << "✨ v3.2.1 新功能:\n";
    std::cout << "  🦆 校园湖与鸭子收集 (10只鸭子)\n";
    std::cout << "  🦆 新NPC: 鸭哥\n";
    std::cout << "  📖 《鸭科动物大全》彩蛋\n";
    std::cout << "  🏆 新成就: 鸭子守护者、鸭子之王等\n";
    std::cout << "  🥚 彩蛋系统\n";
    std::cout << "  🔑 D键查看鸭子状态\n";
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
