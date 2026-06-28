
# 🎓 Student Life Simulator v3.1

### *一款基于 C++17 和 SFML 的校园生活叙事模拟游戏*

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)](https://www.sfml-dev.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-orange.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

---

## 📖 项目简介

**Student Life Simulator** 是一款沉浸式的大学生活模拟游戏。你控制一个角色在可视化的 2D 校园中探索、学习、社交，体验从大一新生到毕业的完整大学生活。**v3.1 版本**在原有基础上，加入了完整的**故事主线**和**成就系统**，让游戏从“数值模拟”蜕变为一个有温度、有深度的“叙事体验”。

### ✨ v3.1 核心亮点

| 新增系统 | 说明 |
| :--- | :--- |
| 📖 **故事主线** | 覆盖大学四年的 5 个核心剧情事件，你的选择将影响角色命运。 |
| 🏆 **成就系统** | 20+ 个 Steam 风格成就，为你的大学生涯留下里程碑。 |
| 💬 **增强NPC交互** | NPC 对话更丰富，好感度奖励更实用，关系养成更有趣。 |
| 📊 **实时数据统计** | 游戏内可查看游玩天数、触发事件数等详细数据。 |

### 🎯 核心特色

| 特色 | 说明 |
| :--- | :--- |
| 🗺️ **2D 校园地图** | 宿舍、图书馆、教室、食堂、健身房等 5 个可交互场景。 |
| 🎲 **随机事件系统** | 10+ 种随机事件（奇遇、事故、机遇），每次游玩体验独一无二。 |
| 💬 **NPC 好感度** | 与学姐、室友、导师三位角色互动，好感度解锁特殊奖励与剧情。 |
| 🏆 **技能与 Buff 系统** | 考试成绩优秀可获得技能；临时增益（如“学习效率+20%”）助你冲刺。 |
| 💾 **JSON 存档** | 存档文件人类可读，跨平台兼容，便于备份与修改。 |
| 🎓 **毕业目标** | 明确的胜利条件：完成 8 个学期、GPA ≥ 2.0、修满 20 门课程。 |

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
│  随机事件生成    │ NPC好感度/交互   │   成就解锁/查询 (v3.1 新增)  │
├─────────────────┴─────────────────┴─────────────────────────────┤
│  StorySystem (v3.1 新增)   |    SaveManager (JSON 存档)        │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                      建筑交互层 (多态)                           │
├──────────┬──────────┬──────────┬──────────┬────────────────────┤
│ Dormitory │ Library  │ Classroom│ Canteen  │        Gym         │
│  宿舍     │  图书馆   │  教室    │  食堂    │       健身房        │
└──────────┴──────────┴──────────┴──────────┴────────────────────┘
```

### 技术栈

| 技术 | 用途 |
| :--- | :--- |
| **C++17** | 核心开发语言 |
| **SFML 2.6** | 图形渲染、窗口管理、事件处理 |
| **nlohmann/json** | 游戏存档的序列化与反序列化 |
| **STL** | 标准容器、算法、文件系统操作 |

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
git clone https://github.com/MC-MCL/Student-Life-Simulator.git
cd Student-Life-Simulator
mkdir build && cd build
cmake ..
cmake --build .
./bin/student_simulator   # Linux/macOS
# student_simulator.exe   # Windows
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

**手动编译 (Linux)**：
```bash
g++ -o student_simulator main.cpp -std=c++17 \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -lstdc++fs
```

---

## 🎮 游戏指南

### 操作说明

| 按键 | 功能 |
| :--- | :--- |
| `⬆ ⬇ ⬅ ➡` | 控制角色在校园中移动 |
| `Enter` | 与当前所在位置的建筑交互 |
| `1` - `9` | 执行主菜单中的对应功能 |
| `S` | 手动存档 |
| `L` | 读取存档 |
| `R` | 修改角色名字 |
| `E` | 主动触发一次随机事件 |
| `A` | 打开成就面板 (v3.1 新增) |
| `ESC` | 退出游戏 |

### 校园建筑

| 建筑 | 交互效果 |
| :--- | :--- |
| 🏠 **宿舍** | 精力 +40（有 Buff 时 +60） |
| 📚 **图书馆** | 提升指定课程的考试成绩 |
| 🏫 **教室** | 进入考试周，集中考核所有已修课程 |
| 🍜 **食堂** | 精力 +20，快乐 +8 |
| 💪 **健身房** | 精力 +15，快乐 +10 |

### NPC 好感度奖励

与 NPC 互动可提升好感度，解锁特殊奖励：

| NPC | 好感度阈值 | 奖励效果 |
| :--- | :--- | :--- |
| **学姐** | 30 | 获得“复习资料”：学习效率 +20% (持续 2 天) |
| | 60 | 获得“考试重点”：随机一门课程成绩提升 |
| **室友** | 30 | “一起吃饭”：精力 +10，快乐 +15 |
| | 60 | “组队学习”：学习效率 +20% (持续 1 天) |
| **导师** | 30 | “学术指导”：快乐 +10 |
| | 60 | “推荐信”：大幅提升毕业竞争力 |

### 随机事件（精选）

| 事件 | 效果示例 |
| :--- | :--- |
| 📚 捡到笔记 | 获得“学习效率+20%”Buff，持续 3 天 |
| 🎮 室友通宵 | 精力 -15，快乐 -5 |
| 💼 找到兼职 | 金钱 +300，快乐 +10 |
| 🏆 竞赛获奖 | 快乐 +20，解锁“编程能力”技能 |
| ☕ 咖啡店偶遇 | 快乐 +15，解锁“项目管理”技能 |

### 🎓 毕业条件

要成功毕业，你需要达成以下目标：

1.  ✅ 完成 **8 个学期**
2.  ✅ 最终 GPA ≥ **2.0**
3.  ✅ 修满 **20 门课程**

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
│   └── campus.png
└── saves/                   # (自动创建) 游戏存档目录
    └── autosave.json
```

---

## 📝 更新日志

### v3.1.0 (2026-06-28) - 故事与成就更新
- ✨ **新增故事主线**：5 个核心剧情事件，覆盖大学四年关键节点。
- ✨ **新增成就系统**：20+ 个成就，增加重复游玩动力。
- ✨ **增强 NPC 交互**：更丰富的对话与好感度奖励。
- ✨ **新增数据统计**：游戏内显示游玩天数、事件总数等。
- 🐛 **修复**：若干稳定性与 UI 显示问题。

### v3.0.0 (2026-06-27) - 架构重构
- ✅ 引入随机事件、NPC、技能、Buff 系统。
- ✅ 采用 JSON 格式进行存档。
- ✅ 使用多态重构建筑交互，提升代码可扩展性。

### v2.0.0 (2026-06-26) - 图形化版本
- ✅ 基于 SFML 的 2D 图形界面。
- ✅ 可视化校园地图与角色实时移动。

### v1.0.0 (2026-06-26) - 初始版本
- ✅ 控制台版本，包含基础课程与考试系统。

---

## 🤝 如何贡献

我们欢迎任何形式的贡献！请遵循以下流程：

1.  **Fork** 本仓库。
2.  创建你的功能分支 (`git checkout -b feature/AmazingFeature`)。
3.  提交你的更改 (`git commit -m 'Add some AmazingFeature'`)。
4.  推送到分支 (`git push origin feature/AmazingFeature`)。
5.  打开一个 **Pull Request**。

### 代码风格指引
- **类名**：驼峰命名法，如 `GameManager`。
- **函数名**：驼峰命名法，如 `handleKeyPress`。
- **变量名**：下划线命名法，如 `energy_value`。
- **常量**：全大写，如 `MAX_ENERGY`。
- **注释**：为复杂逻辑和公共接口添加清晰注释。

---

## 📄 许可证

本项目采用 **Apache License 2.0** 许可证。

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

- **开发人员** : Duck, MLC, Z
- **产品策划** : Duck
- **项目组织** : [MagiciansWand](https://2015137.xyz)
- **支持机构** : [虚舟实验室](https://www.caellab.com/)

---

## 🙏 致谢

感谢以下开源项目和技术支持：

- [**SFML**](https://www.sfml-dev.org/) - 简单易用的多媒体库。
- [**nlohmann/json**](https://github.com/nlohmann/json) - 现代 C++ 的 JSON 库。
- [**MinGW-w64**](https://www.mingw-w64.org/) - Windows 上的卓越 GCC 环境。

---

## ⭐ Star History

如果这个项目对你有帮助，或者给你带来了乐趣，请为它点亮一颗 Star ⭐ ！这对我们很重要。

[![Star History Chart](https://api.star-history.com/svg?repos=MC-MCL/Student-Life-Simulator&type=Date)](https://star-history.com/#MC-MCL/Student-Life-Simulator&Date)

---

**祝你大学生活愉快！🎉**
```
