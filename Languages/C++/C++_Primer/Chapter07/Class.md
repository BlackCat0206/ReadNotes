# Class

***---- Li Jinsheng***

## 一、概述

本篇为《C++ Primer》（第5版）第7章读书笔记，主要内容为类相关基本概述、特性。

## 二、内容

类的基本思想：数据抽象（*data abstraction*）和封装（*encapsulation*）。

* 数据抽象：依赖于接口和实现分离的编程技术。

* 封装：类的接口和类的实现分开。
  * 用户不能访问类中数据，保证用户代码不会无意间破坏类的状态。
  * 封装的具体实现细节可以随时修改，无需调整用户层的代码。

### 2.1 `this`指针

对任何类成员的直接访问都被看作`this`指针的隐式引用，其本身是常量指针（const pointer）。

不允许自定义名称为`this`是非法的。

### 2.2 `const`常量成员函数

在成员函数的参数列表后加`const`用于修饰`this`指针，即`const * const this`。这样的函数也被称为常量函数。

```cpp
class Sales_data
{
public:
    std::string isbn() const { return bookNo; }
private:
    std::string bookNo;
};
```

* **常量函数的作用**：确保函数不修改对象状态，支持`const`对象调用，提升代码安全性。
* **使用原则**：所有 “只读” 操作的成员函数都应声明为`const`，“修改” 操作的函数保持非`const`。
* 类的编译顺序：先编译成员变量，后编译函数定义，这样函数就可以随意的使用成员变量了。
* 返回一个`this`对象的函数，函数返回值类型为类的引用：

```cpp
Sales_data& Sales_data::combine(const Sales_data& rhs)
{
    unint_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}
```

### 2.3 构造函数

* 函数名与类名相同，没有返回值，可以重载，不能被声明为`const`。

#### 2.3.1 默认构造函数（C++11）

* 若类中未显示的定义构造函数，则编译器会隐式地定义一个默认构造函数（没有任何形参），其也被称为**合成的默认构造函数（*synthesized default constructor*）。**

* 初始化规则：
  * 若存在类内初始值，则用其初始化成员。
  * 否则，默认初始化成员。
* 某些类不能依赖默认构造函数：
  * 类初始化的过程最好是可控的。
  * 基本类型（算数类型，复合类型）的默认初始化的结果是位置的。
  * 若一个类中包含了其他的类类型，除法这个类类型的所有成员变量都有类内值，否则编译器无法将其初始化。

* `=default`生成默认构造函数。（C++11）

```cpp
class Sales_data
{
public:
    Sales_data() = default;
    std::string isbn() const { return bookNo; }
private:
    std::string bookNo;
};
```

注：要默认构造函数有效，其类内成员要有类内初始值，若没有，则要使用初始化列表

#### 2.3.2 构造函数的初始化列表

在构造函数的参数列表后，花括号之前，用分号（`:`）和逗号（`,`）进行分隔，用于初始化成员变量。

```cpp
class Sales_data
{
public:
    Sales_data() = default;
    Sales_data(const std::string *str) : bookNo(str) {}
    std::string isbn() const { return bookNo; }
private:
    std::string bookNo;
};
```

* 初始化列表的执行顺序与成员变量的声明顺序一致，与初始化列表顺序无关。

#### 2.3.3 默认实参和构造函数

```cpp
class Sales_data
{
public:
    Sales_data() = default;
    Sales_data(const std::string *str = "") : bookNo(str) {}
    std::string isbn() const { return bookNo; }
private:
    std::string bookNo;
};
```

#### 2.3.4 委托构造函数（C++11）

委托构造函数（*delegating constructor*）：用类的其他构造函数执行自己的初始化过程。其不同之处在于初始化列表是其他构造函数。

```cpp
class Sales_data
{
public:
    // 非委托构造函数
    Sales_data(std::string s, unsigned cnt, double price):
    	bookNo(s), units_sold(cnt), revenue(cnt*price) {}
    // 委托构造函数
    Sales_data() : Sales_data("", 0, 0) {}
    Sales_data(std::string s) : Sales_data(s, 0, 0) {}
  	Sales_data(std::istream &is): Sales_data() {read(is, *this);}
private:
    std::string bookNo;
    unsigned units_sold;
    double revenue;
};
```

#### 2.3.5 隐式的类类型转换（拷贝初始化也是一种隐式初始化）

如果构造函数只接受**一个实参**，它就会有类类型隐式转换机制，也被称为转换构造函数（*converting constructor*）。

```cpp
string str = "9-99-999";
itme.combine(str);		// 隐式的将`str`转换为了Sales_data类型，该对象units_sold和revenue为0，bookNo为9-99-999
```

* 隐式类类型转换只能执行一步。

```cpp
item.combine("9-99-999");		// 错误！这里进行了两步，"9-99-999"转换为`string`类型，`string`类型转换为`sales_data`类型
```

#### 2.3.6 `explicit`抑制构造函数的隐式转换（声明时调用！！！）

* 作用与只接受一个实参的构造函数。用于限制其隐式转换。
* **只在声明时调用即可，定义时不要用**。
* `explicit`构造函数只能直接初始化，不能拷贝初始化。
* 可以通过`static_cast`进行转换

```c++
class Sales_data
{
public:
    Sales_data() = default;
    Sales_data(std::string s, unsigned cnt, double price):
    	bookNo(s), units_sold(cnt), revenue(cnt*price) {}
    
    explicit Sales_data(std::string& s):bookNo(s) {};
    explicit Sales_data(std::istream &is);
private:
    std::string bookNo;
    unsigned units_sold;
    double revenue;
};


Sales_data item1(null_bool);  //正确，直接初始化
Sales_data item2 = item1;     //错误，不能拷贝初始化
item1.combine(static_cast<Sales_data>(cin));	// 正确，static_cast可以使用explicit的构造函数
```

#### 2.3.7 聚合类

聚合类（aggregate class）可以使得用户可以直接访问取对象，并具有特殊的初始化语法。

* 聚合类的条件
  * 所有成员都是`public`的
  * 没有定义任何构造函数
  * 没有类内初始值
  * 没有基类，也没有`virtual`函数
* 初始化方式：用花括号括起来的参数列表,顺序与类型要与声明一致。

```cpp
struct Data{
   int ival;
   string s;
};
Data vall = {0, "Anna"};
```

#### 2.3.8 `constexpr`字面值常量类

在第2章中学到`constexpr`可以将运算提前到编译时期，保证代码的灵活性也提高了程序的性能。

* 字面常量类的条件：
  * 所有类中成员均是**字面值类型**。
  * 至少有一个`constexpr`构造函数。
  * 如果有类内初始值，其必须是一个常量表达式。若其含有其他类类型，要用对应的`constexpr`构造函数。
  * 类必须使用析构函数的默认定义。

### 2.4 访问控制与封装

* `class`和`struct`对于区别：
  * `class`：默认权限为`private`，用户无法访问内部成员。可以通过定义`public`来访问其内容。
  * `struct`：默认权限为`public`。

#### 2.4.1 友元

友元（*friend*）：可以访问类中的非共有成员。

```cpp
class Sales_data;
std::istram &read(std::istream, Salse_data& sd);
class Manager
{
public:
    Manager() = default;
};

class Tester
{
public:
    Tester() = default;
    void clear(Sales_data& );
};

class Sales_data
{
    // 其他类为友元
    friend class Manager;
    
    // 其他类中函数为友元
    friend void Tester::clear(Sales_data& );
    
    // 外部函数为友元
    friend std::istream &read(std::istream &is, Sales_data& sd);
public:
    Sales_data() = default;
    Sales_data(const std::string *str) : bookNo(str) {}
    std::string isbn() const { return bookNo; }
private:
    std::string bookNo;
};
```

* 友元声明：声明的类或函数必须是可见的，即必须在类前声明。

### 2.5 类的其他特性

#### 2.5.1 定义一个类型成员

```cpp
class Screen
{
public:
    typedef std::string::size_type pos;
    
    using pos = std::string::size_type;	// 相同含义
    
private:
    pos cursor = 0;
    pos height = 0;
    pos width = 0;
    std::string contents;
};
```

#### 2.5.2 `mutable`可变数据成员

* 打破`const`限制，可以在**常量函数**中修改`mutable`成员（其永远不是`const`）。主要用于修饰不会改变类状态的辅助成员变量。

```cpp
class Screen
{
public:
    typedef std::string::size_type pos;
    
    using pos = std::string::size_type;	// 相同含义
    
    void some_member() const;
    
private:
    pos cursor = 0;
    pos height = 0;
    pos width = 0;
    std::string contents;
    mutable size_t access_ctr;
};

void Screen::some_member() const
{
    ++access_ctr;			// 保存一个计数值。
}
```

#### 2.5.3 返回`*this`的成员函数

返回引用的函数是**左值**，即函数返回的结果是对象本身而非副本，可以将**一系列的操作符连接起来**。

```cpp
myScreen.move(4,0).set('#');
```

* 一个`const`成员函数返回`*this`那么其返回的类型为**常量引用**。

* `const`成员函数重载：注意位置
  * 在参数列表后：用于修饰`this`指针，可以实现重载。
  * 返回类型前：不行，函数重载是基于函数签名（函数名和参数列表，不能通过返回值类型进行重载）。

#### 2.5.4 类的声明

```cpp
class Screen;
```

* 向前声明（*forward declaration*）：引入`Screen`名字，并说明`Screen`是一个类类型。

* 类只允许其成员是类类型的**指针**或**引用**。

### 2.6 类型作用域

#### 2.6.1 名字查找

名字查找（*name lookup*）：寻找与名字最匹配的声明的过程。

* 步骤：
  * 在名字所在的块中查找，只考虑在名字之前的声明；
  * 若未找到，则在外部作用域查找。
  * 若最终未找到，编译器报错。
* 类成员的声明与查找：

```cpp
typedef double Money;
string bar;
class Account
{
public:
    Money balance() { return bar; }		// 编译器只考虑名字之前的声明
private:
    Money bar;
};
```

* 类型名要特殊处理：

```cpp
typedef double Money;
string bar;
class Account
{
public:
    Money balance() { return bar; }		// 使用外层作用域
private:
    typedef double Money;				// 错误！不能重新定义Money
    Money bar;
};
```

**若类成员中已经使用了外层作用域的某个名字，在类内重新定义是错误的**。

* 成员定义中的普通块作用域和名字查找：
  * 在成员函数内查找该名字，只有在函数使用之前的声明会被考虑。
  * 若未找到，则在类内继续查找。
  * 若未找到，则在成员函数定义之前的作用域查找。

```cpp
int height;
class Screen
{
public:
    typedef std::string::size_type pos;
    void dummy_fcn(pos height){
        cursor = width * height;		// 这里的height是形参。若是要用成员：this->height或Screen::height
    }
private:
    pos cursor = 0;
    pos height = 0;
    pos width = 0;
};
```

* 使用全局作用域内容：

```cpp
int height;
class Screen
{
public:
    typedef std::string::size_type pos;
    void dummy_fcn(pos height){
        cursor = width * ::height;		// 
    }
private:
    pos cursor = 0;
    pos height = 0;
    pos width = 0;
};
```

### 2.7 类的静态成员

* 类的静态成员存在于**任何对象对外**，对象中不包含与静态成员有关数据。
* 其被**所有对象共享**，不与任何对象绑定，它们**不包含`this`指针**。**故静态函数不能被声明为`const`，其函数内部也不能使用`this`指针。**

```cpp
class Account
{
public:
    static void rate() {return interestRate;}
    static void rate(double);
private:
    static double interestRate;
};


// 调用方式：作用域
double r;
r = Account::rate();

// 引用或指针
Account ac1;
Account* ac2 = &ac1;

r = ac1.rate();
r = ac2->rate();
```

* 类外定义时，不能重复`static`关键字

```cpp
void Account::rate(double newRate)
{
   	interestRate = newRate;
}

double Account::interestRate = initRate();
```

* 类内初始值：若静态成员是`constexpr`字面值常量类型类型，可以为其提供`const`常量表达式。即使在内部初始化，还是要在外部定义下。

```cpp
class Account
{
public:
    static void rate() {return interestRate;}
    static void rate(double);
private:
    static double interestRate;
    static constexpr int perid = 30;		// perid是常量表达式
    double daily_tbl[perid]
};

constexper int Account::perid;				// 初始值在类内部提供
```

* 静态成员和普通成员的区别：

  * 静态数据成员可以是**它所属的类型**，普通数据程序只能是引用或指针。

  ```cpp
  class Account
  {
  public:
      static void rate() {return interestRate;}
      static void rate(double);
  private:
      static double interestRate;
      static Account acc;
      Accout *acp1;
      Accout &acr1;
  };
  ```

  * 可以使用静态类型作为默认实参。

  ```cpp
  class Screen
  {
  public:
      Screen& clear(char = bkground);
  private:
      static const char bkground;
  };
  ```

## 三、小结

