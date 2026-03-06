# ROS2_多传感器时间同步

---- Chime Lee

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
#include <boost/lockfree/spsc_queue.hpp>
#include <array>

struct MotorFeedback {
    double position;
    double velocity;
    rclcpp::Time stamp;
};

class MotorDriver {
public:
    MotorDriver() : buffer_(256) {  // 容量256
        feedback_thread_ = std::thread(&MotorDriver::read_feedback, this);
    }

    bool get_latest_feedback(MotorFeedback& fb) {
        return buffer_.pop(fb);  // 非阻塞，若队列空返回false
    }

private:
    void read_feedback() {
        MotorFeedback fb;
        while (running_) {
            // 从硬件读取数据 ...
            fb.position = read_position();
            fb.velocity = read_velocity();
            fb.stamp = now();
            while (!buffer_.push(fb)) {
                // 队列满，可丢弃最旧数据或阻塞等待
                buffer_.pop(); // 丢弃一个旧数据
            }
        }
    }

    boost::lockfree::spsc_queue<MotorFeedback, boost::lockfree::capacity<256>> buffer_;
    std::thread feedback_thread_;
    std::atomic<bool> running_{true};
};
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

