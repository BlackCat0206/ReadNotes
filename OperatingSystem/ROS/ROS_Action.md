# ROS_Action

---- Chime Lee

## 一、概述

`Action`是ROS为**异步、长时间、可中断、需要反馈（指实时进度）**的任务设计的通信机制。主要为弥补`Service`无法处理长任务、无反馈、不可取消的缺点。

1.核心概念：

* Action Server：执行长时间任务节点，支持接收目标，反馈进度，返回最终结果，响应客户端的取消请求。
* Action Client：发送任务目标，异步接收进度返回和最结果，也可以主动取消任务。
* 动作类型（`action`）：自定义的目标/反馈/结果数据结构，以`.action`文件定义（分为`goal`，`feedback`，`result`三部分，用`---`分隔）。

2.工作流程：

* 动作服务端启动，注册动作名（如`/move_base`）到ROS Master
* 客户端发送目标（如机器人移动到某坐标）。
* 服务端开始执行任务，周期性发送进度返回（如已移动50%）。
* 客户端可随时取消任务，或等待任务完成后接收最终结果（如移动成功/失败）；
* 任务结果，通信终止（若有新目标可重新发起）。

3.适用场景：

* 长时间运行的任务：如机器人导航，机械臂运动规划。
* 需要进度反馈的场景：机器人移动进度，打印任务完成百分比。
* 可中断任务：如用户随时取消机器人的移动指令。

4.个人理解：

* `topic`：适合获取长时间数据：实时图像，实时坐标等。
* `service`：属性查询，参数设置，状态查询等。
* `action`：导航，运动规划，`OTA`升级等

## 二、具体操作

### 步骤 1：创建 ROS 功能包

首先创建包含 Action 依赖的功能包（依赖：`actionlib`、`actionlib_msgs`、`roscpp`）：

```bash
# 进入工作空间的src目录（假设你的工作空间是catkin_ws）
cd ~/catkin_ws/src
# 创建功能包，命名为action_demo
catkin_create_pkg action_demo roscpp actionlib actionlib_msgs
cd action_demo
# 创建action目录，用于存放.action消息定义文件
mkdir action
```

### 三、步骤 2：定义 Action 消息（.action 文件）

在`action`目录下创建自定义 Action 消息文件（例：`DoDishes.action`，模拟 “洗碗” 任务）：

```bash
touch action/DoDishes.action
```

编辑该文件，写入以下内容（Goal = 目标，Feedback = 实时反馈，Result = 最终结果）：

```plaintext
# 目标请求：要洗的碗数量
uint32 dishes_to_wash
---
# 执行结果：已洗的碗数量
uint32 dishes_washed
---
# 实时反馈：当前洗到第几个碗
uint32 percent_complete
```

### 四、步骤 3：配置 CMakeLists.txt 和 package.xml

Action 消息需要编译生成对应的 C++ 头文件，因此需修改编译配置：

#### 1. 修改 package.xml（添加依赖）

```xml
<build_depend>actionlib_msgs</build_depend>
<exec_depend>actionlib_msgs</exec_depend>
<build_depend>actionlib</build_depend>
<exec_depend>actionlib</exec_depend>

<!-- 若使用ROS Noetic，需添加： -->
<buildtool_depend>catkin</buildtool_depend>
```

#### 2. 修改 CMakeLists.txt（关键！）

在`CMakeLists.txt`中添加以下配置（按顺序，不要遗漏）：

```cmake
# 1. 查找actionlib_msgs依赖（一般都会默认生成好了）
find_package(catkin REQUIRED COMPONENTS
  roscpp
  actionlib
  actionlib_msgs
)

# 2. 生成Action消息（必须在catkin_package之前）
add_action_files(
  FILES
  DoDishes.action  # 对应你创建的.action文件
)

# 3. 生成消息所需的依赖
generate_messages(
  DEPENDENCIES
  actionlib_msgs
  std_msgs  # 若用到其他基础消息，可添加
)

# 4. 声明catkin包
catkin_package(
  CATKIN_DEPENDS roscpp actionlib actionlib_msgs
)

# 5. 编译Action Server和Client节点
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# 编译Server节点
add_executable(dishwasher_server src/dishwasher_server.cpp)
target_link_libraries(dishwasher_server ${catkin_LIBRARIES})
# 确保先编译Action消息，再编译节点
add_dependencies(dishwasher_server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# 编译Client节点
add_executable(dishwasher_client src/dishwasher_client.cpp)
target_link_libraries(dishwasher_client ${catkin_LIBRARIES})
add_dependencies(dishwasher_client ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
```

### 五、步骤 4：实现 Action Server（C++）

在`src`目录下创建`dishwasher_server.cpp`，实现洗碗任务的服务端：

```cpp
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
// 导入自动生成的Action消息头文件（命名规则：功能包名/Action文件名Action.h）
#include <action_demo/DoDishesAction.h>

// 定义SimpleActionServer类型别名
typedef actionlib::SimpleActionServer<action_demo::DoDishesAction> Server;

// 收到Goal后的回调函数（执行具体任务）
void execute(const action_demo::DoDishesGoalConstPtr& goal, Server* as)
{
    // 初始化反馈和结果
    action_demo::DoDishesFeedback feedback;
    action_demo::DoDishesResult result;

    // 获取目标洗碗数量
    int total_dishes = goal->dishes_to_wash;
    int washed_dishes = 0;

    // 设置循环频率（1Hz，模拟每秒洗1个碗）
    ros::Rate rate(1);
    bool success = true;

    ROS_INFO("开始洗碗，总共需要洗 %d 个碗", total_dishes);

    // 执行洗碗任务
    while (washed_dishes < total_dishes)
    {
        // 检查是否被取消
        if (as->isPreemptRequested() || !ros::ok())
        {
            ROS_WARN("洗碗任务被取消！");
            as->setPreempted();  // 标记任务被取消
            success = false;
            break;
        }

        // 模拟洗碗（洗1个碗）
        washed_dishes++;
        // 设置反馈（进度百分比）
        feedback.percent_complete = (washed_dishes * 100) / total_dishes;
        as->publishFeedback(feedback);  // 发布反馈

        ROS_INFO("已洗 %d 个碗，进度：%d%%", washed_dishes, feedback.percent_complete);
        rate.sleep();
    }

    // 任务完成
    if (success)
    {
        result.dishes_washed = washed_dishes;
        ROS_INFO("洗碗任务完成！总共洗了 %d 个碗", result.dishes_washed);
        as->setSucceeded(result);  // 标记任务成功，返回结果
    }
}

int main(int argc, char** argv)
{
    // 初始化节点
    ros::init(argc, argv, "dishwasher_server");
    ros::NodeHandle nh;

    // 创建Action Server（参数：节点句柄、Action名称、回调函数、自动启动？）
    Server server(nh, "do_dishes", boost::bind(&execute, _1, &server), false);
    server.start();  // 启动Server

    ROS_INFO("洗碗服务器已启动，等待客户端请求...");
    ros::spin();  // 循环等待回调

    return 0;
}
```

### 六、步骤 5：实现 Action Client（C++）

在`src`目录下创建`dishwasher_client.cpp`，实现客户端发送任务请求：

```cpp
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
// 导入Action消息头文件
#include <action_demo/DoDishesAction.h>

// 反馈回调函数（接收实时进度）
void feedbackCallback(const action_demo::DoDishesFeedbackConstPtr& feedback)
{
    ROS_INFO("收到反馈：洗碗进度 %d%%", feedback->percent_complete);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "dishwasher_client");

    // 创建Action Client（参数：Action名称、是否自动启动线程）
    actionlib::SimpleActionClient<action_demo::DoDishesAction> ac("do_dishes", true);

    ROS_INFO("等待洗碗服务器启动...");
    ac.waitForServer();  // 等待Server上线

    ROS_INFO("服务器已连接，发送洗碗任务目标！");
    // 创建Goal并设置目标洗碗数量
    action_demo::DoDishesGoal goal;
    goal.dishes_to_wash = 10;  // 要求洗10个碗

    // 发送Goal，并注册反馈回调、完成回调
    ac.sendGoal(goal,
                // 完成回调（任务结束时触发）
                [](const actionlib::SimpleClientGoalState& state, const action_demo::DoDishesResultConstPtr& result) {
                    ROS_INFO("任务完成状态：%s", state.toString().c_str());
                    ROS_INFO("最终结果：总共洗了 %d 个碗", result->dishes_washed);
                },
                // 激活回调（任务开始时触发）
                []() { ROS_INFO("任务已激活，开始执行！"); },
                // 反馈回调（接收实时进度）
                &feedbackCallback);

    // 等待任务完成（超时时间15秒）
    bool finished_before_timeout = ac.waitForResult(ros::Duration(15.0));

    if (finished_before_timeout)
    {
        // 获取任务最终状态
        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("任务完成：%s", state.toString().c_str());
    }
    else
    {
        ROS_WARN("任务超时！");
        // 取消任务（可选）
        ac.cancelGoal();
    }

    return 0;
}
```

### 七、步骤 6：编译并运行

#### 1. 编译工作空间

```bash
cd ~/catkin_ws
catkin_make
# 刷新环境变量（必须！）
source devel/setup.bash
```

#### 2. 运行节点（需打开 3 个终端）

**终端 1：启动 roscore**

```bash
roscore
```

**终端 2：启动 Action Server**

```bash
source ~/catkin_ws/devel/setup.bash
rosrun action_demo dishwasher_server
```

**终端 3：启动 Action Client**

```bash
source ~/catkin_ws/devel/setup.bash
rosrun action_demo dishwasher_client
```

### 八、预期输出

#### Server 终端输出：

```plaintext
[INFO] [1734600000.000000]: 洗碗服务器已启动，等待客户端请求...
[INFO] [1734600005.000000]: 开始洗碗，总共需要洗 10 个碗
[INFO] [1734600006.000000]: 已洗 1 个碗，进度：10%
[INFO] [1734600007.000000]: 已洗 2 个碗，进度：20%
...
[INFO] [1734600015.000000]: 已洗 10 个碗，进度：100%
[INFO] [1734600015.000000]: 洗碗任务完成！总共洗了 10 个碗
```

#### Client 终端输出：

```plaintext
[INFO] [1734600000.000000]: 等待洗碗服务器启动...
[INFO] [1734600005.000000]: 服务器已连接，发送洗碗任务目标！
[INFO] [1734600005.000000]: 任务已激活，开始执行！
[INFO] [1734600006.000000]: 收到反馈：洗碗进度 10%
[INFO] [1734600007.000000]: 收到反馈：洗碗进度 20%
...
[INFO] [1734600015.000000]: 收到反馈：洗碗进度 100%
[INFO] [1734600015.000000]: 任务完成状态：SUCCEEDED
[INFO] [1734600015.000000]: 最终结果：总共洗了 10 个碗
[INFO] [1734600015.000000]: 任务完成：SUCCEEDED
```