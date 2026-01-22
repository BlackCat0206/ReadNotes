# 语句

***---- Li Jinsheng***

***

## 一、概述

本篇为《C++ Primer》（第5版）第5章读书笔记，主要内容为各类条件、循环语句。整体内容与《C++ Primer Plus》（第6版）**第6章**与**第7章**内容相同。本篇重点在于`C++`中一些特性与之前`C`中未注意的地方。

## 二、内容

### 2.1 条件语句

#### 2.1.1 悬垂`else`(*dangling else*)

条件语句中有多个`if`和`else`的组合，在只使用缩进的情况下，无法区分`else`匹配的`if`。

匹配规则：`else`会与最近的一个未匹配`else`的`if`语句匹配。

#### 2.1.2 `switch`内声明变量

* `case`标签：本质是跳转标记，不是域作用符。故若是在一个`case`中声明变量，在其他的`case`中使用是可以的。但是，这会出现直接跳过声明变量而直接使用的情况，这种情况是绝对禁止的。
* 在工作中最好都是使用块作用域（`{}`）。

```cpp
// 错误示例
switch (x) {
case 1:
    int a = 10;  // 带初始化的变量
    break;
case 2:  // 错误：跳转跨过了 a 的初始化
    a = 20;  // 若 x=2，程序直接跳转到这里，a 从未被初始化
    break;
}

// 正确示例
switch (x) {
case 1: {
    int a = 10;  // 变量在块作用域内初始化
    break;
}
case 2:  // 合法：此时 a 的作用域已被块限制，跳转不会跨过其初始化
    // 这里无法访问 a，避免了未初始化的问题
    break;
}
```

### 2.2 迭代语句

#### 2.2.1 传统的`for`语句

相较于`C`中C99引入在`for`循环中可以声明并定义循环变量，C++中可以在`for`循环中声明并定义循环变量

```cpp
for (decltype(v.size()) i = 0; i < v.size(); ++i)
{
    statement;
}
```

#### 2.2.2 范围`for`语句（不能使用其给`vector`添加元素的原因）

```cpp
for (declaration : expression)
{
    statement;
}
```

* `expression`：它必须是一个**序列**。花括号组成的初始化列表、数组、`vector`或`string`等，它**必须要能返回迭代器的`begin`和`end`**。
* `declaration`：声明并定义一个变量。序列中的元素都可转换为该变量的类型。可以用`auto`。
* **每次迭代都会重新定义循环变量**。

```cpp
vector<int> v = {0,1,2,3,4,5,6,7,8,9};
for (auto &r : v)
{
    r *= 2;
}
```

上述示例也等价为：

```cpp
for (auto beg = v.begin(), end = v.end; beg != end; ++beg)
{
    auto &r = *beg;
    r *= 2;
}
```

* 这就是第3章（2.3.2 向`vector`中添加元素）中强调的**不能使用范围`for`循环对`vector`添加元素**，根据上述的示例结合第3章**迭代器有效性**（2.4.4 迭代器的有效性），当`vector`的大小发生变化，其原先的迭代器可能会**失效**的。

### 2.3 `try`语句和异常处理

异常检测：`throw`语句，其后跟一个表达式，表达式的类型标识异常的类型。

异常检查：`try`语句，以`try`语句开始，以一个或多个`catch`子句（*catch clause*）结束。`try`中抛出的异常，在`catch`子句中处理。

#### 2.3.1 `throw`表达式

`stdexpect`头文件中有定义通用的异常表达式。

```cpp
if (...)
{
    thorw runtime_error("Data must refer to same ISBN.");
}
```

`runtime_error`就是标准库`stdexpect`一种类型之一，其初始化可以使用`string`或C风格字符串。

#### 2.3.2 `try`语句块

```cpp
try
{
    program-statement;
}
catch (exception-declaration)	// 异常声明
{
    handle-statementl
}
catch (exception-declaration)
{
    handle-statementl
}	// ... ...
```

* `try`语句块内声明的变量在**外部是无法使用**的，特别是在`catch`中也**无法使用**。

* `try`语句块中可能会调用另一个含有`try`语句块的函数，若抛出异常，则先从该函数寻找，若未找到，则终止该函数，并在调用该函数的函数中寻找，若未找到则终止调用函数依次向上。最终若还是未找到，会执行标准库函数`terminate`。

简单示例（练习5.25）

```cpp
void test_try_throw2(void)
{
    // get two numbers from standard input
    int v1 = 0, v2 = 0;
    while(cin >> v1 >> v2)
    {
        try
        {
            if (v2 == 0)
            {
                throw std::runtime_error("divisor is 0");
            }
        }
        catch (std::runtime_error err)
        {
            cout << err.what() << std::endl;
            cout << "Try again? (y/n)" << std::endl;
            char c;
            cin >> c;
            if (!cin || c == 'n')
            {
                break;
            }
            continue;
        }
        cout << v1 / v2 << std::endl;
    }
    return;
}
```

#### 2.3.4 标准异常

* stdexcept定义的异常

| 异常标识           | 含义说明                                       |
| ------------------ | ---------------------------------------------- |
| exception          | 常见的问题                                     |
| `runtime_error`    | 运行时检测出问题                               |
| `range_error`      | 运行时检测出问题，计算结果超出有意义的值范围   |
| `overflow_error`   | 运行时错误，计算上溢                           |
| `underflow_error`  | 运行时错误，计算下溢                           |
| `logic_error`      | 程序逻辑错误                                   |
| `domain_error`     | 逻辑错误，参数对应结果不存在                   |
| `invalid_argument` | 逻辑错误，无效参数                             |
| `length_error`     | 逻辑错误，试图创建一个超出该类型最大长度的对象 |
| `out_of_range`     | 逻辑错误，使用一个超过有效范围的值             |

* `exception`头文件定义了最通用的异常类型。
* `new`头文件中定义了`bad_calloc`异常类型，（在第12章中介绍）。
* `type_info`头文件定义了`bad_cast`异常类型。

## 三、小结

本章主要是各类循环语句，整体内容与`C`中一致。C++中异常处理（此类实现较为困难，开发者很多时候对错误是无法预估的。）

主要说明问题：为什么不能用范围`for`循环为`vector`添加元素。