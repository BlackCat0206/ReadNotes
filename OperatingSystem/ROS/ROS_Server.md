# ROS_Server

***---- Chime Lee***

## 一、概述

ROS Service (服务) —— 同步短请求-响应

Service是ROS中基于**同步、点对点、短时间、一次性**的请求—响应通信模式，由Server和Client组成：

### 1.1 核心概念

* Server(服务端)：提供特定功能的节点，持续监听客户端的请求，收到请求后处理并返回响应。
* Client(客户端)：向服务端发送请求，阻塞等待（默认）服务端的响应，拿到结果后继续执行。
* 服务类型（*srv*）：自定义的请求/响应数据结构，以`.srv`文件定义（分为`request`和`response`两不服，用`---`分割）。

### 1.2 工作流程：

1. 服务端节点启动，注册一个服务名（如 `/add_two_ints`）到 ROS Master；
2. 客户端节点通过服务名向 ROS Master 查询服务端的地址，建立连接；
3. 客户端发送请求数据（如两个整数）；
4. 服务端处理请求（如求和），返回响应数据（如和值）；
5. 客户端收到响应后，阻塞解除，通信结束。

## 二、具体操作

### 步骤 1：创建 / 进入 ROS 工作空间

```bash
# 若已有工作空间（如catkin_ws），跳过此步，直接进入src目录
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace
cd ~/catkin_ws
catkin_make
source devel/setup.bash  # 关键：配置环境变量，每次新开终端需执行（或写入~/.bashrc）
```

### 步骤 2：创建功能包（带 C++ 依赖）

进入`src`目录，创建名为`cpp_service_demo`的功能包，依赖`roscpp`（C++ 核心库）、`std_msgs`、`message_generation`（生成 srv 文件）：

```bash
cd ~/catkin_ws/src
catkin_create_pkg cpp_service_demo roscpp std_msgs message_generation
cd cpp_service_demo  # 进入功能包目录
```

### 步骤 3：定义 Service 数据类型（.srv 文件）

#### 3.1 创建 srv 目录并编写.srv 文件

```bash
mkdir srv  # 功能包下创建srv目录
touch srv/AddTwoInts.srv  # 自定义请求-响应结构
```

编辑`AddTwoInts.srv`（用`vim`/`nano`/VSCode）：

```srv
# 请求部分：两个整数a和b
int64 a
int64 b
---  # 分隔符：上=request，下=response
# 响应部分：求和结果sum
int64 sum
```

#### 3.2 配置 `CMakeLists.txt`（核心！C++ 编译关键）

ROS 需编译`.srv`文件生成 C++ 头文件，需修改功能包下的`CMakeLists.txt`，**精准找到对应位置**，取消注释并补充以下内容：

```cmake
# 1. 找到依赖（确保包含roscpp、message_generation）
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation  # 新增：生成srv消息
)

# 2. 声明要编译的srv文件
add_service_files(
  FILES
  AddTwoInts.srv  # 对应我们创建的srv文件
)

# 3. 生成消息依赖（基于std_msgs）
generate_messages(
  DEPENDENCIES
  std_msgs
)

# 4. 声明catkin导出配置（添加message_runtime）
catkin_package(
  CATKIN_DEPENDS roscpp std_msgs message_runtime  # 运行时依赖
  INCLUDE_DIRS include  # 若有自定义头文件需加，此处暂无需
  LIBRARIES cpp_service_demo  # 功能包名
)

# 5. 编译C++服务端代码（核心）
# 5.1 包含头文件目录（ROS核心头文件）
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# 5.2 生成可执行文件（服务端）
add_executable(add_two_ints_server src/add_two_ints_server.cpp)

# 5.3 链接依赖库（roscpp等），且确保先生成srv消息再编译可执行文件
add_dependencies(add_two_ints_server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(add_two_ints_server
  ${catkin_LIBRARIES}
)

# （可选）编译客户端代码（用于测试）
add_executable(add_two_ints_client src/add_two_ints_client.cpp)
add_dependencies(add_two_ints_client ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(add_two_ints_client
  ${catkin_LIBRARIES}
)
```

#### 3.3 配置 package.xml

添加编译 / 运行依赖，确保`<package>`标签内包含：

```xml
<?xml version="1.0"?>
<package format="2">
  <name>cpp_service_demo</name>
  <version>0.0.0</version>
  <description>The cpp_service_demo package</description>
  <maintainer email="your_name@example.com">Your Name</maintainer>
  <license>MIT</license>

  <buildtool_depend>catkin</buildtool_depend>
  <!-- 编译依赖 -->
  <build_depend>roscpp</build_depend>
  <build_depend>std_msgs</build_depend>
  <build_depend>message_generation</build_depend>
  <!-- 运行依赖 -->
  <exec_depend>roscpp</exec_depend>
  <exec_depend>std_msgs</exec_depend>
  <exec_depend>message_runtime</exec_depend>

  <export>
  </export>
</package>
```

### 步骤 4：编写 C++ Service Server 代码

在功能包下的`src`目录（默认已存在）创建服务端代码文件：

```bash
touch src/add_two_ints_server.cpp
```

编辑`add_two_ints_server.cpp`：

```cpp
// 包含ROS核心头文件
#include "ros/ros.h"
// 包含自动生成的服务消息头文件（格式：功能包名/服务名.h）
#include "cpp_service_demo/AddTwoInts.h"

/**
 * @brief 服务请求处理函数
 * @param req 客户端发送的请求（包含a和b）
 * @param res 要返回给客户端的响应（包含sum）
 * @return bool 处理是否成功
 */
bool add(cpp_service_demo::AddTwoInts::Request  &req,
         cpp_service_demo::AddTwoInts::Response &res)
{
    // 计算求和结果
    res.sum = req.a + req.b;
    // 打印日志（ROS_INFO是C++版日志函数）
    ROS_INFO("收到客户端请求：a = %ld, b = %ld", (long int)req.a, (long int)req.b);
    ROS_INFO("返回响应：sum = %ld", (long int)res.sum);
    return true; // 返回true表示处理成功
}

int main(int argc, char **argv)
{
    // 1. 初始化ROS节点，命名为"add_two_ints_server"
    ros::init(argc, argv, "add_two_ints_server");
    // 2. 创建节点句柄（ROS节点的核心接口）
    ros::NodeHandle n;

    // 3. 注册Service：参数1=服务名（客户端通过此名称调用），参数2=请求处理函数
    ros::ServiceServer service = n.advertiseService("add_two_ints", add);
    ROS_INFO("C++版AddTwoInts服务已启动，等待客户端请求...");

    // 4. 阻塞监听请求（保持节点运行）
    ros::spin();

    return 0;
}
```

### 步骤 5：编写 C++ Client 代码（用于测试 Server）

创建客户端代码文件，验证服务端功能：

```bash
touch src/add_two_ints_client.cpp
```

编辑`add_two_ints_client.cpp`：

```cpp
#include "ros/ros.h"
#include "cpp_service_demo/AddTwoInts.h"
#include <cstdlib>  // 用于atoi（字符串转整数）

int main(int argc, char **argv)
{
    // 初始化客户端节点
    ros::init(argc, argv, "add_two_ints_client");
    
    // 检查命令行参数（需传入两个整数）
    if (argc != 3)
    {
        ROS_INFO("使用方法：rosrun cpp_service_demo add_two_ints_client 整数1 整数2");
        return 1;
    }

    ros::NodeHandle n;
    // 创建Service客户端：参数1=服务名，参数2=服务类型
    ros::ServiceClient client = n.serviceClient<cpp_service_demo::AddTwoInts>("add_two_ints");

    // 构造请求对象
    cpp_service_demo::AddTwoInts srv;
    srv.request.a = atoi(argv[1]);  // 命令行参数1转整数
    srv.request.b = atoi(argv[2]);  // 命令行参数2转整数

    // 调用服务（阻塞等待响应）
    if (client.call(srv))
    {
        ROS_INFO("求和结果：%ld", (long int)srv.response.sum);
    }
    else
    {
        ROS_ERROR("调用服务失败！");
        return 1;
    }

    return 0;
}
```

### 步骤 6：编译工作空间

回到工作空间根目录，执行编译（生成 C++ 可执行文件和 srv 头文件）：

```bash
cd ~/catkin_ws
catkin_make  # 编译
source devel/setup.bash  # 必须：加载编译后的环境变量
```

✅ 编译成功标志：无报错，`devel/lib/cpp_service_demo/`目录下生成`add_two_ints_server`和`add_two_ints_client`可执行文件。

### 步骤 7：运行测试

#### 终端 1：启动 roscore

```bash
roscore
```

#### 终端 2：启动 C++ Service Server

```bash
cd ~/catkin_ws
source devel/setup.bash
rosrun cpp_service_demo add_two_ints_server
```

✅ 预期输出：

```
[INFO] [1734578901.234567]: C++版AddTwoInts服务已启动，等待客户端请求...
```

#### 终端 3：运行 C++ Client 发起请求

```bash
cd ~/catkin_ws
source devel/setup.bash
# 传入两个整数（如10和20），测试求和
rosrun cpp_service_demo add_two_ints_client 10 20
```

##### 预期输出：

- 服务端终端：

  ```
  [INFO] [1734578905.789012]: 收到客户端请求：a = 10, b = 20
  ```

* 客户端终端：`[INFO] [1734578905.789012]: 求和结果：30`

```bash
[INFO] [1734578905.789012]: 返回响应：sum = 30
```

