# ROS2_低延时

---- Chime Lee

****

### 1. 调度与实时性：设置线程优先级和 CPU 亲和性

在节点中，获取控制线程并设置为实时优先级，绑定到特定 CPU 核心。

```cpp
#include <pthread.h>
#include <sched.h>

void set_thread_realtime_priority(std::thread &thread, int priority, int cpu_core) {
    pthread_t native_handle = thread.native_handle();

    // 设置调度策略为 SCHED_FIFO，优先级 priority（需 root 权限）
    sched_param param;
    param.sched_priority = priority;
    if (pthread_setschedparam(native_handle, SCHED_FIFO, &param) != 0) {
        RCLCPP_ERROR(rclcpp::get_logger("RealTime"), "Failed to set realtime priority");
    }

    // 绑定到指定 CPU 核心
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_core, &cpuset);
    if (pthread_setaffinity_np(native_handle, sizeof(cpu_set_t), &cpuset) != 0) {
        RCLCPP_ERROR(rclcpp::get_logger("RealTime"), "Failed to set CPU affinity");
    }
}

// 在节点中创建控制线程
class ControlNode : public rclcpp::Node {
public:
    ControlNode() : Node("control_node") {
        control_thread_ = std::thread(&ControlNode::control_loop, this);
        set_thread_realtime_priority(control_thread_, 90, 0); // 优先级 90，绑定到 core 0
    }
private:
    void control_loop() {
        while (rclcpp::ok()) {
            // 1ms 控制循环
            auto start = std::chrono::steady_clock::now();
            // ... 控制计算 ...
            std::this_thread::sleep_until(start + std::chrono::milliseconds(1));
        }
    }
    std::thread control_thread_;
};
```

**说明**：需要 root 权限或设置 `CAP_SYS_NICE` 能力。生产环境中常将节点放入专用进程，并通过 systemd 或 launch 脚本配置权限。

------

### 2. ROS2 QoS 配置：BEST_EFFORT + 小队列

发布高频实时数据（如 IMU）时，使用如下 QoS：

```cpp
// 发布端
rclcpp::QoS qos(rclcpp::KeepLast(1));          // 队列深度1，只保留最新
qos.best_effort();                               // 尽力传输，不重传
qos.durability_volatile();                       // 不保留历史

imu_publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("imu/data", qos);

// 订阅端同理
auto sub_qos = rclcpp::QoS(rclcpp::KeepLast(1)).best_effort().durability_volatile();
imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>(
    "imu/data", sub_qos, [this](const sensor_msgs::msg::Imu::SharedPtr msg) {
        // 实时处理
    });
```

**说明**：对于控制指令，可使用类似配置，但需注意若需可靠传输，可适当调整。

------

### 3. 零拷贝与进程内通信：使用可组合节点（Component）

将多个节点组合为组件，加载到同一进程，实现零拷贝。

**编写组件类**（例如 `EstimatorComponent`）：

```cpp
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_msgs/msg/float64.hpp"

class EstimatorComponent : public rclcpp::Node {
public:
    explicit EstimatorComponent(const rclcpp::NodeOptions & options) : Node("estimator", options) {
        // 订阅 IMU，使用进程内通信
        auto sub_qos = rclcpp::QoS(rclcpp::KeepLast(1)).best_effort();
        imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>(
            "imu/data", sub_qos, std::bind(&EstimatorComponent::imu_callback, this, std::placeholders::_1));
    }

private:
    void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg) {
        // 直接处理 msg，无需拷贝
        // ... 融合估计 ...
    }
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
};

RCLCPP_COMPONENTS_REGISTER_NODE(EstimatorComponent)
```

**CMakeLists.txt** 中添加：

```cmake
add_library(estimator_component SHARED src/estimator_component.cpp)
target_link_libraries(estimator_component ${dependencies})
rclcpp_components_register_node(estimator_component
  PLUGIN "EstimatorComponent"
  EXECUTABLE estimator_component_node)
```

**启动容器**：

```bash
ros2 run rclcpp_components component_container
# 在另一个终端加载组件
ros2 component load /ComponentManager estimator_component EstimatorComponent
```

此时 IMU 发布者若也在同一容器内，则数据通过进程内通道直接传递指针，实现零拷贝。

------

### 4. 数据路径优化：环形缓冲区（无锁队列示例）

对于高频数据，可使用预分配的环形缓冲区，避免动态内存分配和拷贝。这里使用 `boost::lockfree::spsc_queue` 作为示例：

```cpp
1. 实时调度：设置线程优先级与CPU绑核
将关键线程设为 SCHED_FIFO 实时优先级，并绑定到专用CPU核心。

cpp
#include <pthread.h>
#include <sched.h>
#include <thread>

void set_thread_realtime(std::thread& thread, int priority, int cpu_core) {
    pthread_t native_handle = thread.native_handle();

    // 设置调度策略和优先级
    sched_param param;
    param.sched_priority = priority;
    if (pthread_setschedparam(native_handle, SCHED_FIFO, &param) != 0) {
        perror("pthread_setschedparam failed");
    }

    // 绑定CPU核心
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_core, &cpuset);
    if (pthread_setaffinity_np(native_handle, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np failed");
    }
}

// 在节点中启动控制线程
class ControlNode : public rclcpp::Node {
public:
    ControlNode() : Node("control_node") {
        control_thread_ = std::thread(&ControlNode::control_loop, this);
        set_thread_realtime(control_thread_, 90, 0); // 优先级90，绑定core0
    }

private:
    void control_loop() {
        while (rclcpp::ok()) {
            auto start = std::chrono::steady_clock::now();
            // 执行1ms控制周期任务
            // ...
            std::this_thread::sleep_until(start + std::chrono::milliseconds(1));
        }
    }
    std::thread control_thread_;
};
说明：需要root权限或CAP_SYS_NICE能力。生产环境可通过systemd服务配置AmbientCapabilities=CAP_SYS_NICE。

2. ROS2 QoS优化：BEST_EFFORT + 小队列
针对高频实时数据（如IMU），配置QoS为尽力传输、队列深度1。

cpp
// 发布端
rclcpp::QoS imu_qos(rclcpp::KeepLast(1));
imu_qos.best_effort();
imu_qos.durability_volatile();

imu_publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("imu/data", imu_qos);

// 订阅端（同样配置）
auto sub_qos = rclcpp::QoS(rclcpp::KeepLast(1)).best_effort().durability_volatile();
imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>(
    "imu/data", sub_qos,
    [this](const sensor_msgs::msg::Imu::SharedPtr msg) {
        // 实时处理
    });
说明：KeepLast(1)保证队列中只有最新数据，避免处理过时消息；best_effort禁用重传，最小化延迟。

3. 零拷贝与进程内通信：可组合节点（Component）
将驱动、估计、控制节点打包为组件，加载到同一进程，实现零拷贝。

3.1 编写组件类（例如驱动节点）
cpp
// imu_driver_component.cpp
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "sensor_msgs/msg/imu.hpp"

class ImuDriverComponent : public rclcpp::Node {
public:
    explicit ImuDriverComponent(const rclcpp::NodeOptions& options)
        : Node("imu_driver", options) {
        // 发布者使用进程内通信（默认自动启用）
        publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("imu/data", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(5),
                                          std::bind(&ImuDriverComponent::timer_callback, this));
    }

private:
    void timer_callback() {
        auto msg = std::make_unique<sensor_msgs::msg::Imu>();
        msg->header.stamp = this->now();
        // 填充数据...
        publisher_->publish(std::move(msg));  // 移动语义，避免拷贝
    }

    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

RCLCPP_COMPONENTS_REGISTER_NODE(ImuDriverComponent)
3.2 CMakeLists.txt 注册组件
cmake
add_library(imu_driver_component SHARED src/imu_driver_component.cpp)
target_link_libraries(imu_driver_component ${dependencies})
rclcpp_components_register_node(imu_driver_component
  PLUGIN "ImuDriverComponent"
  EXECUTABLE imu_driver_node)
3.3 启动容器并加载组件
bash
# 终端1：启动组件容器
ros2 run rclcpp_components component_container

# 终端2：加载驱动组件
ros2 component load /ComponentManager imu_driver_component ImuDriverComponent

# 加载估计组件（假设已编译）
ros2 component load /ComponentManager estimator_component EstimatorComponent
说明：同一容器内的组件间通信通过进程内管道传递指针，实现零拷贝。

4. 数据路径优化：无锁环形缓冲区
使用 boost::lockfree::spsc_queue 实现线程安全、无锁的缓冲区，用于高频数据传递。

cpp
#include <boost/lockfree/spsc_queue.hpp>
#include <atomic>
#include <thread>

struct MotorFeedback {
    double position;
    double velocity;
    rclcpp::Time stamp;
};

class MotorDriver {
public:
    MotorDriver() : buffer_(256) {  // 容量256
        thread_ = std::thread(&MotorDriver::read_loop, this);
    }

    bool get_latest(MotorFeedback& fb) {
        return buffer_.pop(fb);  // 非阻塞，队列空则返回false
    }

private:
    void read_loop() {
        MotorFeedback fb;
        while (running_) {
            // 从硬件读取数据
            fb.position = read_position();
            fb.velocity = read_velocity();
            fb.stamp = rclcpp::Clock().now();

            // 入队，若队列满则丢弃最旧数据
            while (!buffer_.push(fb)) {
                buffer_.pop();  // 丢弃一个旧数据
            }
        }
    }

    boost::lockfree::spsc_queue<MotorFeedback, boost::lockfree::capacity<256>> buffer_;
    std::thread thread_;
    std::atomic<bool> running_{true};
};
说明：spsc_queue 适用于单生产者单消费者场景，无锁且实时安全。

5. 多线程执行器与回调组解耦
创建两个回调组，分别处理高优先级（电机控制）和低优先级（图像处理）任务，并使用多线程执行器。

cpp
class RobotNode : public rclcpp::Node {
public:
    RobotNode() : Node("robot_node") {
        // 创建两个互斥回调组
        high_cb_group_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        low_cb_group_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        // 高优先级：IMU订阅
        rclcpp::SubscriptionOptions options_high;
        options_high.callback_group = high_cb_group_;
        imu_sub_ = create_subscription<sensor_msgs::msg::Imu>(
            "imu/data", rclcpp::QoS(1).best_effort(),
            [this](const sensor_msgs::msg::Imu::SharedPtr msg) {
                // 快速处理：EKF预测
            }, options_high);

        // 低优先级：图像订阅
        rclcpp::SubscriptionOptions options_low;
        options_low.callback_group = low_cb_group_;
        image_sub_ = create_subscription<sensor_msgs::msg::Image>(
            "camera/image", rclcpp::QoS(10),
            [this](const sensor_msgs::msg::Image::SharedPtr msg) {
                // 耗时处理：AI推理
            }, options_low);
    }

private:
    rclcpp::CallbackGroup::SharedPtr high_cb_group_;
    rclcpp::CallbackGroup::SharedPtr low_cb_group_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<RobotNode>();
    // 创建多线程执行器，2个线程
    rclcpp::executors::MultiThreadedExecutor executor(
        rclcpp::ExecutorOptions(), 2);
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}
说明：MultiThreadedExecutor 会从不同回调组中并行取回调执行，确保高优先级任务不被低优先级阻塞。

6. 高精度定时器实现1ms控制周期
使用 clock_nanosleep 实现微秒级精度的固定周期循环。

cpp
#include <time.h>
#include <thread>

void high_precision_loop() {
    struct timespec next_time;
    clock_gettime(CLOCK_MONOTONIC, &next_time);

    while (rclcpp::ok()) {
        // 设置下一个唤醒时刻为当前时间 + 1ms
        next_time.tv_nsec += 1'000'000; // 1ms
        if (next_time.tv_nsec >= 1'000'000'000) {
            next_time.tv_sec += 1;
            next_time.tv_nsec -= 1'000'000'000;
        }

        // 执行控制计算
        // ...

        // 忙等待到精确时刻（可选，对于<50μs抖动要求）
        // while (clock_gettime(CLOCK_MONOTONIC, &now) == 0 && 
        //        (now.tv_sec < next_time.tv_sec || 
        //         (now.tv_sec == next_time.tv_sec && now.tv_nsec < next_time.tv_nsec))) {
        //     std::this_thread::yield();
        // }

        // 使用clock_nanosleep睡眠到绝对时间（精度较高，但受内核调度影响）
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_time, nullptr);
    }
}
说明：clock_nanosleep 配合 TIMER_ABSTIME 可避免累积误差。对于硬实时要求，可结合忙等待进一步降低抖动。
```

**说明**：无锁队列可在实时线程中安全使用，避免锁竞争。

------

### 5. 多线程执行器与回调组

创建两个回调组，将高优先级与低优先级任务分离，并使用多线程执行器。

cpp

```cpp
class RobotNode : public rclcpp::Node {
public:
    RobotNode() : Node("robot_node") {
        // 创建回调组
        high_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        low_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        // 订阅 IMU（高优先级）
        rclcpp::SubscriptionOptions options;
        options.callback_group = high_cb_group_;
        imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>(
            "imu/data", qos, 
            [this](const sensor_msgs::msg::Imu::SharedPtr msg) {
                // 高优先级处理，如 EKF 预测
            }, options);

        // 订阅图像（低优先级）
        options.callback_group = low_cb_group_;
        image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "camera/image", rclcpp::QoS(10),
            [this](const sensor_msgs::msg::Image::SharedPtr msg) {
                // 耗时图像处理
            }, options);
    }

private:
    rclcpp::CallbackGroup::SharedPtr high_cb_group_;
    rclcpp::CallbackGroup::SharedPtr low_cb_group_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
};

// 主函数使用多线程执行器
int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<RobotNode>();
    rclcpp::executors::MultiThreadedExecutor executor(
        rclcpp::ExecutorOptions(), 2); // 2个线程
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}
```

**说明**：多线程执行器会从不同回调组中并行取回调执行。高优先级回调组可独占一个线程，低优先级共享剩余线程。

------

### 6. 高精度定时器实现 1ms 固定周期

使用 ROS2 的 `create_wall_timer` 配合高精度时钟，或手动实现忙等待。

```cpp
class ControlNode : public rclcpp::Node {
public:
    ControlNode() : Node("control_node") {
        // 使用 ROS2 定时器，但精度可能受系统调度影响
        timer_ = this->create_wall_timer(std::chrono::milliseconds(1), 
                                          std::bind(&ControlNode::control_callback, this));
    }

private:
    void control_callback() {
        // 控制计算
    }
    rclcpp::TimerBase::SharedPtr timer_;
};
```

若需要更高精度（抖动 < 50μs），可使用专用线程 + 忙等待（如之前实时线程示例），并结合 `clock_nanosleep`。

