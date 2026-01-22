# 函数

***---- Li Jinsheng***

***

## 一、概述

本篇为《C++ Primer》（第5版）第6章读书笔记，主要内容为函数、各类返回值、参数、与`C++`独有的特性。基础内容与《C++ Primer Plus》（第6版）**第9章**有相似处。

本篇重点在于`C++`中一些特性与之前`C`中重点内容。

其中有关函数指针可参考《C Traips and Pitfalls》读书笔记：**2.2.1 指针和类型转换**与《C Prime Plus》第14章读书笔记：**2.6 函数和指针**

## 二、内容

### 2.1 形参和实参

* 形参：函数体内部定义的变量，说明函数要调用时要传入的参数类型与数量。自动对象
* 实参，调用函数时，用于初始化形参的变量。
* 局部静态变量：内部链接、静态存储期、块作用域。从函数调用一直到程序结束。

### 2.2 参数传递

#### 2.2.1 引用传参

```cpp
void reset(int &i)
{
    i = 10;
    return;
}
```

* 形参引用绑定了原对象，（实测可以传参一个引用）
* 优点：对于类类型、`string`类等占用内存较大的类型，可以避免拷贝操作，减少内存使用。

#### 2.2.2 `const`形参和实参

* 第2章读书笔记：2.3.4 顶层`const`
* 形参是`const`时，初始化时会忽略顶层`const`，底层`const`保留。

```cpp
void constTest(const int i)
{
    std::cout << i << std:endl;
    return;
}
```

* 指针或引用的`const`，是底层`const`保留，可以用非常量初始化，但反过来不行。

#### 2.2.3 数组形参

* 可参考《C Traips and Pitfalls》读书笔记：**2.3.2 作为函数的参数数组声明**

* 数组作为形参时，因为**不能拷贝数组**，所以传入的是**指向数组首元素的指针**。

* 为规避函数访问数组越界，推荐如下两种方式：

  * `C++11`数组的迭代器`begin()`和`end()`

  ```cpp
  void printf(const int *begin, const int *end)
  {
      while (begin != end)
      {
          cout << *begin++ << endl;
      }
  }
  ```

  * 函数形参要求传入数组的大小：

  ```cpp
  void printf(const int ia[], size_t size)
  {
      for (size_t i = 0; i < size; ++i)
      {
          cout << ia[i] << endl;
      }
  }
  ```

* 数组引用形参：只有数组的引用，没有引用的数组！！！第3章：**2.5.2 引用和数组**

```cpp
void print(int (&arr)[10])
{
    for (auto elem : arr)
    {
        cout << elem << endl;
    }
}
```

### 2.3 含有可变形参函数（C++11）

* 可以有如下三种方式声明含有可变形参的函数：

  * C++11 提供模板`initializer_list`，供形参类型相同的数组，其操作与`vector`类型，有`size()`、`begin()`、`end()`。

  ```cpp
  void error_msg(initializer_list<string> il)
  {
      for (auto beg = il.begin(); beg ！= il.end(); ++beg)
      {
          cout << *beg << ""
      }
      cout << endl;
      return;
  }
  
  error_msg({"functionX", "functionN"});		// 初始化参数列表做形参
  ```

  * 省略符（... ...）：传入的实参不会进行类型检查。

  ```cpp
  void foo(...);		
  ```

### 2.4 返回类型和`return`语句

#### 2.4.1 值是如何被返回的

函数的返回值被用来**初始化调用点的变量**。

**重点：不要返回临时变量的引用或指针。**

```cpp
const string &printf(void)
{
    string ret;
    if (!ret.empty())
    {
        return ret;		// 错误，返回临时变量的引用
    }
    else
    {
        return "Empty";		// 错误，"Empty"是一个临时变量。
    }
}
```

#### 2.4.2 引用返回左值

调用一个**返回引用的函数得到左值**，其余类型都是**右值**（调用返回指针类型函数，返回的是**指针的值，即是地址**，不能解引用和赋值）。

#### 2.4.3 列表初始化返回值（C++11）

C++11规定可以用花括号包围的返回值。

```cpp
vector<string> process()
{
    return {"functionx", "functiony"}
}
```

#### 2.4.4 返回数组指针（C++11）

* 可参考《C Traips and Pitfalls》读书笔记：**2.3.2 作为函数的参数数组声明**
* 或者参考第2章2.5.1节内容

```cpp
typedef int arrT[T];
using arrT = int[10];

arrT *func(int i);
```

* 声明一个**返回指向数组的函数**，格式如下：

```cpp
Type (* functionName(parameter_list))[dimension];

int (*func(int i))[10];
```

* 尾置类型（trailing return type）

```cpp
auto fun(int i) -> int (*)[10];
```

* `decltype`推测，第3章读书笔记：**2.5.4 指针和数组（C++11）**`decltype`转换为数组

```cpp
int numArr[10] = {0};

decltype(numArr) *fun(int i);
```

### 2.5 函数重载

函数重载（*verload function*）：在同一作用域下，函数名形同，参数列表不同。

* 注：不允许两个函数除返回值类型外，其他信息均相同。

```cpp
int func();
int func(int);
int func(int, int);
int func(double);
int func(double, double);
```

#### 2.5.1 重载和`const`

* 重载会忽略顶层`const`保留底层`const`。有关顶层`const`概念见第2章读书笔记：2.3.4 顶层`const`。
* 一个拥有顶层`const`的形参和一个没有顶层`const`的形参区分：

```cpp
int func(int);
int func(const int);		// 重复声明了 int func(int);

int func(int*);
int func(int* const);		// 重复声明了 int func(int*);
```

* 对于引用和指针，通过区分其所指内容是否是常量对象，是可以实现函数重载的。

```cpp
int func(int*);
int func(const int*);			// 作用于指向常量的指针

int func(int &);
int func(const int&);			// 作用于绑定常量的引用
```

* `const_cast`和重载：`const_cast`见第4章读书笔记：**2.9.3 `reinterpret_cast`**

对于形参是底层`const`返回值也是底层`const`的情况下，我们可以用非常量作为实参。但会引入一个问题，我们想修改返回的常量引用，可借助`const_cast`实现。

```cpp
const string &compareShort(const string &str1, const string &str2)
{
    return (s1.size() <= s2.size()) ? s1 : s2;
}

string &compareShort2(string &str1, string &str2)
{
    auto &r = compareShort(const_cast<const string&>str1, const_cast<const string&>str2);
    return const_cast<string&>(r);
}
```

#### 2.5.2 调用重载函数

对于定义了重载函数，调用其过程被称为**函数匹配（*function matching*）**或者**重载确认（*overlaod resolution*）**。其结果会有如下三种：

* 编译器找到一个和实参匹配的**最佳匹配（*best matching*）**，并生成调用函数的代码。
* 找不到任何一个与实参匹配的函数，编译器发出**无匹配（*no match*）**的错误告警。
* 有多与一个的匹配结果，但每一个都不是明显的最近选择。此时会发生**二义性调用（*ambiguous call*）**。

#### 2.5.3 重载和作用域

C++中函数的名字查找在参数匹配之前。（即局部声明会隐藏外层作用域的函数声明）。

```cpp
void print(double);
int main(void)
{
    void print(int);
    int i = 0;
    print(i);		// 调用的是局部声明
    print(3.14);	// 调用的是局部声明
    return 0;
}
```

### 2.6 特殊用途语言特性

#### 2.6.1 默认实参

* 函数定义时用**非局部变量**为形参赋于默认值。最好将默认实参的形参声明在形参列表的右侧。
* 规则：一旦一个形参有了默认实参，其后的实参也必须有默认实参。

```cpp
int numSize = 10;

string test(int nums[], size_t size = numSize);		// 非局部变量

// 其求值过程发生在函数调用时
string test()
{
    size = numSize;
}
```

* 注意：不能通过修改默认实参实现函数重载！

#### 2.6.2 内联函数和`constexpr`函数

* 内敛函数：最好声明在头文件中，用关键字`inline`在函数类型前，用于简单的函数操作，在其调用点展开，用于节省函数调用时间。

```cpp
inline const string &compareShort(const string &str1, const string &str2)
{
    return (s1.size() <= s2.size()) ? s1 : s2;
}
```

#### 2.6.3 `constexpr` 函数的 “双重身份”

* `constexpr`函数：**返回类型**以及所有**参数类型**必须时**字面值常量**：第2章 2.3.4 `constexper`和常量表达式（C++11）

```cpp
// 编译期可计算的函数：求阶乘
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

int main() {
    constexpr int val = factorial(5);  // 编译期计算，结果为120
    int arr[factorial(3)];  // 合法：编译期计算出3! = 6，作为数组大小
    return 0;
}
```

`constexpr` 函数有一个关键特性：它既可以在**编译期**被调用（此时若满足条件则返回常量表达式），也可以在**运行期**被调用（此时返回的是普通值，非常量表达式）。

具体来说：

1. 当 `constexpr` 函数被**常量表达式实参**调用，且函数体内的操作都符合编译期计算的要求时，其返回值是**常量表达式**（可用于需要常量表达式的场景，如数组大小、模板参数等）。
2. 当 `constexpr` 函数被**非常量表达式实参**（如运行时才能确定的变量）调用时，其行为与普通函数一致，返回的是**运行期的值**（非常量表达式）。

##### 举例说明

```cpp
constexpr int add(int a, int b) {
    return a + b;
}

int main() {
    // 情况1：实参是常量表达式，返回值是常量表达式
    constexpr int x = add(2, 3); // 正确：编译期计算，x 是常量表达式
    int arr[add(2, 3)]; // 正确：数组大小需要常量表达式

    // 情况2：实参是变量（非常量表达式），返回值不是常量表达式
    int a = 2, b = 3;
    int y = add(a, b); // 正确：运行期计算，y 是普通变量（非常量表达式）
    // constexpr int z = add(a, b); // 错误：实参是变量，返回值不是常量表达式
}
```

在这个例子中：

- `add(2, 3)` 用常量表达式调用，返回值 `5` 是常量表达式，可用于 `constexpr` 变量初始化或数组大小。
- `add(a, b)` 用变量调用，返回值是运行期计算的 `5`，不是常量表达式，不能用于需要常量表达式的场景。

##### 总结

`constexpr` 函数的 “`constexpr`” 修饰符仅表示它**有能力在编译期返回常量表达式**，但并非 “必须返回常量表达式”。其返回值是否为常量表达式，完全取决于调用时的实参是否为常量表达式以及函数体内的操作是否支持编译期计算。这一特性让 `constexpr` 函数兼具灵活性（可在运行期调用）和编译期计算能力。

#### 2.6.4 `assert`预处理宏

位置：`assert.h`中。用于检查“不能发生”的条件。

### 2.7 函数匹配

```cpp
void f();
void f(int);
void f(int, int);
void f(double, double = 3.14);

f(3.14);			// void f(double, double = 3.14);
```

#### 2.7.1 函数匹配的过程：

* 确认重载函数集合，其内含的被称为候选函数（*candition function*）。特征：函数名相同，调用点可见

* 检查实参：确认实参数量是否和形参相同，确认参数类型是否匹配或可以转换为形参类型。

* 多个形参的函数匹配成功的条件：

  * 每个实参的匹配都不劣于其他函数。（每个实参都不差）
  * 至少有一个实参匹配优于其他函数。（至少有一个实参是匹配的）

  ```cpp
  void f(int, int);
  void f(double, double = 3.14);
  
  f(3, 3.14);		// 有调用二义性。
  ```

#### 2.7.2 实参类型转换等级

* 精确匹配。
* 通过`const`类型转换实现的匹配。
* 通过类型提示实现的匹配。
* 通过算数类型转换或指针类型转换实现的匹配。
* 通过类类型转换实现的匹配。

### 2.8 函数指针

其中有关函数指针可参考

《C Traips and Pitfalls》读书笔记：**2.2.1 指针和类型转换**

《C Prime Plus》第14章读书笔记：**2.6 函数和指针**

#### 2.8.1 `decltype`返回函数类型

```cpp
typedef bool Func(const string&, const string&);
decltype(Func) Func2;							// 等价类型

typedef bool (*FuncP)(const string&, const string&);
decltype(Func) *Funcp2;							// 注意这里是Func，等价类型
```

#### 2.8.2 返回指向函数的指针

* 传统类型：

```cpp
Type (*function(arguments))(return_argment);

int (*function(int))(int, int);
```

* 通过`typedef`和`using`定义

```cpp
typedef int (*PF)(int, int);
using PF1 = int (*)(int, int);

PF function(int);
PF1 function(int);
```

* 通过尾置类型说明：

```cpp
auto function(int) -> int (*)(int, int);
```

* `decltype`返回函数类型

```cpp
int sum(int, int);
decltype(sum) *fun(int);
```

## 三、小结

本章主要介绍`C++`中函数的概念以及各位形参、返回值如何处理、特有属性（默认形参、内联和`constexpr`）、重载于形参与实参的匹配。

## 四、参考内容

* 有关函数指针可参考

《C Traips and Pitfalls》读书笔记：**2.2.1 指针和类型转换**

《C Prime Plus》第14章读书笔记：**2.6 函数和指针**

* `constexpr`函数：**返回类型**以及所有**参数类型**必须时**字面值常量**：

第2章 2.3.4 `constexper`和常量表达式（C++11）