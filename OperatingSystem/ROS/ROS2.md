# ROS2

--- Chime Lee

## 一、概述

`ROS2`：（Robot Operating System 2）机器人软件开发框架，解决ROS1的痛点：**去中心化（‘Master’节点去掉，使用DDS通信，适合机器人实时控制）**、**无单点故障**、**支持实时控制**、**支持跨平台（Win/Linux/嵌入式）**

**ROS1与ROS2核心区别:**

| 维度         | ROS1                  | ROS2                                            |
| :----------- | :-------------------- | :---------------------------------------------- |
| 编译工具     | `catkin_make`         | `colcon build`                                  |
| 通信架构     | 中心化（依赖 Master） | 去中心化（DDS）                                 |
| 实时性       | 弱（无原生实时支持）  | 强（支持 ROS2 Real-Time，适配嵌入式）           |
| 通信机制     | Topic/Service/Action  | 兼容原有机制，新增 DataWriter/DataReader（DDS） |
| 开发语言支持 | C++/Python 为主       | 多语言（C++/Python/Java）+ 嵌入式适配（STM32）  |

