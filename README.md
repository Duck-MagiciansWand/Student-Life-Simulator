# 🎓 Student Life Simulator v3.2.2

### *一款基于 C++17 和 SFML 的校园生活叙事模拟游戏*

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)](https://www.sfml-dev.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-orange.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()
[![Version](https://img.shields.io/badge/Version-3.2.2-brightgreen.svg)]()
[![Status](https://img.shields.io/badge/Status-Stable-success.svg)]()

---

## 📖 项目简介

**Student Life Simulator** 是一款沉浸式的大学生活模拟游戏。玩家控制一个角色在可视化 2D 校园中自由探索、学习课程、社交互动、发展恋爱关系、收集鸭子、探索传送门，体验从大一新生到毕业的完整大学生涯。

**v3.2.2 版本（“鸭子纪元”更新）** 是迄今为止内容最丰富的版本，引入了完整的**鸭子生态模拟系统**——包括 16 种不同稀有度的鸭子、**鸭子繁殖系统**、**厕所传送门彩蛋**，以及一个特殊的 **MagiciansWand 一周年庆祝场景**。本次更新将游戏从“收集模拟器”升级为“生态模拟 + 彩蛋探索”的复合体验。

---

## ✨ v3.2.2 核心亮点

| 新增内容 | 说明 |
| :--- | :--- |
| 🦆 **16 种鸭子** | 分为普通（6只）、稀有（4只）、史诗（3只）、传说（3只）四个等级 |
| 🧬 **鸭子繁殖系统** | 收集到的鸭子可以配对繁殖，产生稀有后代！ |
| 🚽 **厕所传送门彩蛋** | 学校厕所会随机出现传送门，靠近按 `F` 进入隐藏区域 |
| 🎂 **一周年庆祝场景** | 传送门通往 MagiciansWand 一周年的神秘庆祝现场 |
| 🏆 **新成就** | “繁殖大师”、“传送门旅人”、“周年庆嘉宾”等 5 个新成就 |
| 🎨 **稀有度系统** | 鸭子分为普通/稀有/史诗/传说，不同颜色标识 |

---

## 🎯 核心特色一览

| 特色 | 说明 |
| :--- | :--- |
| 🗺️ **2D 校园地图** | 宿舍、图书馆、教室、食堂、健身房、湖泊等 6+ 个可交互场景 |
| 🦆 **鸭子收集与繁殖** | 16 种鸭子分布在校园各处，可配对繁殖产生新品种 |
| 🚽 **传送门系统** | 厕所随机出现传送门，按 `F` 进入隐藏区域 |
| 🎂 **周年庆场景** | 特殊传送门通往 MagiciansWand 一周年庆典现场 |
| 💕 **恋爱系统** | 与学姐、室友、导师三位 NPC 发展从相识到相恋的关系 |
| 🔍 **探索系统** | 发现秘密花园、旧书信、校园猫等 6 个隐藏内容 |
| 🎭 **情感状态** | 心情、孤独感、稳定性构成角色的情感画像 |
| 🎲 **随机事件系统** | 12+ 种随机事件，每次游玩体验独一无二 |
| 💬 **NPC 好感度** | 与 NPC 互动提升好感度，解锁特殊奖励 |
| 🏆 **技能与 Buff** | 考试成绩优秀可获得技能；临时增益助你冲刺 |
| 💾 **JSON 存档** | 存档文件人类可读，跨平台兼容 |
| 🎓 **毕业目标** | 完成 8 个学期、GPA ≥ 2.0、修满 20 门课程 |

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
│  DuckCollection (16种鸭子 + 繁殖系统)  |  PortalSystem (传送门) │
│  AnniversaryRoom (周年庆场景) |  SaveManager (JSON 存档)       │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                      建筑交互层 (多态)                           │
├──────────┬──────────┬──────────┬──────────┬──────────┬────────┤
│ Dormitory │ Library  │ Classroom│ Canteen  │   Gym    │  Lake  │
│  宿舍     │  图书馆   │  教室    │  食堂    │  健身房  │  湖泊  │
└──────────┴──────────┴──────────┴──────────┴──────────┴────────┘
```

### 技术栈

| 技术 | 版本 | 用途 |
| :--- | :--- | :--- |
| **C++** | C++17 | 核心开发语言 |
| **SFML** | 2.6+ | 图形渲染、窗口管理、事件处理 |
| **nlohmann/json** | 3.11+ | 游戏存档的序列化与反序列化 |
| **STL** | C++17 | 标准容器、算法、文件系统操作 |

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
| `F` | **传送门交互 / 恋爱面板** | v3.2 / v3.2.2 |
| `ESC` | 退出游戏 | v1.0 |

### 校园建筑

| 建筑 | 交互效果 | 可探索内容 |
| :--- | :--- | :--- |
| 🏠 **宿舍** | 精力 +40（有 Buff 时 +60） | 💌 旧书信 |
| 📚 **图书馆** | 提升指定课程的考试成绩 | 🌸 秘密花园、📖 秘密自习室 |
| 🏫 **教室** | 进入考试周，集中考核所有已修课程 | - |
| 🍜 **食堂** | 精力 +20，快乐 +8 | 🐱 校园猫 |
| 💪 **健身房** | 精力 +15，快乐 +10 | - |
| 🦆 **校园湖** | 靠近鸭子按 Enter 收集 | 16 种鸭子 |

---

## 🦆 鸭子系统详解 (v3.2.2)

### 16 种鸭子一览

#### 普通鸭子 (6只) — 基础收集
| 编号 | 名称 | 位置 |
| :--- | :--- | :--- |
| 001 | 🦆 小黄 | 湖中心 |
| 002 | 🦆 大花 | 湖西岸 |
| 003 | 🦆 斑斑 | 湖东岸 |
| 004 | 🦆 雪球 | 图书馆旁 |
| 005 | 🦆 黑豆 | 食堂后面 |
| 006 | 🦆 灰灰 | 宿舍楼下 |

#### 稀有鸭子 (4只) — 需要探索
| 编号 | 名称 | 位置 |
| :--- | :--- | :--- |
| 007 | 🦆 金毛 | 教学楼前 |
| 008 | 🦆 白鹭 | 湖边草丛 |
| 009 | 🦆 小橘 | 花园角落 |
| 010 | 🦆 鸭哥 | **隐藏**（收集 5 只后出现） |

#### 史诗鸭子 (3只) — 繁殖获得
| 编号 | 名称 | 获得方式 |
| :--- | :--- | :--- |
| 011 | 🌌 星河鸭 | 繁殖获得 |
| 012 | 🌈 极光鸭 | 繁殖获得 |
| 013 | 🤖 机械鸭 | 繁殖获得 |

#### 传说鸭子 (3只) — 稀有繁殖
| 编号 | 名称 | 获得方式 |
| :--- | :--- | :--- |
| 014 | 🌈 彩虹鸭 | 史诗×史诗繁殖（5%概率） |
| 015 | 👻 幽灵鸭 | 史诗×史诗繁殖（5%概率） |
| 016 | 🔥 凤凰鸭 | 史诗×史诗繁殖（5%概率） |

### 🧬 鸭子繁殖系统

收集到的鸭子可以配对繁殖，产生新品种！

| 亲本组合 | 后代稀有度概率 |
| :--- | :--- |
| 史诗 × 史诗 | 5% 传说 / 95% 史诗 |
| 史诗 × 稀有 | 30% 史诗 / 70% 稀有 |
| 稀有 × 稀有 | 10% 史诗 / 90% 稀有 |
| 普通 × 普通/稀有 | 20% 稀有 / 80% 普通 |

**繁殖条件**：
- 两只鸭子必须都已收集
- 不能是传说级鸭子
- 必须在彼此的“可繁殖列表”中
- 同一对鸭子不能无限繁殖

### 📊 收集进度追踪

按 `D` 键可随时查看鸭子收集进度：

```text
🦆 鸭子收集进度: 7/16
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
【普通】✅小黄 ✅大花 ✅斑斑 ✅雪球 ✅黑豆 ✅灰灰
【稀有】✅金毛 ✅白鹭 ⬜小橘 ⬜鸭哥
【史诗】⬜星河鸭 ⬜极光鸭 ⬜机械鸭
【传说】⬜彩虹鸭 ⬜幽灵鸭 ⬜凤凰鸭
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
💡 提示: 收集 5 只鸭子后，鸭哥会出现
💡 提示: 收集史诗鸭子后，可以尝试繁殖！
```

---

## 🚽 传送门系统 (v3.2.2)

### 厕所传送门

学校厕所会**随机出现**神秘的传送门！

| 属性 | 说明 |
| :--- | :--- |
| **出现规律** | 每 30-60 秒在厕所区域随机出现 |
| **持续时长** | 出现后持续 10 秒 |
| **交互方式** | 靠近传送门按 `F` 键进入 |
| **奖励** | 随机获得金钱、心情提升或精力恢复 |

### 🎂 周年庆传送门

收集 **10 只鸭子** 后，会激活一个特殊的周年庆传送门！

| 属性 | 说明 |
| :--- | :--- |
| **位置** | 校园中央区域 |
| **交互方式** | 靠近按 `F` 键进入 |
| **场景内容** | MagiciansWand 一周年庆典现场 |
| **奖励** | 500 元礼金 + “周年庆嘉宾”成就 |

### 🥚 彩蛋系统

| 彩蛋 | 触发方式 | 效果 |
| :--- | :--- | :--- |
| 🚽 厕所传送门 | 厕所区域随机出现，按 F 进入 | 随机奖励 |
| 🎂 周年庆场景 | 收集 10 只鸭子后激活传送门 | 500元 + 成就 |
| 🦆 鸭哥的礼物 | 和鸭哥对话 5 次 | 获得金色羽毛 |
| 🌈 彩虹鸭 | 繁殖获得彩虹鸭 | 视觉效果彩蛋 |

---

## 🏆 成就系统 (v3.2.2 新增)

### v3.2.2 新增成就

| 成就 | 条件 | 奖励 |
| :--- | :--- | :--- |
| 🧬 繁殖大师 | 成功繁殖 5 次 | 获得“养鸭专家”称号 |
| 🚽 传送门旅人 | 通过任意传送门 | 获得“时空旅人”称号 |
| 🎂 周年庆嘉宾 | 参加 MagiciansWand 一周年庆典 | 500 元礼金 |
| 🏆 鸭王之王 | 收集全部 16 只鸭子 | 金色鸭王头像 |

### 完整成就列表 (15+ 个)

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
| 🚽 传送门旅人 | 通过任意传送门 |
| 🎂 周年庆嘉宾 | 参加周年庆场景 |
| 💕 初恋 | 开始一段恋爱关系 |
| 🔍 探索者 | 发现 3 个校园秘密 |
| 🗺️ 探索大师 | 发现所有校园秘密 |

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
│   ├── campus.png
│   ├── ducks.png            # 鸭子系统截图
│   └── portal.png           # 传送门截图
└── saves/                   # (自动创建) 游戏存档目录
    └── autosave.json
```

---

## 📝 更新日志

### v3.2.2 (2026-07-04) - 鸭子纪元更新
- 🦆 **新增 16 种鸭子**：普通/稀有/史诗/传说四个等级
- 🧬 **新增鸭子繁殖系统**：收集到的鸭子可以配对繁殖
- 🚽 **新增厕所传送门彩蛋**：随机出现，按 F 进入
- 🎂 **新增周年庆场景**：MagiciansWand 一周年庆祝现场
- 🏆 **新增成就**：繁殖大师、传送门旅人、周年庆嘉宾
- 🎨 **UI 优化**：稀有度颜色标识，繁殖界面

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

### v2.0.0 (2026-06-26) - 图形化版本
- ✅ 基于 SFML 的 2D 图形界面

### v1.0.0 (2026-06-26) - 初始版本
- ✅ 控制台版本

---

## 🔧 开发路线图

| 版本 | 计划内容 | 预计时间 |
| :--- | :--- | :--- |
| v3.2.3 | 更多鸭子种类、鸭子竞赛 | 2026-07-10 |
| v3.3.0 | 季节系统、天气系统 | 2026-07-20 |
| v4.0.0 | 多人联机模式 | 2026-08-15 |

---

## 🤝 如何贡献

我们欢迎任何形式的贡献！

1. **Fork** 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开一个 **Pull Request**

### 代码风格指引
- **类名**：驼峰命名法
- **函数名**：驼峰命名法
- **变量名**：下划线命名法
- **常量**：全大写
- **注释**：为复杂逻辑添加清晰注释

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

| 角色 | 成员 |
| :--- | :--- |
| **开发人员** | Duck, MLC, Z |
| **产品策划** | Duck |
| **项目组织** | [MagiciansWand](https://magicianswand.cn) |
| **支持机构** | [虚舟实验室](https://xuzhou-lab.com) |

---

## 🙏 致谢

- [**SFML**](https://www.sfml-dev.org/) - 2D 图形库
- [**nlohmann/json**](https://github.com/nlohmann/json) - JSON 库
- [**MinGW-w64**](https://www.mingw-w64.org/) - Windows 编译环境
- 所有贡献者、测试者和玩家

---

## ⭐ Star History

如果这个项目给你带来了乐趣，请点亮右上角的 Star ⭐ ！

[![Star History Chart](https://api.star-history.com/svg?repos=Duck-MagiciansWand/Student-Life-Simulator&type=Date)](https://star-history.com/#Duck-MagiciansWand/Student-Life-Simulator&Date)

---

**祝你大学生活愉快！🦆🎉**
