# ROS_Communication

---- Chime Lee

## 一、概述

* 无人机侧使用协议封装与外界消息传输，内部通过对协议的反序列化，转换为ROS消息。
* 当前目标：实现`Ubuntu`下与无人机的通信。

## 二、编译

* 直接使用catkin_make生成`complie_commands.json`

```bash
# 先source ROS环境和本地工作空间
source /opt/ros/noetic/setup.bash
source ~/owl_client/devel/setup.bash

# 清理旧编译产物
catkin_make clean

# 生成compile_commands.json并编译
catkin_make -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

