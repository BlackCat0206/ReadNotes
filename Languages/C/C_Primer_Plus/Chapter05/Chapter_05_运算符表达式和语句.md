# Chapter_05_运算符表达式和语句

***---- Li Jinsheng***

***

## 一、概述

本章用于记录《C Prime Plus》（第6版）第五章读书笔记。

***

## 二、内容

### 2.1 循环简介

```c
/* shoes1.c -- converts a shoe size to inches */
#include <stdio.h>
#define ADJUST 7.31              // one kind of symbolic constant
int main(void)
{
    const double SCALE = 0.333;  // another kind of symbolic constant
    double shoe, foot;
    
    shoe = 9.0;
    foot = SCALE * shoe + ADJUST;
    printf("Shoe size (men's)    foot length\n");
    printf("%10.1f %15.2f inches\n", shoe, foot);
    
    return 0;
}

/*
Result:
Shoe size (men's)    foot length
       9.0           10.31 inches
*/
```

[^程序清单 5.1]: shoes1.c

* 计算穿9码男鞋的脚长（单位：英尺）。上述程序只能进行单一的计算，若写完交互程序，无法利用计算机的优势，应该让计算机去做重复计算的重复的计算工作，这里先简单介绍下`while循环`。

```c
/* shoes2.c -- calculates foot lengths for several sizes */
#include <stdio.h>
#define ADJUST 7.31              // one kind of symbolic constant
int main(void)
{
    const double SCALE = 0.333;  // another kind of symbolic constant
    double shoe, foot;
    
    printf("Shoe size (men's)    foot length\n");
    shoe = 3.0;
    while (shoe < 18.5)      /* starting the while loop */
    {                        /* start of block          */
        foot = SCALE * shoe + ADJUST;
        printf("%10.1f %15.2f inches\n", shoe, foot);
        shoe = shoe + 1.0;
    }                        /* end of block            */
    printf("If the shoe fits, wear it.\n");
    
    return 0;
}

/*
Result:
Shoe size (men's)    foot length
       3.0            8.31 inches
       4.0            8.64 inches
       5.0            8.97 inches
       6.0            9.31 inches
       7.0            9.64 inches
       8.0            9.97 inches
       9.0           10.31 inches
      10.0           10.64 inches
      11.0           10.97 inches
      12.0           11.31 inches
      13.0           11.64 inches
      14.0           11.97 inches
      15.0           12.31 inches
      16.0           12.64 inches
      17.0           12.97 inches
      18.0           13.30 inches
*/
```

[^程序清单 5.2]: shoes2.c

* 程序第一次到达`while`循环时，会先判断其中的条件是否位真。
* 当第一次循环执行结束后，会返回第一个判断语句。
* 使用花括号（`{}`）括起来的内容，即为`while`循环的内容，也被称为代码块（*block*）。

### 2.2 基本运算符

C语言用运算符（*opertator*）表示算数运算。我们简单介绍下基本算数运算符：=、+、-、*、/。（C没有指数运算，C的标准库提供了一个`pow()`函数进行指数运算，例（2.5， 3）即返回2.5的3次幂）。

#### 2.2.1 赋值运算符：=

```c
int bmw = 2002;
```

上述示例中：把2002赋值给变量bmw。赋值行为是：**从右向左执行的**。

```c
int i = 1;
i = i + i;
```

上述示例表示：先找到变量i的值，将其+1在存放在变量i中。

```c
2002 = bmw;
```

注：上述的语句是错误的，因为`2002`是一个**右值（*rvalue*）**，它是一个字面常量，常量本身就是它的值。赋值运算符的左侧应该是一个可以引用其内存的量，被称作**可修改左值（*modifiable lvalue*）**。

##### 2.2.1.1 左值、右值、数据对象和运算符。

* 赋值表达式的目的，是将值存放在对应的内存中。这里存放值的数据区域被称为**数据对象（*data object*）**。
* 左值（*lvalue*）：标识特定对象的名称或表达式。**对象指的是数据存储**，而左值是用于定位或标识**存储位置**的**标签**。
* 左值的特性：1.用来**标识特定的数据存储位置**，可以**引用内存中的地址**。2.可以放在**赋值运算符的左侧**。
* 可修改左值（*modifiable lvalue*）：因后续C语言 引入了`const`限定符，它可以满足条件一，但不能满足条件条件二。为此C语言新增了术语可修改左值。
* 右值（*rvalue*）：能给可修改左值赋值的量，且本身不是左值。它可以是：常量、变量、或者其他可求值表达式（例：两个变量i，j的加法运算表达式：i+j是一个右值）的值。

```c
/* golf.c -- golf tournament scorecard */
#include <stdio.h>
int main(void)
{
    int jane, tarzan, cheeta;
    
    cheeta = tarzan = jane = 68;
    printf("                  cheeta   tarzan    jane\n");
    printf("First round score %4d %8d %8d\n",cheeta,tarzan,jane);
    
    return 0;
}

/*
Result:
                  cheeta   tarzan    jane
First round score   68       68       68
*/
```

* C语言可以实现多重赋值，执行顺序为：从右向左。

#### 2.2.2 加法运算符：+

* 加法运算符（*addition opertaor*）：用于加法运算，将两侧的值相加。

#### 2.2.3 减法运算符：-

* 减法运算符（*subtraction operator*）：用于减法运算，将其左侧数减去右侧的数。
* `+`和`-`被称为**二元运算符**（*binary operator*），即这些运算符都需要两个运算对象才能操作。

#### 2.2.4 符号运算符：-和+

* `-`除了用于减法运算，还可以用来修改一个值的代数符号。

```c
int i = -12;
int j = -i;
```

* 此类运算符也被称为**一元运算符（*unary operator*）**，它只需要一个运算对象就能操作。
* C90新增了一元`+`运算符，**它不会改变运算对象的符号值**。

#### 2.2.5 乘法运算符：*

* 符号`*`表示乘法。

```c
/* squares.c -- produces a table of first 20 squares */
#include <stdio.h>
int main(void)
{
    int num = 1;
    
    while (num < 21)
    {
        printf("%4d %6d\n", num, num * num);
        num = num + 1;
    }
    
    return 0;
}

/*
Result:
   1      1
   2      4
   3      9
   4     16
   5     25
   6     36
   7     49
   8     64
   9     81
  10    100
  11    121
  12    144
  13    169
  14    196
  15    225
  16    256
  17    289
  18    324
  19    361
  20    400
*/
```

[^程序清单 5.4]: squares.c

上述示例程序用来依次打印1~20的数值对应的平方。

```c
/* wheat.c -- exponential growth */
#include <stdio.h>
#define SQUARES 64             // squares on a checkerboard
int main(void)
{
    const double CROP = 2E16;  // world wheat production in wheat grains
    double current, total;
    int count = 1;
    
    printf("square     grains       total     ");
    printf("fraction of \n");
    printf("           added        grains    ");
    printf("world total\n");
    total = current = 1.0; /* start with one grain   */
    printf("%4d %13.2e %12.2e %12.2e\n", count, current,
           total, total/CROP);
    while (count < SQUARES)
    {
        count = count + 1;
        current = 2.0 * current;
        /* double grains on next square */
        total = total + current;     /* update total */
        printf("%4d %13.2e %12.2e %12.2e\n", count, current,
               total, total/CROP);
    }
    printf("That's all.\n");
    
    return 0;
}

/**
Result:
square     grains       total     fraction of 
           added        grains    world total
   1     1.00e+000    1.00e+000    5.00e-017
   2     2.00e+000    3.00e+000    1.50e-016
   3     4.00e+000    7.00e+000    3.50e-016
   4     8.00e+000    1.50e+001    7.50e-016
   5     1.60e+001    3.10e+001    1.55e-015
   6     3.20e+001    6.30e+001    3.15e-015
   7     6.40e+001    1.27e+002    6.35e-015
   8     1.28e+002    2.55e+002    1.27e-014
   9     2.56e+002    5.11e+002    2.55e-014
  10     5.12e+002    1.02e+003    5.12e-014
  11     1.02e+003    2.05e+003    1.02e-013
  12     2.05e+003    4.10e+003    2.05e-013
  13     4.10e+003    8.19e+003    4.10e-013
  14     8.19e+003    1.64e+004    8.19e-013
  15     1.64e+004    3.28e+004    1.64e-012
  16     3.28e+004    6.55e+004    3.28e-012
  17     6.55e+004    1.31e+005    6.55e-012
  18     1.31e+005    2.62e+005    1.31e-011
  19     2.62e+005    5.24e+005    2.62e-011
  20     5.24e+005    1.05e+006    5.24e-011
  21     1.05e+006    2.10e+006    1.05e-010
  22     2.10e+006    4.19e+006    2.10e-010
  23     4.19e+006    8.39e+006    4.19e-010
  24     8.39e+006    1.68e+007    8.39e-010
  25     1.68e+007    3.36e+007    1.68e-009
  26     3.36e+007    6.71e+007    3.36e-009
  27     6.71e+007    1.34e+008    6.71e-009
  28     1.34e+008    2.68e+008    1.34e-008
  29     2.68e+008    5.37e+008    2.68e-008
  30     5.37e+008    1.07e+009    5.37e-008
  31     1.07e+009    2.15e+009    1.07e-007
  32     2.15e+009    4.29e+009    2.15e-007
  33     4.29e+009    8.59e+009    4.29e-007
  34     8.59e+009    1.72e+010    8.59e-007
  35     1.72e+010    3.44e+010    1.72e-006
  36     3.44e+010    6.87e+010    3.44e-006
  37     6.87e+010    1.37e+011    6.87e-006
  38     1.37e+011    2.75e+011    1.37e-005
  39     2.75e+011    5.50e+011    2.75e-005
  40     5.50e+011    1.10e+012    5.50e-005
  41     1.10e+012    2.20e+012    1.10e-004
  42     2.20e+012    4.40e+012    2.20e-004
  43     4.40e+012    8.80e+012    4.40e-004
  44     8.80e+012    1.76e+013    8.80e-004
  45     1.76e+013    3.52e+013    1.76e-003
  46     3.52e+013    7.04e+013    3.52e-003
  47     7.04e+013    1.41e+014    7.04e-003
  48     1.41e+014    2.81e+014    1.41e-002
  49     2.81e+014    5.63e+014    2.81e-002
  50     5.63e+014    1.13e+015    5.63e-002
  51     1.13e+015    2.25e+015    1.13e-001
  52     2.25e+015    4.50e+015    2.25e-001
  53     4.50e+015    9.01e+015    4.50e-001
  54     9.01e+015    1.80e+016    9.01e-001
  55     1.80e+016    3.60e+016    1.80e+000
  56     3.60e+016    7.21e+016    3.60e+000
  57     7.21e+016    1.44e+017    7.21e+000
  58     1.44e+017    2.88e+017    1.44e+001
  59     2.88e+017    5.76e+017    2.88e+001
  60     5.76e+017    1.15e+018    5.76e+001
  61     1.15e+018    2.31e+018    1.15e+002
  62     2.31e+018    4.61e+018    2.31e+002
  63     4.61e+018    9.22e+018    4.61e+002
  64     9.22e+018    1.84e+019    9.22e+002
*/
```

[^程序清单 5.5]: wheat.c

上述程序表示指数增长。

#### 2.2.6 除法运算符：/

C使用符号`/`表示除法运算，其左侧为被除数，右侧为除数。

注：浮点数运算和整数的除法运算有所不同，**整数的除法运算小数部分会被丢弃**，这一操作被称为**截断（*truncation*）**。

```c
/* divide.c -- divisions we have known */
#include <stdio.h>
int main(void)
{
    printf("integer division:  5/4   is %d \n", 5/4);
    printf("integer division:  6/3   is %d \n", 6/3);
    printf("integer division:  7/4   is %d \n", 7/4);
    printf("floating division: 7./4. is %1.2f \n", 7./4.);
    printf("mixed division:    7./4  is %1.2f \n", 7./4);
    
    return 0;
}

/*
Result:
integer division:  5/4   is 1 
integer division:  6/3   is 2 
integer division:  7/4   is 1 
floating division: 7./4. is 1.75 
mixed division:    7./4  is 1.75 
*/
```

[^程序清单 5.6]: divide.c

* 整数的除法运算小数部分会丢弃，且不会**四舍五入**。
* 整数和浮点数的混合运算的结果是**浮点数**。实际由编译器完成，编译器将两个运算对象转换为同一个类型，再进行计算。
* C99之前，C语言给开发者提供了选项用于决定**负数**的整数除法。一种是四舍五入（例：-3.8会被四舍五入为-4），另一种则是直接丢弃小数部分（例：-3.8的结果会是-3）这种方式被称为**趋零截断**。C99之后，规定使用**趋零截断**。

#### 2.2.7 运算符优先级

`butter = 25.0 + 60.0 * n / SCALE;`

`butter = (25.0 + 60.0) * n / SCALE;`

上述语句如何执行取决于操作顺序，C语言明确规定，使用运算符优先级来解决操作顺序的问题。

当前学习的运算符优先级为：从高到低

| 运算符       | 结合律   |
| ------------ | -------- |
| （）         | 从左向右 |
| -- ++        | 从右向左 |
| -、+（一元） | 从右向左 |
| *、/         | 从左向右 |
| - + （二元） | 从左向右 |
| =            | 从右向左 |

#### 2.2.8 优先级和求值顺序

运算符优先级虽然提供了计算的依据，但是并没有完全规定所有的顺序

`y = 6 * 12 + 5 * 20;`

在上述示例中`*`优先级大于`+`，所有优先计算乘法，但是并未规定先计算前面的乘法还是后面的乘法。这类问题由开发者更具不同的硬件自行抉择。

`12 / 3 * 2`

* **结合律**只适用于**共享**同一算数对象的运算符。在上述例子中，乘法和除法是同一优先级，`3`就是被共享的对象。这时`从左向右`的结合律就发挥了作用。从左向右计算的结果是8，若是从右向左计算的结果是：2。

```c
/* rules.c -- precedence test */
#include <stdio.h>
int main(void)
{
    int top, score;
    
    top = score = -(2 + 5) * 6 + (4 + 3 * (2 + 3));
    printf("top = %d, score = %d\n", top, score);
    
    return 0;
}

/*
Result:
top = -23, score = -23
*/
```

[^程序清单 5.7]: rules.c

### 2.3 其他运算符

C语言大约有40种运算符，本章再来介绍4个比较常用的。

#### 2.3.1 `sizeof`运算符和`size_t`类型

* `sizeof`运算符：以字节为单位返回运算对象的大小。运算对象可以是数据对象（例：变量名）或数据类型。如果运算对象是类型，则必须用圆括号括起来。

```c
// sizeof.c -- uses sizeof operator
// uses C99 %z modifier -- try %u or %lu if you lack %zd
#include <stdio.h>
int main(void)
{
    int n = 0;
    size_t intsize;
    
    intsize = sizeof (int);
    printf("n = %d, n has %zd bytes; all ints have %zd bytes.\n",
           n, sizeof n, intsize );
    
    return 0;
}

/**
Result:
n = 0, n has 4 bytes; all ints have 4 bytes.
*/
```

[^程序清单 5.8]: sizeof.c

* `size_t`类型：C语言规定，`sizeof`返回`size_t`类型的值。是一个**无符号整形**。
* C99 做了进一步调整，新增`%zd`转换说明用于`printf()`函数中打印`size_t`类型，若系统缺少，可以使用`%u`或`lu`。

#### 2.3.2 求模运算符：`%`

* 求模运算符（*modulus operator*）用于**整数运算（不能用于浮点型）**。给出左侧整数除以右侧整数的**余数**。

```c
// min_sec.c -- converts seconds to minutes and seconds
#include <stdio.h>
#define SEC_PER_MIN 60            // seconds in a minute
int main(void)
{
    int sec, min, left;
    
    printf("Convert seconds to minutes and seconds!\n");
    printf("Enter the number of seconds (<=0 to quit):\n");
    scanf("%d", &sec);            // read number of seconds
    while (sec > 0)
    {
        min = sec / SEC_PER_MIN;  // truncated number of minutes
        left = sec % SEC_PER_MIN; // number of seconds left over
        printf("%d seconds is %d minutes, %d seconds.\n", sec,
               min, left);
        printf("Enter next value (<=0 to quit):\n");
        scanf("%d", &sec);
    }
    printf("Done!\n");
    
    return 0;
}

/*
Result:
Convert seconds to minutes and seconds!
Enter the number of seconds (<=0 to quit):
59
59 seconds is 0 minutes, 59 seconds.
Enter next value (<=0 to quit):
127
127 seconds is 2 minutes, 7 seconds.
Enter next value (<=0 to quit):
0
Done!
*/
```

[^程序清单 5.9]: min_sec.c

* 负数求模：C99的趋零截断之前，情况各有不同。自从有此规定后，其结果为，若第一个运算对象是正数，则求模的结果是整数，若一个运算对象是负数，则求模的结果是负数。
* **结果符号和求模数保持一致**。

```c
-11 % 5 = -1;
```

* 若不支持C99，标准由规定，只要a和b是整数，则使用`a - (a/b)*b`来计算`a%b`。例如：-11%5

`-11 - (-11 / 5) * 5 = -11 - (-2) * 5  = -11 - (-10) = -1`

#### 2.3.3 递增运算符 ：`++`

*  递增运算符（*increment operator*）：将其运算对象递增1。
* 递增运算符的工作模式：
  * 前缀模式：递增运算符在运算对象前。（例：++i）其结果是**左值**。
  * 后缀模式：递增运算符在运算对象后。（例：i++）其结果是**右值**。

```c
/* add_one.c -- incrementing: prefix and postfix */
#include <stdio.h>
int main(void)
{
    int ultra = 0, super = 0;
    
    while (super < 5)
    {
        super++;
        ++ultra;
        printf("super = %d, ultra = %d \n", super, ultra);
    }
    
    return 0;
}

/*
Result:
super = 1, ultra = 1 
super = 2, ultra = 2 
super = 3, ultra = 3 
super = 4, ultra = 4 
super = 5, ultra = 5 
*/
```

[^程序清单 5.10]: add_one.c

* 递增运算符的优点：
  * 简化代码，增加可读性。
  * 此类写法生成的机器语言效率更高。（最新的智能编译器已经可以实现将 i = i + 1，转为 ++i，故此类优势可能不在那么明显）
* 工作模式的区别：

```c
/* post_pre.c -- postfix vs prefix */
#include <stdio.h>
int main(void)
{
    int a = 1, b = 1;
    int a_post, pre_b;
    
    a_post = a++;  // value of a++ during assignment phase
    pre_b = ++b;   // value of ++b during assignment phase
    printf("a  a_post   b   pre_b \n");
    printf("%1d %5d %5d %5d\n", a, a_post, b, pre_b);
    
    return 0;
}

/*
Result:
a  a_post   b   pre_b 
2     1     2     2
*/
```

[^程序清单 5.11]: post_pre.c

* 前缀模式：使用运算对象前，先递增运算对象。
* 后缀模式：使用运算对象后，再递增运算对象。

#### 2.3.4 递减运算符：`--`

* 递减运算符（*decrement operator*）：对运算对象进行递减，其工作模式同递增相同：
  * 前缀模式：递增运算符在运算对象前。（例：--i）其结果是左值。
  * 后缀模式：递增运算符在运算对象后。（例：i--）其结果是右值。

```c
#include <stdio.h>
#define MAX 100
int main(void)
{
    int count = MAX + 1;
    
    while (--count > 0) {
        printf("%d bottles of spring water on the wall, "
               "%d bottles of spring water!\n", count, count);
        printf("Take one down and pass it around,\n");
        printf("%d bottles of spring water!\n\n", count - 1);
    }
    
    return 0;
}

/*
Result:
100 bottles of spring water on the wall, 100 bottles of spring water!
Take one down and pass it around,
99 bottles of spring water!

99 bottles of spring water on the wall, 99 bottles of spring water!
Take one down and pass it around,
98 bottles of spring water!

... ...

2 bottles of spring water on the wall, 2 bottles of spring water!
Take one down and pass it around,
1 bottles of spring water!

1 bottles of spring water on the wall, 1 bottles of spring water!
Take one down and pass it around,
0 bottles of spring water!
*/
```

[^程序清单 5.12]: bottles.c

* 示例中的`>`为大于运算符，`<`是小于运算符。他们都是**关系运算符**（第6章会阐述）。

#### 2.3.5 优先级

* 递增运算符和递减运算符的优先级，只有圆括号高于它们。

#### 2.3.6 不要自作聪明

* C语言中，**编译器可以自行选择对函数中的哪个参数求值**。即对运算符两侧用的求值顺序是不固定的。

`n = 3;`

`y = n++ + n++;`

`array[++i] = array[i++] + array[i];` 这种逆天语句建议想都不要想。

上述例子的结果是不明确的，编译器可能先使用2次n的旧值，再递增两次，即y=6, n=5。编译器也有可能先使用一次n的旧值，递增再求和，再递增，那么结果是y=7， n = 5。两种方法都是可行的。

建议规则：

* 一个变量出现在**一个函数的多个参数**中，不要对其使用递增或递减运算符。
* 一个变量出**多次出现在一个表达式**中，不要对其使用递增或递减运算符。

### 2.4 表达式和语句

#### 2.4.1 表达式

表达式（*expression*）：由**运算符**和**运算对象**组成。

`4`

`4+21`

`a*(b + c/d)`

`q = p * 5`

`q > 3`

* 表达式可以是常量、变量或二者组合，一些由子表达式（*subexpression*）组成。

**表达式的特性**：每个表达式都有一个值。由赋值运算符的表达式结果与其左侧变量的值相同。`q > 3`此类的结果若为真这是1，若为假则是0。

| 表达式          | 值   |
| --------------- | ---- |
| -4 + 6          | 2    |
| c = 3 + 8       | 11   |
| 5 > 3           | 1    |
| 6 + (c = 3 + 8) | 17   |

* 最后一个虽然合法，但是不建议使用。

#### 2.4.2 语句

语句（*statement*）是C语言的基本构件块。大部分末尾都以分号结尾。

`4`

这就是一个表达式。

`4;`

这就是一个语句（表达式语句）。

* **注意：声明不是表达式语句 （与C++不同）**。

```c
/* addemup.c -- five kinds of statements */
#include <stdio.h>
int main(void)                /* finds sum of first 20 integers */
{
    int count, sum;           /* declaration statement          */
    
    count = 0;                /* assignment statement           */
    sum = 0;                  /* ditto                          */
    while (count++ < 20)      /* while                          */
        sum = sum + count;    /*     statement                  */
    printf("sum = %d\n", sum);/* function statement             */
    
    return 0;                 /* return statement               */
}

```

[^程序清单5.13 ]: addemup.c

* `while`是一个**迭代语句**，有时被称为**结构化语句**。

##### 2.4.2.1 副作用和序列点

**副作用（*side effect*）**：是对**数据对象**或**文件**的修改。

例：y = 50;

在C语言中，看上述例子的主要目的是对**表达式求值**。副作用是对变量`y`进行赋值。

**序列点（*sequence point*）**：程序执行的**点**，在该点上，每一个**副作用**都必须在发生**下一步**之前完成。

例：`;`就标记了一个序列点。

**完整表达式（*full expression*）**：也是一个序列点，指该表达式不是另一个更大表达式的子表达式。

例：5.13中`while`就是一个序列点，保证表示`conut`需要在求和前已经完成了递增。

#### 2.4.3 复合语句

复合语句（*compond statement*）：由花括号括起来的一条或多条语句被称为复合语句，也叫做块（*block*）。

### 2.5 类型转换

通常在表达式中，会使用相同的类型，若使用混合类型，则会进行自动类型类型转换，规则如下：

* **升级（*promote*）**：当类型转换出现在表达式中，无论是`unsigned`、`signed`、`char`、`short`都会被转换为`int`类型。（若`short`类型与`int`类型大小相同，即`unsigned short`会大于`int`类型，这种情况下，会转换为`unsigned int`类型）。
* 涉及两种类型计算时，两个值均转换成其**最高类型**。
* 类型的级别从高到低时：`long double`、`double`、`float`、`unsigend long long`、`long long`、`unsigned long`、`long`、`unsigned int`和`int`。此外若`int`和`long`大小相同，则`unsined int`等级高于`long`。`short`和`char`已经被升级到`int`或`unsigned int`。
* 计算的最终**结果**会被转换为**被赋值变量的类型**。这一过程可能会出现升级（*promote*）或**降级（*demotion*）**，所谓降级，就是将高等级转换为低等级。浮点数降级位整数时，浮点位会被截断（见程序清单5.14：line17、line18）。
* 作为**函数参数传递**时，`short`和`char`都会被转换为`int`，`float`会被转换为`double`。第9章会介绍，函数原型（*function prototype*）会自动覆盖升级。

待赋值与目标值不匹配时，规则如下：

* 目标类型是**无符号整形**，且待赋值的也是**整数**时，额外位会被忽略。例：如果目标类型是8位`unsigned char`类型，则赋值的结果是待赋值求模256。
* 目标类型是**有符号整形**，且待赋值也是**整数**，结果因实现而定。
* 目标类型是一个**整形**，待赋值是一个**浮点数**，结果未定义（补充：浮点数在整数范围内“向零截断”，若超出整数表达范围，其结果时未定义的 ----2025.09.22）。

```c
/* convert.c -- automatic type conversions */
#include <stdio.h>
int main(void)
{
    char ch;
    int i;
    float fl;
    
    fl = i = ch = 'C';                                  /* line 9  */
    printf("ch = %c, i = %d, fl = %2.2f\n", ch, i, fl); /* line 10 */
    ch = ch + 1;                                        /* line 11 */
    i = fl + 2 * ch;                                    /* line 12 */
    fl = 2.0 * ch + i;                                  /* line 13 */
    printf("ch = %c, i = %d, fl = %2.2f\n", ch, i, fl); /* line 14 */
    ch = 1107;                                          /* line 15 */
    printf("Now ch = %c\n", ch);                        /* line 16 */
    ch = 80.89;                                         /* line 17 */
    printf("Now ch = %c\n", ch);                        /* line 18 */
    
    return 0;
}

/**
Result:
ch = C, i = 67, fl = 67.00
ch = D, i = 203, fl = 339.00
Now ch = S
Now ch = P
*/
```

[^程序清单 5.14]: convert.c

#### 2.5.1 强制类型转换符

强制类型转换符（*cast operator*）：圆括号与类型名构成强制类型转换符，说明要转换的变量。（type）

注：在日常开发中应该尽力避免类型混用，无论是自动类型或强制类型，均会对数据精度产生损失。

### 2.6 带参数的函数

```c
/* pound.c -- defines a function with an argument   */
#include <stdio.h>
void pound(int n);   // ANSI function prototype declaration
int main(void)
{
    int times = 5;
    char ch = '!';   // ASCII code is 33
    float f = 6.0f;
    
    pound(times);    // int argument
    pound(ch);       // same as pound((int)ch);
    pound(f);        // same as pound((int)f);
    
    return 0;
}

void pound(int n)    // ANSI-style function header
{                    // says takes one int argument
    while (n-- > 0)
        printf("#");
    printf("\n");
}

/**
Result:
#####
#################################
######
*/
```

[^程序清单 5.15]: pound.c

* 作为函数参数传递时，`short`，`char`

* 实参和形参：英文中`argument`和`parameter`经常互换使用，C99规定`actual argument`和`actual parament`使用术语`argument`（实参）；对于`formal argument`或`formal parameter`使用术语`parameter`（形参）。
* 函数原型（prototype）和函数声明（declaration）的区别：
  * 函数原型：说明函数名、函数返回值类型和参数信息：`void pound(int n);`
  * 函数声明：说明函数名、和返回值类型：`void pound()；`
  * ANSI C之前C使用函数声明，且为了向下兼容，C目前依然支持函数声明的写法。但实际调用中会出现问题。例：5.15中`pound(f)`若为函数声明，则根据自动转换规则，`float`会升级为`double`类型。但输出的结果会出现异常（本地测试输出内容为空）。

### 2.7 示例程序

```c
// running.c -- A useful program for runners
#include <stdio.h>
const int S_PER_M = 60;         // seconds in a minute
const int S_PER_H = 3600;       // seconds in an hour
const double M_PER_K = 0.62137; // miles in a kilometer
int main(void)
{
    double distk, distm;  // distance run in km and in miles
    double rate;          // average speed in mph
    int min, sec;         // minutes and seconds of running time
    int time;             // running time in seconds only
    double mtime;         // time in seconds for one mile
    int mmin, msec;       // minutes and seconds for one mile
    
    printf("This program converts your time for a metric race\n");
    printf("to a time for running a mile and to your average\n");
    printf("speed in miles per hour.\n");
    printf("Please enter, in kilometers, the distance run.\n");
    scanf("%lf", &distk);  // %lf for type double
    printf("Next enter the time in minutes and seconds.\n");
    printf("Begin by entering the minutes.\n");
    scanf("%d", &min);
    printf("Now enter the seconds.\n");
    scanf("%d", &sec);
    // converts time to pure seconds
    time = S_PER_M * min + sec;
    // converts kilometers to miles
    distm = M_PER_K * distk;
    // miles per sec x sec per hour = mph
    rate = distm / time * S_PER_H;
    // time/distance = time per mile
    mtime = (double) time / distm;
    mmin = (int) mtime / S_PER_M; // find whole minutes
    msec = (int) mtime % S_PER_M; // find remaining seconds
    printf("You ran %1.2f km (%1.2f miles) in %d min, %d sec.\n",
           distk, distm, min, sec);
    printf("That pace corresponds to running a mile in %d min, ",
           mmin);
    printf("%d sec.\nYour average speed was %1.2f mph.\n",msec,
           rate);
    
    return 0;
}

/*
Result:
This program converts your time for a metric race
to a time for running a mile and to your average
speed in miles per hour.
Please enter, in kilometers, the distance run.
3
Next enter the time in minutes and seconds.
Begin by entering the minutes.
20
Now enter the seconds.
3
You ran 3.00 km (1.86 miles) in 20 min, 3 sec.
That pace corresponds to running a mile in 10 min, 45 sec.
Your average speed was 5.58 mph.
*/
```

[^程序清单 5.16]: running.c

## 三、小结

* C通过多种运算符来实现各类运算，在使用过程中需要关注运算符的优先级、结合律还有运算对象的数量。当等级相同的运算符共享同一个运算对象时，需要依靠结合律在进行计算。注意在多项运算过程中，由编译器决定先计算哪一项，故在实现计算的过程中内容尽量区分，对复杂运算可以添加注释说明，而不是按照公式实现内容。
* 左值、右值和运算符：赋值语句的目的，是为将值存放在对应的变量中。左值（*lvaue*）可以**标定数据在对应在内存中数据存储位置的标签**（可以与第二章结合起来，声明变量时就是将变量名计算机内存联系起来，编译器根据声明的类型在计算机内存中进行空间分配）。书中将数据存储代指为对象（*object*）。其特性为：可以标定特定的数据对象，可以引用内存中的地址；可以放在赋值符号的左边。因`const`限定符，C99引入可修改左值（*modifiable lvalue*）概念。右值：可以给可修改左值赋值的量。可是是常量，变量或者运算表达式的值。
* 表达式（*expression*）：由运算符和运算对象组成。每个表达式都有一个值。
* 语句（*statement*）：C语言的基本构建块，大部份语句都有分号结尾（;）。由花括号括起来的一条或多条语句称为复合语句（*pound statement*）。
* 副作用（*side effect*）和序列点（*sequence point*）：从C的角度看，赋值语句的主要作用是对表达式求值，副作用是对数据对象或文件进行修改。序列点是程序中的点，在该点上，所有的副作用都必须在下一步完成。
* C允许使用混合类型计算，但是无论是自动类型转换还是强制类型转换，均会存在各种问题。在日常开发与工作中应当尽量避免，若需要，建议添加注释说明。
* 目标类型是一个**整形**，待赋值是一个**浮点数**，结果未定义（补充：浮点数在整数范围内“向零截断”，若超出整数表达范围，其结果时未定义的 ----2025.09.22）。