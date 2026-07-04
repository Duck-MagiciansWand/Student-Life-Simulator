# 🎓 Student Life Simulator v3.2.3

### *一款基于 C++17 和 SFML 的校园生活叙事模拟游戏*

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)](https://www.sfml-dev.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-orange.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()
[![Version](https://img.shields.io/badge/Version-3.2.3-brightgreen.svg)]()
[![Status](https://img.shields.io/badge/Status-Stable-success.svg)]()

---

## 📖 项目简介

**Student Life Simulator** 是一款沉浸式的大学生活模拟游戏。玩家控制一个角色在可视化 2D 校园中自由探索、学习课程、社交互动、发展恋爱关系，体验从大一新生到毕业的完整大学生涯。

**v3.2.3 版本（“云朵与星空”更新）** 在 v3.2.2 的鸭子与传送门基础上，加入了**特殊的云朵彩蛋**和**与虚舟实验室（CaelLab）的联动**。本次更新让游戏世界更加生动，隐藏着更多等待发现的秘密。

---

## ✨ v3.2.3 核心亮点

| 新增内容 | 说明 |
| :--- | :--- |
| ☁️ **特殊云朵** | 夜空中会出现一朵特殊的云，只有夜晚才能看到 |
| 🌙 **睡眠系统** | 睡觉时有 50% 概率触发云朵事件 |
| 🔗 **CaelLab 联动** | 触发云朵事件后自动打开虚舟实验室官网 |
| 🌟 **昼夜循环** | 游戏世界有白天和夜晚的视觉变化 |
| 🏆 **新成就** | 新增“追云者”和“虚舟访客”两个成就 |

### 🎯 核心特色

| 特色 | 说明 |
| :--- | :--- |
| ☁️ **特殊云朵彩蛋** (v3.2.3) | 夜晚睡觉有 50% 概率看到发光的云朵，触发隐藏联动 |
| 🔗 **CaelLab 联动** (v3.2.3) | 云朵事件触发后自动打开 [虚舟实验室官网](https://www.caellab.com/) |
| 🌙 **昼夜循环** (v3.2.3) | 白天/夜晚视觉切换，夜晚有星空效果 |
| 🦆 **鸭子收集系统** | 16 种鸭子（普通/稀有/史诗/传说）分布在校园各处 |
| 🧬 **鸭子繁殖系统** | 收集到的鸭子可以配对繁殖，产生新品种 |
| 🚽 **传送门彩蛋** | 学校厕所随机出现传送门，按 F 进入隐藏区域 |
| 🎂 **一周年庆典** | 收集 10 只鸭子后解锁周年庆传送门 |
| 💕 **恋爱系统** | 与 NPC（学姐/室友/导师）发展浪漫关系 |
| 📖 **故事分支** | 关键剧情拥有 2-3 种不同结局 |
| 🏆 **成就系统** | 20+ 个成就，包括隐藏成就 |
| 💾 **JSON 存档** | 人类可读的存档格式，跨平台兼容 |

---

## 🏗️ 系统架构

### 类结构设计

```text
┌─────────────────────────────────────────────────────────────────┐
│                       GameManager                                │
│  - 游戏主循环 / 事件处理 / 渲染 / UI 管理                       │
└──────────────────────────┬──────────────────────────────────────┘
                           │
           ┌───────────────┼───────────────┐
           ▼               ▼               ▼
   ┌───────────────┐ ┌───────────────┐ ┌───────────────┐
   │    Stats      │ │CourseManager  │ │ SkillManager  │
   │  核心属性/状态 │ │ 课程/成绩管理  │ │  技能解锁/查询 │
   └───────────────┘ └───────────────┘ └───────────────┘
           │               │               │
           └───────────────┼───────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                        辅助系统层                                │
├─────────────────┬─────────────────┬─────────────────────────────┤
│  EventSystem    │   NPCManager    │   AchievementManager        │
│  随机事件生成    │ NPC好感度/交互   │   成就解锁/查询             │
├─────────────────┴─────────────────┴─────────────────────────────┤
│  CloudSystem (☁️ v3.2.3)  |  DuckCollection (🦆 16种)          │
│  PortalSystem (🚽 传送门)  |  AnniversaryRoom (🎂 周年庆)      │
│  SaveManager (💾 JSON存档)                                     │
└─────────────────────────────────────────────────────────────────┘
```

### 技术栈

| 技术 | 版本 | 用途 |
| :--- | :--- | :--- |
| **C++** | C++17 | 核心开发语言 |
| **SFML** | 2.6+ | 图形渲染、窗口管理、事件处理 |
| **nlohmann/json** | 3.11+ | 游戏存档的序列化与反序列化 |
| **STL** | C++17 | 标准容器、算法、文件系统操作 |

---

## 🦆 鸭子系统（16 种）

### 鸭子种类

| 稀有度 | 数量 | 鸭子名称 |
| :--- | :--- | :--- |
| 普通 | 6 只 | 小黄、大花、斑斑、雪球、黑豆、灰灰 |
| 稀有 | 4 只 | 金毛、白鹭、小橘、鸭哥 |
| 史诗 | 3 只 | 星河鸭、极光鸭、机械鸭 |
| 传说 | 3 只 | 彩虹鸭、幽灵鸭、凤凰鸭 |

### 繁殖规则

| 亲代组合 | 后代稀有度概率 |
| :--- | :--- |
| 史诗 × 史诗 | 5% 传说 / 95% 史诗 |
| 史诗 × 稀有 | 30% 史诗 / 70% 稀有 |
| 稀有 × 稀有 | 10% 史诗 / 90% 稀有 |
| 普通 × 普通 | 20% 稀有 / 80% 普通 |

---

## 🚽 传送门系统

| 传送门 | 出现条件 | 效果 |
| :--- | :--- | :--- |
| 🚽 厕所传送门 | 每 30-60 秒随机出现 | 按 F 进入，获得随机奖励 |
| 🎂 周年庆传送门 | 收集 10 只鸭子后 | 进入 MagiciansWand 一周年庆典 |

---

## ☁️ 云朵系统 (v3.2.3 新增)

| 属性 | 说明 |
| :--- | :--- |
| **触发条件** | 夜晚睡觉，50% 概率 |
| **视觉效果** | 发光的云朵出现在夜空，带“☁️ ?”标识 |
| **触发效果** | 弹出提示框 → 自动打开 [CaelLab 官网](https://www.caellab.com/) |
| **奖励** | 解锁“追云者”成就 + 快乐 +10 |

### 触发流程

```text
1. 等待夜晚（游戏内自动昼夜切换）
   ↓
2. 选择菜单 7（睡觉）
   ↓
3. 50% 概率触发云朵事件
   ↓
4. 弹出提示框：“你看到了特殊云朵”
   ↓
5. 自动打开浏览器 → https://www.caellab.com/
   ↓
6. 解锁成就 + 获得奖励
```

---

## 🏆 成就系统（20+ 个）

### v3.2.3 新增成就

| 成就 | 条件 |
| :--- | :--- |
| ☁️ 追云者 | 在夜晚看到特殊云朵 |
| 🔗 虚舟访客 | 通过云朵打开 CaelLab 网站 |

### 完整成就列表

| 成就 | 条件 |
| :--- | :--- |
| 📚 初入学术 | 选修第一门课程 |
| 💪 学霸之路 | 一门课程考到 90 分以上 |
| 🌟 优秀毕业生 | GPA 达到 3.5 以上 |
| 🦆 初级鸭迷 | 收集 3 只鸭子 |
| 🦆 中级鸭迷 | 收集 6 只鸭子 |
| 🦆 高级鸭迷 | 收集 9 只鸭子 |
| 🏆 鸭王之王 | 收集全部 16 只鸭子 |
| 🧬 繁殖大师 | 成功繁殖 5 次 |
| 🚽 传送门旅人 | 通过传送门 |
| 🎂 周年庆嘉宾 | 参加一周年庆典 |
| ☁️ 追云者 | 看到特殊云朵 |
| 🔗 虚舟访客 | 打开 CaelLab 网站 |

---

## 🎮 游戏指南

### 操作说明

| 按键 | 功能 | 引入版本 |
| :--- | :--- | :--- |
| `⬆ ⬇ ⬅ ➡` | 控制角色在校园中移动 | v1.0 |
| `Enter` | 与当前所在位置的建筑/鸭子/NPC 交互 | v1.0 |
| `1` - `9` | 执行主菜单中的对应功能 | v1.0 |
| `S` | 手动存档 | v2.0 |
| `L` | 读取存档 | v2.0 |
| `R` | 修改角色名字 | v2.0 |
| `E` | 主动触发一次随机事件 | v3.0 |
| `A` | 打开成就面板 | v3.1 |
| `F` | 传送门交互 | v3.2 |
| `7` | 睡觉（触发云朵事件） | v3.2.3 |
| `ESC` | 退出游戏 | v1.0 |

### 菜单功能

| 选项 | 功能 |
| :--- | :--- |
| 1. 📊 状态 | 查看角色属性、鸭子收集进度 |
| 2. 📖 选课 | 选修新课程 |
| 3. 📝 学习 | 学习课程，提升成绩 |
| 4. 💬 NPC | 与 NPC 互动，提升好感度 |
| 5. 📝 考试周 | 集中考试所有已修课程 |
| 6. 📅 新学期 | 推进到下一学期 |
| 7. 😴 睡觉 | 恢复精力，触发云朵事件 |
| 8. 🎮 摸鱼 | 放松娱乐 |
| 9. 🎓 毕业 | 检查毕业条件 |

### 毕业条件

1. ✅ 完成 **8 个学期**
2. ✅ 最终 GPA ≥ **2.0**
3. ✅ 修满 **20 门课程**

---

## 🚀 快速开始

### 环境配置

#### Windows
```bash
# 1. 安装 MinGW-w64 (https://www.mingw-w64.org/)
# 2. 安装 SFML (https://www.sfml-dev.org/download/sfml/2.6.1/)
#    解压到 C:\SFML-2.6.1
# 3. 安装 nlohmann/json
#    方式A: vcpkg install nlohmann-json
#    方式B: 下载 single header (https://github.com/nlohmann/json/releases)
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install g++ make libsfml-dev nlohmann-json3-dev
```

#### macOS
```bash
brew install sfml nlohmann-json
```

### 编译与运行

**推荐使用 CMake**：
```bash
git clone https://github.com/Duck-MagiciansWand/Student-Life-Simulator.git
cd Student-Life-Simulator
mkdir build && cd build
cmake ..
cmake --build .
./bin/student_simulator
```

**手动编译 (Windows/MinGW)**：
```bash
g++ -o student_simulator.exe main.cpp -std=c++17 \
    -I C:\SFML-2.6.1\include \
    -I C:\vcpkg\installed\x64-windows\include \
    -L C:\SFML-2.6.1\lib \
    -L C:\vcpkg\installed\x64-windows\lib \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -lstdc++fs
```

---

## 📁 项目结构

```text
Student-Life-Simulator/
├── main.cpp                 # 游戏主程序 (单文件实现)
├── CMakeLists.txt           # CMake 构建配置
├── README.md                # 项目说明文档
├── LICENSE                  # Apache-2.0 许可证
├── .gitignore               # Git 忽略文件配置
├── screenshots/             # (建议) 存放游戏截图
│   ├── gameplay.png
│   ├── duck.png
│   └── cloud.png            # 云朵事件截图
└── saves/                   # (自动创建) 游戏存档目录
    └── autosave.json
```

---

## 📝 更新日志

### v3.2.3 (2026-07-04) - 云朵与星空更新
- ☁️ **新增云朵系统**：夜晚睡觉 50% 概率触发特殊云朵
- 🔗 **新增 CaelLab 联动**：云朵触发后打开 https://www.caellab.com/
- 🌙 **新增昼夜循环**：白天/夜晚视觉切换
- 🏆 **新增成就**：“追云者”和“虚舟访客”
- ✨ **UI 优化**：夜晚星空效果

### v3.2.2 (2026-07-04) - 鸭子纪元更新
- 🦆 新增 16 种鸭子（普通/稀有/史诗/传说）
- 🧬 新增鸭子繁殖系统
- 🚽 新增厕所传送门彩蛋
- 🎂 新增 MagiciansWand 一周年庆典场景

### v3.2.1 (2026-07-03) - 鸭子与彩蛋更新
- 🦆 新增校园湖与 10 只鸭子收集
- 🦆 新增 NPC“鸭哥”
- 📖 新增《鸭科动物大全》隐藏书籍

### v3.2.0 (2026-07-03) - 恋爱与探索更新
- 💕 新增恋爱系统
- 🔍 新增探索系统
- 🎭 新增情感状态

### v3.1.0 (2026-06-28) - 故事与成就更新
- 📖 新增故事主线
- 🏆 新增成就系统

### v3.0.0 (2026-06-27) - 架构重构
- 🎲 随机事件系统
- 💬 NPC 好感度系统
- 💾 JSON 存档

---

## 🤝 如何贡献

1. **Fork** 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 **Pull Request**

### 代码风格
- **类名**：驼峰命名法，如 `GameManager`
- **函数名**：驼峰命名法，如 `handleKeyPress`
- **变量名**：下划线命名法，如 `energy_value`
- **常量**：全大写，如 `MAX_ENERGY`

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

- [**SFML**](https://www.sfml-dev.org/) - 2D 图形库
- [**nlohmann/json**](https://github.com/nlohmann/json) - JSON 库
- [**虚舟实验室 (CaelLab)**](https://www.caellab.com/) - 合作伙伴与联动支持
- 所有贡献者和玩家

---

## ⭐ Star History

如果这个项目对你有帮助，请点亮右上角的 Star ⭐！

[![Star History Chart](https://api.star-history.com/svg?repos=Duck-MagiciansWand/Student-Life-Simulator&type=Date)](https://star-history.com/#Duck-MagiciansWand/Student-Life-Simulator&Date)

---

**祝你大学生活愉快！☁️🎉**
