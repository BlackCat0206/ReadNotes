# ReadNotes

---- *Chime Lee*

***



<div align="center">
  📝 系统化技术学习笔记仓库 | 嵌入式开发 · 编程语言 · 算法数据结构 · 操作系统
</div>

## 🔥 仓库简介
本仓库是个人技术学习的沉淀与整理，聚焦嵌入式开发、C/C++ 核心编程、算法数据结构、系统开发等方向，按模块化分类梳理知识点，力求内容实用、结构清晰、体系化，可作为：
- 嵌入式/后端开发入门的学习指南
- 技术知识点速查手册
- 算法刷题、项目开发的参考笔记

## 📂 目录结构

```BASH
./ReadNotes/
├── CMake
│   └── CMake.md
├── DataStruct	# 数据结构与算法
│   ├── Array
│   ├── Backtracking
│   ├── BinaryTree
│   ├── Docs
│   ├── DoublePointers
│   ├── DynamicProgramming
│   ├── Graph_Theory
│   ├── Greedy
│   ├── Hash
│   ├── LinkedList
│   ├── Skills
│   ├── Sort
│   ├── Stack_And_Queue
│   └── String
├── Embedded	# 嵌入式开发核心
│   ├── Docs
│   ├── GIC
│   ├── GPIO
│   ├── I2C
│   ├── SPI
│   └── UART
├── Languages
│   ├── C   # 《C Primer Plus》、《Pointer On C》、《C Traps and Pitfalls》
│   ├── C++ # 《C++ Primer》
│   ├── Python
│   └── STL
├── 🤖 LLM/ # 大语言模型相关笔记
│   └── LLM.md
├── OperatingSystem # 操作系统核心
│   ├── FreeRTOS    # 嵌入式 RTOS（任务 / 调度 / 中断）
│   └── Linux       # Linux 系统（命令 / 驱动 / 应用开发）
└── Tips	# 进阶技术技巧
    ├── C++_Something.md
    └── C++_设计模式.md

```

## 📌 核心内容速览
| 模块分类       | 核心覆盖知识点                                               |
| -------------- | ------------------------------------------------------------ |
| 操作系统       | Linux命令、驱动开发、进程/线程管理; FreeRTOS任务管理、中断处理； |
| 编程语言       | C/C++语法、内存管理、STL容器/算法；Python脚本开发            |
| 嵌入式开发     | 外设驱动（GPIO/I2C/SPI/UART）、ARM GIC中断、嵌入式调试技巧   |
| 数据结构与算法 | 基础数据结构、回溯/DP/贪心等算法思想、刷题技巧、复杂度分析   |
| 进阶技巧       | C++高级特性、设计模式（单例/工厂/观察者等）、工程化开发经验  |

## 🎯 适用人群
- 嵌入式开发入门/进阶工程师
- C/C++ 程序员（补充算法、系统、设计模式知识）
- 算法学习爱好者（聚焦编程题实战）
- 高校计算机/电子类学生、初入职场的技术新人

## 🚀 使用方式
### 1. 克隆仓库
```bash
git clone https://github.com/你的用户名/ReadNotes.git
cd ReadNotes
```

### 2. 查阅笔记

直接打开对应目录下的 `.md` 文件阅读，推荐使用支持 Markdown 的编辑器（如 VS Code、Typora），体验更佳。

### 3. 个性化补充

基于本仓库结构，添加自己的学习笔记，形成专属的技术知识库。

## 🤝 贡献说明

本仓库为个人学习笔记，欢迎交流与共建：

- 提交 Issue：指出笔记错误、补充待完善的知识点
- 提交 PR：补充优质笔记、优化目录结构或内容描述
- 技术交流：探讨嵌入式、算法、C/C++ 相关学习思路

## 📄 许可证

本仓库采用 [MIT 许可证](LICENSE)，你可以自由使用、修改、分发本仓库内容，只需注明来源即可。

## ⚠️ 免责声明

仓库内容为个人学习总结，可能存在疏漏或理解偏差，仅供学习参考，请勿直接用于生产环境。

### 定制化调整建议
1. 替换仓库地址：将 `https://github.com/你的用户名/ReadNotes.git` 改为实际的仓库 URL；
2. 补充 LICENSE 文件：若需要开源，可在仓库根目录添加 MIT 许可证文件（网上可直接复制模板）；
3. 精简/扩展内容：如果某模块（如 LLM）内容较少，可简化描述；若有新增模块，直接在目录结构和表格中补充；
4. 增加徽章（可选）：如需美化，可在开头添加仓库徽章，示例：
   ```markdown
   [![Stars](https://img.shields.io/github/stars/你的用户名/ReadNotes)](https://github.com/你的用户名/ReadNotes/stargazers)
   [![License](https://img.shields.io/github/license/你的用户名/ReadNotes)](LICENSE)