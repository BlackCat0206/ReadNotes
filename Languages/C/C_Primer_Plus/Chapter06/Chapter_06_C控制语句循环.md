# Chapter_06_C控制语句:循环

***---- Li Jinsheng***

## 一、概述

本章为《C Primer Plus》（第6版）C控制语句：循环读书笔记

注：本章内容不类似于前五章注重于书本概念的记录，更多注重于笔者自身不曾理解的内容于工作中的经验。对书本中的程序清单，会挑选重点、有代表性的进行摘录。

## 二、内容

### 2.1 `while`循环

`while`循环格式：

```c
while( expression )
{
    statement;
}
```

* 若`expression`为真（第5章有说明，每个表达式都有一个值。C语言中 0为假，非0的数字为真），while循环执行；若为假，结束循环。
* 因为执行`while`前需要执行`expression`，故`while`也被称为入口条件（*entry condition*）循环（了解）。
* 注：若出现`while(expression);`以分号结尾，其中的表达式依然会执行，直到结果是假。（考试题中经常出现）

### 2.2 相等运算符和`scanf()`函数拓宽

* 在C中`==`被称为相等运算符，用来判断左右两边运算对象是否相等。
* `scanf()`会返回成功读取项的数量，这一点在书本的用例中比较常见。工作中对于`scanf()`的使用比较少，更多的是按照规定的内部协议使用`JSON`字段，更加灵活。

### 2.3 关系运算符

* 运算优先级：仅高于赋值运算符。
* 不要用关系运算符比较字符串! （工作中代码可以不那么潇洒，但一定要保证可读性、稳定性。查bug比写代码痛苦！！！）
* 浮点类型比较尽量使用`>`和`<`，第二章中有提到浮点类型有其对应的有效数字，故可能看似相同的浮点数，在计算机中比较结果可能会不同。
* 浮点类型比较也可以使用`math.h`中函数`fabs()`函数，示例如下：

```c
// cmpflt.c -- floating-point comparisons
#include <math.h>
#include <stdio.h>
int main(void)
{
    const double ANSWER = 3.14159;
    double response;
    
    printf("What is the value of pi?\n");
    scanf("%lf", &response);
    while (fabs(response - ANSWER) > 0.0001)		// fabs()函数的使用
    {
        printf("Try again!\n");
        scanf("%lf", &response);
    }
    printf("Close enough!\n");
    
    return 0;
}

/*
Result:
What is the value of pi?
2
Try again!
4
Try again!
3.14159
Close enough!
*/
```

[^程序清单 6.5]: cmpflt.c

#### 2.3.1 什么是真

* 表达式、计算结果或计算结果为0则是假，非0均被判为真。
* 在日常开发中，不要将其他对象（例：整数、字符、指针）当作`_Bool`类型直接在条件判断中直接使用，都要说明**判断条件**。（代码规范）例如：

```c
if (NULL != pTest){
    statement;
}

if (0 != iTest){
    statement;
}
```

* 在进行条件判断判读时，为了规避将赋值运算符（`=`）和比较运算符（`==`）写混，将常量写在比较运算符的左侧（编译器不允许给常量赋值，用编译器作为最后的拦截）。规避以下示例的错误。

```c
// trouble.c -- misuse of =
// will cause infinite loop
#include <stdio.h>
int main(void)
{
    long num;
    long sum = 0L;
    int status;
    
    printf("Please enter an integer to be summed ");
    printf("(q to quit): ");
    status = scanf("%ld", &num);
    while (status = 1)									// 赋值表达式的值时赋值运算符左侧的值，即1。while进入死循环。
    {
        sum = sum + num;
        printf("Please enter next integer (q to quit): ");
        status = scanf("%ld", &num);
    }
    printf("Those integers sum to %ld.\n", sum);
    
    return 0;
}

/**
Please enter an integer to be summed (q to quit):3
Please enter next integer (q to quit):q
Please enter next integer (q to quit):
Please enter next integer (q to quit):
Please enter next integer (q to quit):
... ...
*/
```

#### 2.3.2 新`_Bool`类型

* C99提供`stdbool.h`头文件，该文件将`bool`作为`_Bool`类型的别名，还把`true`和`false`分别定义为`1`和`0`，可以写出兼容`C++`的代码。

#### 2.3.3 关系运算符优先级

* 关系运算符中`!=`和`==`优先级**低于**其他比较运算符。

| 运算符                         | 结合律   |
| ------------------------------ | -------- |
| （）                           | 从左向右 |
| -、+（一元）、--、++、`sizeof` | 从右向左 |
| *、/                           | 从左向右 |
| - + （二元）                   | 从左向右 |
| < > >= <=                      | 从左向右 |
| == !=                          | 从左向右 |
| =                              | 从右向左 |

### 2.4 `for`循环

`for`循环格式：

```c
for (initialize; test; update)
{
    statemenet;
}
```

* 在`initialize`中初始化控制变量（变量在`for`外声明，`c++`可以在这里声明并初始化）。
* `test`进行表达式求值，若为真则执行一次循环。
* `update`更新条件。

### 2.5 逗号运算符

* 逗号运算符（`,`）类似序列点（*sequence point*），所有的计算必须在次之前完成，即它的计算顺序是**从左向右**。即在逗号前完成副作用（*side effect*）。

```c
ounces++, cost = ounces * 1;	// 这里执行右侧表达式时，已经完成了左侧的递增。
```

* 逗号表达式的值：逗号表达式的值是其**右侧的值**。

```c
x = (y = 3, z = ( ++y + 2) + 5);

// z = ((4 + 2) + 5)
// z = (6 + 5)
// z = 11
// x = 11
```

* **重点：区分表达式和逗号运算符。**

```c
x = 249,500;	// x = 249 整个表达式的值是500

x = (249,500);	// x = 500 这里x被赋值的是右侧整个逗号表达式的值。
```

### 2.6 出口条件循环：`do while`

* `do whie`循环会在出循环时判断条件，故被称为出口条件循环（*exit-condition loop*）。（了解）

* 格式：

```c
do
{
    statement;
}while(expression);		// 以分号结尾
```

* 其可以保证循环内容至少被执行一次。
* 工作中见到此类写法：多用于**宏定义**，保证宏展开可以执行一次，同时**使用时加分号**结尾保证语法正确。

```c
/*
do
{
    statement;
}while(0);
*/


#define SAFE_MACRO(x) do { \
    printf("Value: %d\n", (x)); \
} while(0)

SAFE_MACRO(10);
```

### 2.7 数组简介

* 数组（*array*）：有序的存放这相同类型元素的序列。通过下标访问其中**元素（*element*）**。下标（subscript）、索引（*indice*）或偏移（*offset*）。后续开发中可以使用上述三者对下标进行起名，工作中常用的是*index*。
* 访问越界：C编译器不会检查数组的下标是否正确，故在使用数组时要首先保证访问有效的下标。若使用的下标已经超过数组本身大小，可能会出现各种未知情况（踩内存，访问空地址，数据污染等相关连锁反应），导致功能异常或直接死机。

## 三、小结

本章主要介绍了C语言中三种循环逻辑，开发者可根据其对应特性选择使用。循环嵌套在开发中建议添加注释说明循环内容，增强代码可读性，毕竟人无法向机器一样直接看出循环的结果。C99新增了`stdbool.h`定义了`bool`、`true`和`false`使其可以兼容`c++`。逗号运算符相当一个序列点，保证从左向右的计算顺序。逗号表达式的值是逗号右边的值。对于数组的使用，要注意下标的边界范围，若访问越界，会直接导致功能异常或死机。