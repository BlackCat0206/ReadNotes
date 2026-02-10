# C++ 设计模式

---- Chime Lee

## 一、概述

**设计模式分类**：设计模式主要分为三大类（**创建型、结构型、行为型**）；

## 二、创建型模式

**创建型模式**：关注对象创建细节，C++中常和构造函数、指针、智能指针等特性实现。

* **单例模式**（**Singleton**）

  * **核心**：保证一个类只有一个实例，并提供全局访问点。
  * **实现**：
    * 私有化构造、拷贝构造、赋值重载；
    * 提供静态成员函数获取实例，注意线程安全（双重检查锁）；
  * **场景**：配置管理、日志
  * **示例**：

  ```cpp
  #include <mutex>
  class Singleton {
  private:
      static Singleton* instance;
      static std::mutex mtx;
      // 私有化构造/拷贝/赋值
      Singleton() = default;
      Singleton(const Singleton&) = delete;
      Singleton& operator=(const Singleton&) = delete;
  public:
      static Singleton* getInstance() {
          if (instance == nullptr) { // 双重检查锁（DCL）
              std::lock_guard<std::mutex> lock(mtx);
              if (instance == nullptr) {
                  instance = new Singleton();
              }
          }
          return instance;
      }
  };
  // 静态成员初始化
  Singleton* Singleton::instance = nullptr;
  std::mutex Singleton::mtx;
  ```

* **工厂模式（Factory）**

  * **核心**：用**工厂类封装对象创建**逻辑，避免客户端直接**new具体类**，符合“**开闭原则**“；
  * **细分**：简单工程、工厂方法、抽象工厂；
  * **C++实现要点**：基类定义接口，子类实现具体逻辑，工厂类根据不同的条件返回不同子类型；
  * **场景**：不同类型的日志输出（文件、控制台）；

  ```cpp
  #include <iostream>
  #include <memory>
  
  // 产品基类（定义统一接口）
  class Product {
  public:
      virtual ~Product() = default;
      virtual void show() = 0; // 纯虚函数，子类必须实现
  };
  
  // 具体产品A
  class ProductA : public Product {
  public:
      void show() override { std::cout << "这是产品A\n"; }
  };
  
  // 具体产品B
  class ProductB : public Product {
  public:
      void show() override { std::cout << "这是产品B\n"; }
  };
  
  // 工厂类（封装创建逻辑）
  class Factory {
  public:
      // 根据类型创建不同产品
      static std::unique_ptr<Product> createProduct(const std::string& type) {
          if (type == "A") {
              return std::make_unique<ProductA>();
          } else if (type == "B") {
              return std::make_unique<ProductB>();
          }
          return nullptr;
      }
  };
  
  // 测试
  int main() {
      // 客户端只和工厂交互，无需知道具体产品的创建细节
      auto productA = Factory::createProduct("A");
      productA->show(); // 输出：这是产品A
  
      auto productB = Factory::createProduct("B");
      productB->show(); // 输出：这是产品B
      return 0;
  }
  ```

## 二、结构型

**结构型模式**：关注类/对象的组合方式，优化结构；

* **适配器模式（Adapter）**

  * **核心**：将一个类的接口转换为客户端期望的另一个接口，解决接口不兼容问题。
  * **C++实现**：对象适配器（组合）：适配器类持有旧对象成员变量，继承客户期望的类，重写期望接口，用旧对象调用旧接口返回；
  * **场景**：**旧接口适配新系统**、第三方库接口封装；

  ```cpp
  #include <iostream>
  
  // 现有类（接口不兼容）
  class OldInterface {
  public:
      void oldRequest() { std::cout << "旧接口：执行旧逻辑\n"; }
  };
  
  // 客户端期望的目标接口
  class Target {
  public:
      virtual ~Target() = default;
      virtual void request() = 0;
  };
  
  // 适配器（组合旧接口，实现目标接口）
  class Adapter : public Target {
  private:
      OldInterface* oldObj; // 持有旧接口对象
  public:
      Adapter(OldInterface* obj) : oldObj(obj) {}
      void request() override {
          // 适配：调用旧接口，包装成新接口
          oldObj->oldRequest();
      }
  };
  
  // 测试
  int main() {
      OldInterface oldObj;
      // 客户端只调用Target接口，无需关心旧接口细节
      Target* adapter = new Adapter(&oldObj);
      adapter->request(); // 输出：旧接口：执行旧逻辑
  
      delete adapter;
      return 0;
  }
  ```

* **装饰器模式（Decorator）**

  * **核心**：动态给对象添加额外功能，替代继承的灵活方案。
  * **C++实现**：基类定义核心接口、装饰器继承基类并持有基类之子，子类扩展功能；
  * **场景**：I/O流
  * **示例**：

  ```cpp
  #include <iostream>
  
  // 基础组件接口
  class Coffee {
  public:
      virtual ~Coffee() = default;
      virtual std::string getDesc() = 0; // 获取描述
      virtual double getPrice() = 0;     // 获取价格
  };
  
  // 具体基础组件：原味咖啡
  class SimpleCoffee : public Coffee {
  public:
      std::string getDesc() override { return "原味咖啡"; }
      double getPrice() override { return 10.0; }
  };
  
  // 装饰器基类（继承Coffee，持有Coffee指针）
  class CoffeeDecorator : public Coffee {
  protected:
      Coffee* coffee;
  public:
      CoffeeDecorator(Coffee* c) : coffee(c) {}
  };
  
  // 具体装饰器：加牛奶
  class MilkDecorator : public CoffeeDecorator {
  public:
      MilkDecorator(Coffee* c) : CoffeeDecorator(c) {}
      std::string getDesc() override { return coffee->getDesc() + "+牛奶"; }
      double getPrice() override { return coffee->getPrice() + 2.0; }
  };
  
  // 具体装饰器：加糖
  class SugarDecorator : public CoffeeDecorator {
  public:
      SugarDecorator(Coffee* c) : CoffeeDecorator(c) {}
      std::string getDesc() override { return coffee->getDesc() + "+糖"; }
      double getPrice() override { return coffee->getPrice() + 1.0; }
  };
  
  // 测试
  int main() {
      // 基础咖啡
      Coffee* coffee = new SimpleCoffee();
      std::cout << coffee->getDesc() << "：" << coffee->getPrice() << "元\n"; // 原味咖啡：10元
  
      // 加牛奶
      coffee = new MilkDecorator(coffee);
      std::cout << coffee->getDesc() << "：" << coffee->getPrice() << "元\n"; // 原味咖啡+牛奶：12元
  
      // 再加糖
      coffee = new SugarDecorator(coffee);
      std::cout << coffee->getDesc() << "：" << coffee->getPrice() << "元\n"; // 原味咖啡+牛奶+糖：13元
  
      delete coffee;
      return 0;
  }
  ```

## 三、行为型模式

**行为型模式**：关注对象间的交互与责任分配

* **观察者模式（Observer）**

  * **核心**：定义一对多的依赖关系，当一个对象状态变化时，所有依赖它的对象自动收到通知。
  * **C++实现**：
    * 主题类维护**观察者列表，提供注册/注销/通知接口**；
    * 观察者类定义**统一的更新接口**；
  * **场景**：事件监听、消息通知、`UI`控件状态同步。
  * **示例**：

  ```cpp
  #include <iostream>
  #include <vector>
  #include <memory>
  
  // 前向声明
  class Subject;
  
  // 观察者接口
  class Observer {
  public:
      virtual ~Observer() = default;
      virtual void update(Subject* sub) = 0; // 收到通知后更新
  };
  
  // 主题接口
  class Subject {
  private:
      std::vector<Observer*> observers; // 观察者列表
      int state; // 主题状态
  public:
      void attach(Observer* obs) { observers.push_back(obs); } // 注册观察者
      void detach(Observer* obs) {
          // 移除观察者（简化版）
          for (auto it = observers.begin(); it != observers.end(); ++it) {
              if (*it == obs) {
                  observers.erase(it);
                  break;
              }
          }
      }
      void notify() { // 通知所有观察者
          for (auto obs : observers) {
              obs->update(this);
          }
      }
      // 设置状态并通知
      void setState(int s) {
          state = s;
          notify();
      }
      int getState() { return state; }
  };
  
  // 具体观察者1
  class ConcreteObserver1 : public Observer {
  public:
      void update(Subject* sub) override {
          std::cout << "观察者1收到通知，主题状态：" << sub->getState() << "\n";
      }
  };
  
  // 具体观察者2
  class ConcreteObserver2 : public Observer {
  public:
      void update(Subject* sub) override {
          std::cout << "观察者2收到通知，主题状态：" << sub->getState() << "\n";
      }
  };
  
  // 测试
  int main() {
      Subject sub;
      ConcreteObserver1 obs1;
      ConcreteObserver2 obs2;
  
      sub.attach(&obs1);
      sub.attach(&obs2);
  
      sub.setState(10); // 输出：两个观察者都收到状态10的通知
      sub.setState(20); // 输出：两个观察者都收到状态20的通知
  
      sub.detach(&obs1);
      sub.setState(30); // 仅观察者2收到通知
  
      return 0;
  }
  ```

* **策略模式（Strategy）**

  * **核心**：定义一系列算法，将每个算法封装起来并可相互替换，算法独立于使用它的客户端。
  * **C++实现**：算法基类定义接口，子类实现不同算法，客户端持有基类指针切换算法；
  * **场景**：排序算法切换、支付方式选择；
  * **示例**：

  ```cpp
  #include <iostream>
  
  // 策略接口（定义算法规范）
  class SortStrategy {
  public:
      virtual ~SortStrategy() = default;
      virtual void sort(int arr[], int len) = 0;
  };
  
  // 具体策略1：冒泡排序
  class BubbleSort : public SortStrategy {
  public:
      void sort(int arr[], int len) override {
          std::cout << "使用冒泡排序\n";
          // 简化版冒泡排序逻辑
          for (int i = 0; i < len-1; ++i) {
              for (int j = 0; j < len-i-1; ++j) {
                  if (arr[j] > arr[j+1]) {
                      std::swap(arr[j], arr[j+1]);
                  }
              }
          }
      }
  };
  
  // 具体策略2：选择排序
  class SelectionSort : public SortStrategy {
  public:
      void sort(int arr[], int len) override {
          std::cout << "使用选择排序\n";
          // 简化版选择排序逻辑
          for (int i = 0; i < len-1; ++i) {
              int minIdx = i;
              for (int j = i+1; j < len; ++j) {
                  if (arr[j] < arr[minIdx]) minIdx = j;
              }
              std::swap(arr[i], arr[minIdx]);
          }
      }
  };
  
  // 上下文（客户端使用策略的入口）
  class Sorter {
  private:
      SortStrategy* strategy; // 持有策略指针
  public:
      Sorter(SortStrategy* s) : strategy(s) {}
      // 切换策略
      void setStrategy(SortStrategy* s) { strategy = s; }
      // 执行排序
      void sortArray(int arr[], int len) {
          strategy->sort(arr, len);
          // 打印排序结果
          for (int i = 0; i < len; ++i) {
              std::cout << arr[i] << " ";
          }
          std::cout << "\n";
      }
  };
  
  // 测试
  int main() {
      int arr[] = {3,1,2,5,4};
      int len = sizeof(arr)/sizeof(arr[0]);
  
      // 使用冒泡排序
      Sorter sorter(new BubbleSort());
      sorter.sortArray(arr, len);
  
      // 切换为选择排序
      sorter.setStrategy(new SelectionSort());
      sorter.sortArray(arr, len);
  
      return 0;
  }
  ```

## 四、核心原则

设计模型的核心是遵循'开闭原则'（对扩展开放、对修改关闭）、‘里氏替换原则’等;