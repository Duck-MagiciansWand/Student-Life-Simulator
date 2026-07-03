# 🎓 Student Life Simulator v3.2

### *一款基于 C++17 和 SFML 的校园生活叙事模拟游戏*

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)](https://www.sfml-dev.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-orange.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

---

## 📖 项目简介

**Student Life Simulator** 是一款沉浸式的大学生活模拟游戏。你控制一个角色在可视化的 2D 校园中探索、学习、社交、恋爱，体验从大一新生到毕业的完整大学生活。

**v3.2 版本**在故事与成就系统的基础上，进一步深化了**情感维度**和**世界丰富度**。现在，你可以与 NPC 建立从相识到相恋的羁绊，在校园里发现隐藏的秘密，并真正感受到每一次选择带来的情感回响。

### ✨ v3.2 核心亮点

| 新增系统 | 说明 |
| :--- | :--- |
| 💕 **恋爱系统** | 与 NPC 发展从“相识”到“相恋”的关系，解锁专属互动与剧情。 |
| 🔍 **探索系统** | 校园各处隐藏着 6 个可发现的“秘密”与“收藏品”，丰富世界观。 |
| 🎭 **情感状态** | 角色新增“心情”、“孤独感”与“情感稳定性”维度，影响游戏体验。 |
| 📖 **故事分支** | 关键剧情事件拥有 2-3 种不同结局，你的选择真正改变故事走向。 |
| 🏆 **新增成就** | 新增“初恋”、“心碎”、“探索者”等 5 个成就，总计 15+ 个。 |

### 🎯 核心特色

| 特色 | 说明 |
| :--- | :--- |
| 💕 **恋爱与羁绊** (v3.2) | 与 NPC（学姐/室友/导师）发展浪漫关系，体验青涩的校园恋情。 |
| 🔍 **探索与收藏** (v3.2) | 发现秘密花园、旧书信、校园猫等 6 个隐藏内容。 |
| 🎭 **情感状态** (v3.2) | 心情、孤独感、稳定性构成角色的情感画像，影响剧情走向。 |
| 🗺️ **2D 校园地图** | 宿舍、图书馆、教室、食堂、健身房等 5 个可交互场景。 |
| 🎲 **随机事件系统** | 12+ 种随机事件（奇遇、事故、机遇），每次游玩体验不同。 |
| 💬 **NPC 好感度** | 与 3 位 NPC 互动，好感度解锁特殊奖励与剧情。 |
| 🏆 **技能与 Buff** | 考试成绩优秀可获得技能；临时增益（如“学习效率+20%”）助你冲刺。 |
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
│  随机事件生成    │ NPC好感度/交互   │   成就解锁/查询             │
├─────────────────┴─────────────────┴─────────────────────────────┤
│  StorySystem (故事分支)  |  ExplorationSystem (v3.2 新增)      │
│  RelationshipSystem (v3.2 新增) |  SaveManager (JSON 存档)     │
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
git clone https://github.com/Duck-MagiciansWand/Student-Life-Simulator.git
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
| `A` | 打开成就面板 |
| `F` | 打开恋爱关系面板 (v3.2 新增) |
| `ESC` | 退出游戏 |

### 校园建筑

| 建筑 | 交互效果 | 可探索内容 |
| :--- | :--- | :--- |
| 🏠 **宿舍** | 精力 +40（有 Buff 时 +60） | 💌 旧书信 |
| 📚 **图书馆** | 提升指定课程的考试成绩 | 🌸 秘密花园、📖 秘密自习室 |
| 🏫 **教室** | 进入考试周，集中考核所有已修课程 | - |
| 🍜 **食堂** | 精力 +20，快乐 +8 | 🐱 校园猫 |
| 💪 **健身房** | 精力 +15，快乐 +10 | - |

### 🔍 探索系统 (v3.2 新增)

在校园中探索，发现隐藏的秘密：

| 发现物 | 位置 | 效果 |
| :--- | :--- | :--- |
| 🌸 秘密花园 | 图书馆后方 | 孤独感 -15，心情 → 平静 |
| 📜 旧书信 | 宿舍阁楼 | 快乐 +5，获得情感记忆 |
| 🐱 校园猫 | 食堂附近 | 孤独感 -10，快乐 +15 |
| 📦 时光胶囊 | 操场 | 快乐 +10，获得情感记忆 |
| 🌅 天台风景 | 教学楼天台 | 孤独感 -20，心情 → 平静 |
| 📖 秘密自习室 | 图书馆 | 快乐 +5，获得“学习效率+20%”Buff |

### 💕 恋爱系统 (v3.2 新增)

与 NPC 发展浪漫关系：

| 阶段 | 好感度要求 | 说明 |
| :--- | :--- | :--- |
| 陌生人 | 0-10 | 初次见面 |
| 认识 | 10-25 | 开始熟悉 |
| 朋友 | 25-40 | 建立友谊 |
| 好朋友 | 40-50 | 亲密朋友 |
| 暗恋 | 50-60 | 产生好感 |
| 恋爱中 💕 | 60-80 | 表白成功，开始恋爱 |
| 深爱 ❤️ | 80-100 | 深厚感情 |

**操作方式**：
1. 按 `F` 键打开恋爱面板
2. 选择 NPC 名字
3. 选择“表白”或“分手”

### 🎭 情感状态 (v3.2 新增)

| 状态 | 触发条件 | 影响 |
| :--- | :--- | :--- |
| 😊 快乐 | 幸福 > 80 | 学习效率 +5% |
| 😢 悲伤 | 幸福 < 30 | 学习效率 -10% |
| 🤩 兴奋 | 触发特殊事件 | 行动力 +10% |
| 😔 孤独 | 孤独感 > 70 | 幸福下降加速 |
| 🥰 被爱 | 恋爱中 | 幸福缓慢上升 |
| 😰 焦虑 | 稳定性 < 30 | 学习效率 -15% |
| 😌 平静 | 通过休息/探索获得 | 状态恢复加速 |

### NPC 好感度奖励

与 NPC 互动可提升好感度，解锁特殊奖励：

| NPC | 好感度阈值 | 奖励效果 |
| :--- | :--- | :--- |
| **学姐** | 30 | 获得“复习资料”：学习效率 +20% (持续 2 天) |
| | 60 | 获得“考试重点”：随机一门课程成绩提升 |
| | 80 | 获得“推荐信”：大幅提升毕业竞争力 |
| **室友** | 30 | “一起吃饭”：精力 +10，快乐 +15 |
| | 60 | “组队学习”：学习效率 +20% (持续 1 天) |
| | 80 | “互相监督”：学习效率 +20% (持续 3 天) |
| **导师** | 30 | “学术指导”：快乐 +10 |
| | 60 | “研究机会”：快乐 +15，收入 +200 |
| | 80 | “推荐信”：毕业竞争力大幅提升 |

### 🎓 毕业条件

要成功毕业，你需要达成以下目标：

1.  ✅ 完成 **8 个学期**
2.  ✅ 最终 GPA ≥ **2.0**
3.  ✅ 修满 **20 门课程**

---

## 🏆 成就系统

### v3.2 新增成就

| 成就 | 条件 |
| :--- | :--- |
| 💕 初恋 | 开始一段恋爱关系 |
| 💔 心碎 | 经历一次分手 |
| 🔍 探索者 | 发现 3 个校园秘密 |
| 🗺️ 探索大师 | 发现所有校园秘密 |
| 🌹 浪漫主义者 | 同时与 2 个 NPC 保持恋爱关系 |

### 完整成就列表 (15+ 个)

| 成就 | 条件 |
| :--- | :--- |
| 📚 初入学术 | 选修第一门课程 |
| 💪 学霸之路 | 一门课程考到 90 分以上 |
| 🎓 课程大师 | 完成所有课程 |
| 🌟 优秀毕业生 | GPA 达到 3.5 以上 |
| 🦋 社交达人 | 所有 NPC 好感度达到 60 以上 |
| 💰 小富翁 | 攒够 2000 元 |
| 🎲 事件收藏家 | 触发 10 次随机事件 |
| ⚡ 提前毕业 | 6 个学期内毕业 |
| 🎯 完美成绩 | 所有课程都在 85 分以上 |
| 💕 初恋 | 开始一段恋爱关系 |
| 💔 心碎 | 经历一次分手 |
| 🔍 探索者 | 发现 3 个校园秘密 |
| 🗺️ 探索大师 | 发现所有校园秘密 |
| 🌹 浪漫主义者 | 同时与 2 个 NPC 保持恋爱关系 |

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

### v3.2.0 (2026-07-03) - 恋爱与探索更新
- ✨ **新增恋爱系统**：与 NPC 发展从相识到相恋的关系
- ✨ **新增探索系统**：6 个隐藏秘密等待发现
- ✨ **新增情感状态**：心情、孤独感、稳定性三维情感系统
- ✨ **新增故事分支**：关键剧情拥有 2-3 种不同结局
- ✨ **新增成就**：初恋、心碎、探索者等 5 个新成就
- ✨ **新增快捷键 F**：打开恋爱关系面板
- 🐛 **修复**：若干 UI 显示与存档兼容性问题

### v3.1.0 (2026-06-28)
- ✨ 新增故事主线（5 个核心剧情事件）
- ✨ 新增成就系统（10+ 个成就）
- ✨ 增强 NPC 交互（更丰富的对话与奖励）

### v3.0.0 (2026-06-27)
- ✅ 引入随机事件、NPC、技能、Buff 系统
- ✅ 采用 JSON 格式进行存档
- ✅ 使用多态重构建筑交互

### v2.0.0 (2026-06-26)
- ✅ 基于 SFML 的 2D 图形界面
- ✅ 可视化校园地图与角色实时移动

### v1.0.0 (2026-06-26)
- ✅ 控制台版本，包含基础课程与考试系统

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
- **项目组织** : [MagiciansWand](https://magicianswand.cn)
- **支持机构** : [虚舟实验室](https://xuzhou-lab.com)

---

## 🙏 致谢

感谢以下开源项目和技术支持：

- [**SFML**](https://www.sfml-dev.org/) - 简单易用的多媒体库。
- [**nlohmann/json**](https://github.com/nlohmann/json) - 现代 C++ 的 JSON 库。
- [**MinGW-w64**](https://www.mingw-w64.org/) - Windows 上的卓越 GCC 环境。

---

## ⭐ Star History

如果这个项目对你有帮助，或者给你带来了乐趣，请为它点亮一颗 Star ⭐ ！

[![Star History Chart](https://api.star-history.com/svg?repos=Duck-MagiciansWand/Student-Life-Simulator&type=Date)](https://star-history.com/#Duck-MagiciansWand/Student-Life-Simulator&Date)

---

**祝你大学生活愉快！🎉**


---
