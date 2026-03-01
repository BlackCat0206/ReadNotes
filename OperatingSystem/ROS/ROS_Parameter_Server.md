# ROS_Parameter_Server

---- Chime Lee

## 一、概述

ROS参数服务器：**存储和共享全局配置参数的核心组件**，本质是一个键值对（key-value）存储系统，运行在ROS Master上，支持多节点读写，动态更新，常用于存储机器人参数（如尺寸、传感器定值）、算法阈值、路径配置等全局信息。

**核心概念：**

1.**数据类型**：支持常见类型（int/float/bool/string/list/dictionary）

2.**访问权限**：

* 全局参数：`/param_name` （所有节点可见）
* 私有参数：`~param_name` (仅当前节点可见，自动拼接节点命名空间)
* 命名空间参数：`/ns/param_name` （指定命名空间）

3.**核心特征**：

* 非实时：不适合高频更新参数（高频可以用topic）
* 可动态修改：运行时可更新参数，节点可监听参数变化
* 持久化：通过YAML文件加载/保存参数

## 二、命令行操作

### 1. 基础操作（增删改查）

| 命令                         | 功能         | 示例                                                         |
| ---------------------------- | ------------ | ------------------------------------------------------------ |
| `rosparam set <key> <value>` | 设置参数     | `rosparam set /max_speed 5`（全局）`rosparam set ~use_imu true`（节点私有，需先启动节点） |
| `rosparam get <key>`         | 获取参数     | `rosparam get /max_speed`（返回 5）`rosparam get /`（获取所有参数） |
| `rosparam delete <key>`      | 删除参数     | `rosparam delete /max_speed`                                 |
| `rosparam list`              | 列出所有参数 | `rosparam list`（查看当前参数服务器中的所有键）              |

### 2. YAML 文件加载 / 保存（批量管理参数）

#### （1）创建 YAML 文件（如`robot_params.yaml`）

```yaml
# 全局参数
/max_speed: 5.0
/wheel_radius: 0.15
/use_imu: true
/joint_names: ["joint1", "joint2", "joint3"]
/robot_pose: {x: 0.0, y: 0.0, theta: 0.0}

# 命名空间参数（对应节点ns）
/control/kp: 1.2
/control/ki: 0.1
/control/kd: 0.05
```

#### （2）加载 YAML 到参数服务器

```bash
rosparam load robot_params.yaml  # 加载所有参数
# 或指定命名空间加载：rosparam load robot_params.yaml /my_robot
```

#### （3）保存参数服务器所有参数到 YAML

```bash
rosparam dump all_params.yaml  # 保存所有参数到文件
rosparam dump control_params.yaml /control  # 仅保存/control命名空间下的参数
```

## 三、操作

### 3.1前置准备

1. 确保 ROS 环境已安装并配置：

   ```bash
   # 验证ROS环境
   roscore
   ```

   若提示

   ```
   roscore: command not found
   ```

   ，需先安装 ROS 并执行：

   ```bash
   source /opt/ros/noetic/setup.bash  # Noetic版本，Melodic替换为melodic
   ```

2. 已有 ROS 工作空间（如`catkin_ws`），若未创建：

   ```bash
   # 创建工作空间
   mkdir -p ~/catkin_ws/src
   cd ~/catkin_ws/
   catkin_make
   source devel/setup.bash
   ```

### 步骤 1：创建 ROS 功能包

进入工作空间的`src`目录，创建名为`param_cpp_demo`的功能包，依赖`roscpp`和`std_msgs`：

```bash
cd ~/catkin_ws/src
catkin_create_pkg param_cpp_demo roscpp std_msgs
```

创建后目录结构：

```plaintext
catkin_ws/src/param_cpp_demo/
├── CMakeLists.txt
├── include/param_cpp_demo/
├── package.xml
└── src/
```

### 步骤 2：创建参数配置文件（YAML）

在功能包下新建`config`目录，用于存放参数配置文件：

```bash
cd ~/catkin_ws/src/param_cpp_demo
mkdir config
touch config/robot_params.yaml
```

编辑`robot_params.yaml`（批量定义机器人参数）：

```yaml
# 全局命名空间参数 - 机器人基础配置
/robot/base/wheel_radius: 0.15    # 轮子半径（单位：m）
/robot/base/max_speed: 1.0        # 最大线速度（单位：m/s）
/robot/base/use_imu: true         # 是否启用IMU传感器
/robot/base/joint_names: ["left_wheel", "right_wheel"]  # 车轮关节名

# 命名空间参数 - PID控制配置
/robot/control/kp: 2.0           # 比例系数
/robot/control/ki: 0.1           # 积分系数
/robot/control/kd: 0.05          # 微分系数
```

### 步骤 3：创建启动文件（launch）

在功能包下新建`launch`目录，创建启动文件：

```bash
mkdir launch
touch launch/param_demo.launch
```

编辑`param_demo.launch`（一键启动 ROS 核心 + 加载参数 + 启动节点）：

```xml
<launch>
    <!-- 启动ROS核心（若已启动则自动复用） -->
    <node name="roscore" pkg="rosmaster" type="roscore" output="screen" />

    <!-- 从YAML文件加载参数到参数服务器 -->
    <rosparam command="load" file="$(find param_cpp_demo)/config/robot_params.yaml" />

    <!-- 启动参数操作节点 -->
    <node name="robot_param_node" 
          pkg="param_cpp_demo" 
          type="robot_param_node" 
          output="screen"  <!-- 终端打印日志 -->
          respawn="false"  <!-- 节点崩溃不自动重启 -->
          required="true"/> <!-- 节点退出则整个launch退出 -->
</launch>
```

### 步骤 4：编写 C++ 核心代码

编辑`src/param_demo.cpp`文件（实现参数全操作）：

```cpp
#include <ros/ros.h>
#include <vector>
#include <string>
// 仅保留核心头文件，移除所有参数事件相关头文件
#include <ros/param.h>

// 全局变量：存储需要监听的参数值（用于对比变化）
double g_max_speed = 0.0;
double g_kp = 0.0;

/**
 * @brief 定时检查参数变化（替代事件监听，兼容所有ROS1版本）
 * @param nh ROS节点句柄
 */
void checkParamChange(const ros::NodeHandle& nh) {
    // 获取当前参数值
    double current_max_speed = nh.param("/robot/base/max_speed", 0.0);
    double current_kp = nh.param("/robot/control/kp", 0.0);

    // 检查最大速度是否变化
    if (fabs(current_max_speed - g_max_speed) > 1e-6) {
        ROS_WARN_STREAM("\n[Parameter Change Notification]");
        ROS_WARN_STREAM("Parameter: /robot/base/max_speed");
        ROS_WARN_STREAM("Old value: " << g_max_speed);
        ROS_WARN_STREAM("New value: " << current_max_speed);
        g_max_speed = current_max_speed;  // 更新全局值
    }

    // 检查PID Kp是否变化
    if (fabs(current_kp - g_kp) > 1e-6) {
        ROS_WARN_STREAM("\n[Parameter Change Notification]");
        ROS_WARN_STREAM("Parameter: /robot/control/kp");
        ROS_WARN_STREAM("Old value: " << g_kp);
        ROS_WARN_STREAM("New value: " << current_kp);
        g_kp = current_kp;  // 更新全局值
    }
}

int main(int argc, char** argv) {
    // ===================== 1. 初始化ROS节点 =====================
    ros::init(argc, argv, "robot_param_node");
    ros::NodeHandle nh;
    ros::NodeHandle nh_private("~");

    // ===================== 2. 补充设置参数 =====================
    nh.setParam("/robot/base/max_angular", 0.5);  // 最大角速度
    nh_private.setParam("debug_mode", true);      // 私有参数：调试模式

    // ===================== 3. 获取参数 =====================
    double wheel_radius, max_speed, max_angular;
    bool use_imu, debug_mode;
    std::vector<std::string> joint_names;
    double ki, kd;

    // 方式1：直接获取必选参数
    nh.getParam("/robot/base/wheel_radius", wheel_radius);
    nh.getParam("/robot/base/max_speed", max_speed);
    nh.getParam("/robot/base/use_imu", use_imu);
    nh.getParam("/robot/base/joint_names", joint_names);
    
    // 方式2：带默认值获取
    nh.param("/robot/base/max_angular", max_angular, 0.3);
    nh.param("/robot/control/kp", g_kp, 1.0);  // 初始化监听的KP值
    nh.param("/robot/control/ki", ki, 0.0);
    nh.param("/robot/control/kd", kd, 0.0);
    
    // 私有参数
    nh_private.param("debug_mode", debug_mode, false);

    // 初始化监听的最大速度值
    g_max_speed = max_speed;

    // ===================== 4. 打印参数 =====================
    ROS_INFO("\n===================== Robot Parameter List =====================");
    ROS_INFO("Wheel radius: %.2f m", wheel_radius);
    ROS_INFO("Max linear speed: %.2f m/s", max_speed);
    ROS_INFO("Max angular speed: %.2f rad/s", max_angular);
    ROS_INFO("IMU enabled: %s", use_imu ? "Yes" : "No");
    ROS_INFO("Wheel joint names: %s, %s", joint_names[0].c_str(), joint_names[1].c_str());
    ROS_INFO("PID params: kp=%.1f, ki=%.1f, kd=%.2f", g_kp, ki, kd);
    ROS_INFO("Debug mode: %s", debug_mode ? "ON" : "OFF");
    ROS_INFO("===============================================================\n");

    // ===================== 5. 检查参数是否存在 =====================
    if (nh.hasParam("/robot/base/max_speed")) {
        ROS_INFO("✅ Parameter /robot/base/max_speed exists");
    } else {
        ROS_ERROR("❌ Parameter /robot/base/max_speed does not exist!");
    }

    // ===================== 6. 修改参数 =====================
    ROS_INFO("\n[Modify Parameter] Change max linear speed from %.2f m/s to 1.5 m/s", max_speed);
    nh.setParam("/robot/base/max_speed", 1.5);
    nh.getParam("/robot/base/max_speed", max_speed);
    g_max_speed = max_speed;  // 更新监听的全局值
    ROS_INFO("Max linear speed after modification: %.2f m/s", max_speed);

    // ===================== 7. 启动参数监听（定时轮询版，兼容所有ROS1） =====================
    ros::Rate rate(10);  // 轮询频率：10Hz
    ROS_INFO("\n[Parameter Monitoring Started]");
    ROS_INFO("You can test by running the following commands in terminal:");
    ROS_INFO("rosparam set /robot/base/max_speed 2.0");
    ROS_INFO("rosparam set /robot/control/kp 3.0\n");

    // ===================== 8. 循环运行（监听参数+保持节点存活） =====================
    ROS_INFO("Node started, press Ctrl+C to exit...\n");
    while (ros::ok()) {
        checkParamChange(nh);  // 检查参数变化
        ros::spinOnce();       // 处理回调（若有其他订阅）
        rate.sleep();          // 按频率休眠
    }

    return 0;
}
```

### 步骤 5：修改编译配置（CMakeLists.txt）

替换`param_cpp_demo/CMakeLists.txt`原有内容为以下代码：

```cmake
cmake_minimum_required(VERSION 3.0.2)
project(param_cpp_demo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)  # 编译模式：Release（优化）/ Debug（调试）

# 查找ROS依赖包
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

# 声明catkin包（供其他包依赖）
catkin_package(
  CATKIN_DEPENDS roscpp std_msgs
)

# 包含头文件目录
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# 编译C++节点
add_executable(robot_param_node src/param_demo.cpp)

# 链接ROS库
target_link_libraries(robot_param_node
  ${catkin_LIBRARIES}
)

# 安装可执行文件到ROS环境（可选，便于全局调用）
install(TARGETS robot_param_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# 安装launch和config文件（可选，便于roslaunch调用）
install(DIRECTORY launch/ config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
```

### 步骤 6：编译功能包

回到工作空间根目录，执行编译：

```bash
cd ~/catkin_ws
# 编译（若报错可加 -DCMAKE_BUILD_TYPE=Debug 调试）
catkin_make
# 刷新ROS环境变量（必须执行，否则找不到包）
source devel/setup.bash
```

### 步骤 7：运行实验

#### 方式 1：通过 launch 文件一键启动（推荐）

```bash
roslaunch param_cpp_demo param_demo.launch
```

#### 方式 2：分步启动（调试用）

```bash
# 终端1：启动ROS核心
roscore

# 终端2：加载YAML参数
rosparam load ~/catkin_ws/src/param_cpp_demo/config/robot_params.yaml

# 终端3：启动节点
rosrun param_cpp_demo robot_param_node
```

### 步骤 8：验证参数操作

#### 验证 1：查看参数列表

新开终端执行：

```bash
rosparam list
```

预期输出（关键参数）：

```plaintext
/robot/base/wheel_radius
/robot/base/max_speed
/robot/base/use_imu
/robot/control/kp
/robot_param_node/debug_mode
...
```

#### 验证 2：修改参数触发监听

新开终端执行：

```bash
# 修改最大线速度
rosparam set /robot/base/max_speed 2.0

# 修改PID参数
rosparam set /robot/control/kp 3.0
```

此时启动节点的终端会打印**参数变化通知**，验证监听功能生效。

#### 验证 3：获取参数值

```bash
# 获取单个参数
rosparam get /robot/base/max_speed
# 获取所有参数
rosparam get /
```

## 四、预期输出

启动节点后，终端输出示例：

```plaintext
[ INFO] [1734620000.000]: ===================== 机器人参数列表 =====================
[ INFO] [1734620000.001]: 轮子半径：0.15 m
[ INFO] [1734620000.001]: 最大线速度：1.00 m/s
[ INFO] [1734620000.001]: 最大角速度：0.50 rad/s
[ INFO] [1734620000.001]: 是否启用IMU：√
[ INFO] [1734620000.001]: 车轮关节名：left_wheel, right_wheel
[ INFO] [1734620000.001]: PID参数：kp=2.0, ki=0.1, kd=0.05
[ INFO] [1734620000.001]: 调试模式：开启
[ INFO] [1734620000.001]: ==========================================================

[ INFO] [1734620000.001]: ✅ 参数 /robot/base/max_speed 存在

[ INFO] [1734620000.001]: 【修改参数】将最大线速度从 1.00 m/s 改为 1.5 m/s
[ INFO] [1734620000.001]: 修改后最大线速度：1.50 m/s

[ INFO] [1734620000.001]: 【参数监听已启动】
[ INFO] [1734620000.001]: 可在终端执行以下命令测试：
[ INFO] [1734620000.001]: rosparam set /robot/base/max_speed 2.0
[ INFO] [1734620000.001]: rosparam set /robot/control/kp 3.0

[ INFO] [1734620000.001]: 节点已启动，按 Ctrl+C 退出...
```

执行`rosparam set /robot/base/max_speed 2.0`后，终端输出：

```plaintext
[ WARN] [1734620010.000]: 【参数变化通知】
[ WARN] [1734620010.000]: 参数名：/robot/base/max_speed
[ WARN] [1734620010.000]: 旧值：1.5
[ WARN] [1734620010.000]: 新值：2.0
```

## 五、关键知识点总结（文档重点）

| 操作类型     | 核心 API（C++）                          | 说明                                   |
| ------------ | ---------------------------------------- | -------------------------------------- |
| 初始化句柄   | `ros::NodeHandle nh` / `nh_private("~")` | 全局句柄操作`/xxx`，私有句柄操作`~xxx` |
| 设置参数     | `nh.setParam(key, value)`                | 支持 int/float/bool/vector 等类型      |
| 获取参数     | `nh.getParam(key, var)`                  | 参数不存在抛异常，适用于必选参数       |
| 安全获取参数 | `nh.param(key, var, default)`            | 参数不存在时使用默认值，推荐使用       |
| 检查参数存在 | `nh.hasParam(key)`                       | 返回 bool 值，判断参数是否存在         |
| 删除参数     | `nh.deleteParam(key)`                    | 谨慎使用，避免影响其他节点             |
| 监听参数变化 | 订阅`/parameter_events`话题              | 实时感知参数更新，适用于动态调参场景   |
| 批量加载参数 | `rosparam load xxx.yaml` /launch 加载    | 推荐用 YAML 管理大量参数，便于维护     |

## 六、常见问题与解决方案

| 问题现象               | 原因分析                        | 解决方案                                                     |
| ---------------------- | ------------------------------- | ------------------------------------------------------------ |
| 编译报错：找不到头文件 | 未配置`include_directories`     | 检查 CMakeLists.txt 中是否包含`${catkin_INCLUDE_DIRS}`       |
| 运行报错：找不到包     | 未执行`source devel/setup.bash` | 每次编译后必须执行，或添加到`~/.bashrc`：`echo "source ~/catkin_ws/devel/setup.bash" >> ~/.bashrc` |
| 参数监听不生效         | 未订阅`/parameter_events`       | 检查回调函数订阅代码，确保`ros::spin()`阻塞运行              |
| YAML 加载失败          | 文件路径错误                    | 使用`rospack find param_cpp_demo`验证路径，launch 中用`$(find 包名)`引用 |
| 私有参数路径异常       | 节点名重复                      | 启动节点时添加`anonymous=true`（如`ros::init(argc, argv, "node", ros::init_options::AnonymousName)`） |

## 七、扩展实验（可选）

1. **参数合法性校验**：在获取参数后添加校验（如最大速度不能为负）；
2. **Service 接口修改参数**：编写 Service 服务，通过请求修改参数（替代命令行）；
3. **dynamic_reconfigure 可视化调参**：集成`dynamic_reconfigure`包，实现 RViz / 命令行可视化调参；
4. **参数持久化**：将运行时修改的参数保存到 YAML 文件，实现重启后复用。

## 八、文档版本信息

| 版本 | 更新时间   | 更新内容               | 维护人   |
| ---- | ---------- | ---------------------- | -------- |
| V1.0 | 2025-12-19 | 初始版本，完成核心流程 | 文档专用 |