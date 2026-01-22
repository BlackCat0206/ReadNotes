# Chapter_03_数据和C

***---- Li Jinsheng***

***

## 一、概述

本篇为《C Primer Plus》（第6版）第三章 数据和C读书笔记与小结

***

## 二、内容

### 2.1示例程序

```c
/* platinum.c  -- your weight in platinum */
#include <stdio.h>
int main(void)
{
    float weight;    /* user weight             */
    float value;     /* platinum equivalent     */
    
    printf("Are you worth your weight in platinum?\n");
    printf("Let's check it out.\n");
    printf("Please enter your weight in pounds: ");
    
    /* get input from the user                     */
    scanf("%f", &weight);
    /* assume platinum is $1700 per ounce          */
    /* 14.5833 converts pounds avd. to ounces troy */
    value = 1700.0 * weight * 14.5833;
    printf("Your weight in platinum is worth $%.2f.\n", value);
    printf("You are easily worth that! If platinum prices drop,\n");
    printf("eat more to maintain your value.\n");
    
    return 0;
}
```

[^程序清单3.1]: platinum.c

#### 2.1.1 程序中的新元素

* `float`浮点数类型：可以存储带小数的数据。
* `printf()`函数中使用`%.2f`精确控制输出为浮点数小数点后2位。
* `scanf()`函数使用`%f`用来获取用户输入的浮点数给变量`weight`，`&`表示找到了变量`weight`的位置。（下一章讨论符号`&`的作用）

### 2.2 变量和常量数据

程序在运行的过程中需要处理的信息被称为**数据**，其中有些数据是不会变化的（例如：一年有12个月）这类数据被称为**常量（*constant*）**，部分数据在程序运行的过程中是会发生变化的（例如：求和程序，最终输出值是会一直变化的），这类数据被程序为**变量（*variable*）**。

### 2.3 数据：数据类型和关键字

* 不同的数据类型之间也有差异，C语言通过识别不同的**数据类型**来区别和使用不同的数据。
* 常量：若数据是常量，编译器会根据用户**书写形式**来识别数据类型（例：42是整数，42.100是浮点数）。**对于八进制和十六进制整形数据，系统优先解释为无符号类型**。
* 变量：变量在声明时就需明确其数据类型。
* C语言基本关键字：K&R C给出7个数据类型的关键字，C90添加两个，C99添加3个。

| K&R给出关键字 | C90标准添加关键字 | C99添加关键字      |
| ------------- | ----------------- | ------------------ |
| int           | void              | _Bool（布尔类型）  |
| long          | signed            | _Complex（复数）   |
| shot          |                   | _Imaginary（虚数） |
| unsigned      |                   |                    |
| char          |                   |                    |
| float         |                   |                    |
| double        |                   |                    |

* 位（*bit*）：计算机最小的存储单位。可以存放0或1。
* 字节（byte）：计算机存储单位。1字节=8位，即1字节的数值范围在0~255（0~2^8-1）。
* 字（*word*）：计算机存储自然单元。其大小取决于计算机本身，例：8位微型计算机，1字=8位。

#### 2.3.1 整数和浮点数

* 对人而言整数和浮点数的区别在于书写方式不同。
* 对计算机而言整数和浮点数的区别在于存储方式不同。

#### 2.3.2 整数

* 整数是没有小数部分的，计算机以二进制的方式存储整数（例：整数7的二进制位111）。

#### 2.3.3 浮点数

* 特殊形式：3.16E7表示3.16*10^7。其中7是10的指数。
* 浮点数存储形式：符号位+小数位+指数位。例：3.6的存储方式位。同理7.0可以写为0.7E1。

| 符号 | 小数位 | 指数位 |
| ---- | ------ | ------ |
| +    | .36    | 1      |
| +    | .36    | 10^1   |

* 表示范围大：浮点数比整数表示的范围大。
* 损失精度大：在一些算数运算的过程中，（两个很大的数据相减），浮点数损失的精度更多。
* 数据为近似值：在任何区间内（如1.0到2.0）有无穷多个数据，浮点数不能完全表示区间的所有值。浮点数通常是近似值。
* 运算速度慢：当前CPU多数包含浮点计算器，早期CPU对浮点数计算比较慢。

### 2.4 C语言基本数据类型

本章介绍如何声明常量、如何表示字面值常量以及一些典型用法。

#### 2.4.1 `int`类型

* `int`类型是有符号整型。

* IOS C规定`int`类型的**最小**取值范围（-2^15~2^15 - 1）-32768~32767。即用16位存储`int`类型。若为32位则（2^-15~2^15-1）。**最高位用于表示正负，所以是2^15**。

##### 2.4.1.1 声明`int`变量

```c
int test;
int test1, test2, test3;
```

变量获取值的方法：

* 赋值、`scanf()`函数赋值、初始化；

##### 2.4.1.2 初始化变量

 **初始化（*initialize*）**变量，就是为变量赋一个初始值，使用赋值运算符（`=`）即可。**在C语言中初始化可在声明中完成**。

```c
int hogs = 3;
int cows = 4, goats = 5;
int dogs, cats = 6;	// 这种写法有效，但是很糟糕。只初始化了cats，没有初始化dogs。
```

* 声明为变量**创建和标记了存储空间**，初始化和赋值操作时将**数值存储在对应的空间中**。

##### 2.4.1.3 `int`常量

* C语言将大多数整形常量视为`int`类型，但非常大的数据除外。（后续介绍的`long`类型常量、`long long`类型常量）。

##### 2.4.1.4 打印`int`值

* 第2章介绍用`printf()`函数打印，用`%d`指明了在一行打印整数的位置。`%d`称为**转换说明**（必须是小写字母）。

* 开发者要确认**转换说明与待打印值的数量相同**，编译器不会识别该错误。

```c
/* print1.c-displays some properties of printf() */
#include <stdio.h>
int main(void)
{
    int ten = 10;
    int two = 2;
    
    printf("Doing it right: ");
    printf("%d minus %d is %d\n", ten, 2, ten - two );
    printf("Doing it wrong: ");
    printf("%d minus %d is %d\n", ten );  // forgot 2 arguments 这里转换说明与待打印值数量没对上，最终输出结果为随机数。
    
    return 0;
}

```

[^程序清单3.2]: print1.c

##### 2.4.1.5 **八进制和十六进制**

* `0x`或`0X`：作为前缀表示十六进制数据，例：十进制数据16可表示为`0x10`或`0X10`。
* `0`（数字0）：作为前缀表示八进制，例：十进制数据16可表示为`020`的八进制数据。

##### 2.4.1.6 **显示八进制和十六进制数据**

* `%o`：显示八进制数据。
* `%x`或`%X`：显示16进制数据。
* `%#0`、`%#x`、`%#X`：分别显示八进制、十六进制前缀。

```c
/* bases.c--prints 100 in decimal, octal, and hex */
#include <stdio.h>
int main(void)
{
    int x = 100;
    
    printf("dec = %d; octal = %o; hex = %x\n", x, x, x);
    printf("dec = %d; octal = %#o; hex = %#x\n", x, x, x);
    
    return 0;
}

/*
Result:
dec = 100; octal = 144; hex = 64
dec = 100; octal = 0144; hex = 0x64
*/
```

[^程序清单3.3]: bases.c

#### 2.4.2 其他数据类型

* `short int`或`short`：有符号类型，可能比`int`表示的范围小。
* `long int`或`long`：有符号类型，比`int`占用空间可能大。
* `long long int`或`long long`：有符号类型，该类型至少占**64**位，C99标准加入。
* `unsigned int`：表示为非复数，该类型可以表示更大的数。例：16位`unsigned int`取值范围是0~65535（0~2^16-1），**最高位不用表示符号，故可以取到最大值**。
* `unsigned long int`或`unsigned long`：C90标准引入。
* `unsigned long long int`或`unsigned long long `：C99标准引入。

##### 2.4.2.1 使用多种整形的原因

* C语言规定：`short`类型占用存储空间不能多于`int`类型，`long`类型占用存储空间不能少于`int`类型。这样的规定是为了适配不同的机器。
* 个人计算机最常见设计：`long long`类型占64位，`long`类型占32位、`short`类型占16位、`int`类型占16位或32位（依据计算中的**自然字长**）。
* 如果在`long`类型和`int`占用空间相同的机器上编码时，如果需要32位数，应该选择使用`long`类型，以便于把程序移植到16位机器后任然可以继续工作。

##### 2.4.2.2 `long`和`long long`常量及其后缀

* 通常，程序中使用的数字会被存储为`int`类型，若是一个1000000这样一个大数字，编译器会先尝试`unsigned int`，若还是无法存放，编译器会继续尝试存储为`long int`类型，假设还是不行，编译器会将其视为`long long int`或`unsigned long long`。
* **`long`类型常量后缀**：若想让编译器把一个小数字当作`long`类型对的，可以在值的末尾加`l`（小写的L）或`L`后缀。例：在`int`类型占16位，`long`类型占32位的环境中:

```c
7    // 作为16位存储
7L   // 作为32位存储

/*八进制与十六进制也是同理*/
0x10  // 作为16位存储
0X10L // 作为32位存储
    
020   // 作为16位存储
020L  // 作为32位存储
```

* **`unsigned`类型与`long long`类型常量后缀**：无符号类型后缀：`u`或`U`。`long long`可以用`ll`或`LL`。且可以穿插组合例如：`unsigned long long`后缀为`ULL`.

##### 2.4.2.X **整数溢出：**

```c
/* toobig.c-exceeds maximum int size on our system */
#include <stdio.h>
int main(void)
{
    int i = 2147483647;
    unsigned int j = 4294967295;
    
    printf("%d %d %d\n", i, i+1, i+2);
    printf("%u %u %u\n", j, j+1, j+2);
    
    return 0;
}

/**
 Result:
 2147483647 -2147483648 -2147483647
 4294967295 0 1
*/
```

[^程序清单3.x]: toobig.c

* `unsigned int`在溢出后会从0开始
* `int`溢出后会从**负数最小值**开始。注意，**数据溢出其对应类型所能表达最大值时，系统不会提醒用户**，开发者要自我注意。

##### 2.4.2.3 打印`short` 、`long`、 `long long`和`short`类型

| short(十进制) | long(十进制) | long long(十进制) | unsigned long（十进制） | long(八进制) | long(十六进制) | short(八进制) | 自由组合 |
| ------------- | ------------ | ----------------- | ----------------------- | ------------ | -------------- | ------------- | -------- |
| %hd           | %ld          | %lld              | %lu                     | %lo          | %lx            | %ho           | ... ...  |

* C语言允许使用大小写的**常量后缀**，但在转换说明必须是小写。

```c
/* print2.c-more printf() properties */
#include <stdio.h>
int main(void)
{
    unsigned int un = 3000000000; /* system with 32-bit int */
    short end = 200;              /* and 16-bit short       */
    long big = 65537;
    long long verybig = 12345678908642;
    
    printf("un = %u and not %d\n", un, un);
    printf("end = %hd and %d\n", end, end);
    printf("big = %ld and not %hd\n", big, big);
    printf("verybig= %lld and not %ld\n", verybig, verybig);
    
    return 0;
}

/**
Result:
un = 3000000000 and not -1294967296		// 3000000000的有符号值是-1294967296
end = 200 and 200						// 
big = 65537 and not 1					// 65537显示为32位的二进制是：0000 0000 0000 0000 0000 0000 0000 0001 0000 0000 0000 0001，使用%hd printf()函数值查看后16位，故输出的结果是1
verybig= 12345678908642 and not 1942899938	// 同理"%ld"只查看了后32位的值。
*/
```

[^程序清单3.4]: print2.c

* `%h`的作用：显示较大整数被截断成`short`类型的结果。如上述第三行显示内容。
* 各类整形后缀可以不区分大小写，但是使用**转义字符打印时必须时小写**。

#### 2.4.3 使用字符：`char`类型

* 从技术层面将，`char`是整数类型，因其实际上存储的是整数。其范围在0~127。计算机**用特定整数表示特定的字符**。本文主要中ASCII表。

##### 2.4.3.1 声明与定义

* 声明：不分配内存

```c
extern char test1;	// 告诉编译器有一个变量test1;
void func();		// 不分配内存
```

* 定义：分配内存

```c
char i = 1;			// 分配内存
```

##### 2.4.3.2 初始化：

```c
char test1;
test1 = 'A';

char test2 = 'B';

/*错误写法！！！*/
char test3 = "C";	/*此时，“T”是一个字符串*/
    
/*不建议使用*/
char test4 = 'D';
```

* **字符常量（*character constant*）**：C语言中，被用单引号（`''`）括起来的单个字符被称为字符常量（*character constant*）。

* C**语言将常量字符视为`int`类型而不是`char`类型**。例：在`int`为32位，`char`为8位：

```c
char test1 = 'A'
```

'A'对应的数值66存储在32位存储单元中，现在却可以存储在8位的存储单元中。利用这一特性，可以定义一个字符常量'FATE'，即把4个ASCII码存储在一个32位的存储单元中，如果把这样的字符常量赋值位`char`类型的`test1`，只有**最后8位有效**。因此，`test1`的结果是`‘E’`。

##### 2.4.3.3 非打印字符（转义序列）

* 使用ASCII码值进行打印，例：蜂鸣器的ASCII码值是7；

```c
char beep = 7;
```

* **转义序列（*escape sequence*）**：以反斜杠（`\`）开头，用来表示难以输入的数据。

| 转义序列 | 含义                                                         |
| -------- | ------------------------------------------------------------ |
| \a       | 蜂鸣器                                                       |
| \b       | 退格                                                         |
| \f       | 换页                                                         |
| \n       | 换行                                                         |
| \r       | 回车（在 `printf()` 中，`\r` 会把光标移动到当前行的开头，但不会换到下一行。后续输出会从行首开始覆盖原有内容） |
| \t       | 水平制表符                                                   |
| \v       | 垂直制表符                                                   |
| \\\      | 反斜杠                                                       |
| \\‘      | 单引号                                                       |
| \\"      | 双引号                                                       |
| \?       | 问好                                                         |
| \\ooo    | `000`是1~3位八进制数字（0-7）例如：printf("A: \101\n"); // 输出: A: A |
| \\xhh    | `hh`是1~3位十六进制数字（0-9, a-f, A-F）例如：printf("A: \x41\n"); // 输出: A: A  （C++中hh是表示后面所有的数字 第二章 2.1） |

* `\ooo`和`\xhh`是ASCII码表的特殊表示。若用八进制的ASCII码值用`\ooo`格式，十六进制同理，例：如果编译器不识别报警字符（\a），可以使用ASCII码值替代：

```c
char beep = '\007'; // 可以省略前面的0， 07或7都可以。
```

* `\xhh`：从C90引入。
* 注意：数字和字符的区别。例如：字符4对应ASCII码值52，不是数字4。

##### 2.4.3.4 打印字符

* `%c`：`printf()`函数使用`%c`指明待打印字符。

```c
/* charcode.c-displays code number for a character */
#include <stdio.h>
int main(void)
{
    char ch;
    
    printf("Please enter a character.\n");
    scanf("%c", &ch);   /* user inputs character */
    printf("The code for %c is %d.\n", ch, ch);
    
    return 0;
}

/*
Result:
Please enter a character.
C
The code for C is 67.
*/
```

[^程序清单3.5]: charcode.c

##### 2.4.3.5 有符号还是无符号

* `char`是否最有符号还是无符号取决于不同的**编译器**。
* C90标准：`char`关键字前可以添加`signed`或`unsigned`指明数据类型。

#### 2.4.4 _Bool类型

`_Bool`类型：**C99添加**，即逻辑值`true`或`false`。C语言中值1表示`true`，值0表示`false`。故`_Bool`类型实际上也是一种整数类型。但**原则上它仅占用1位存储空间**。

#### 2.4.5 可移植类型：stdint.h 和 inttypes.h

* C99新增了两个头文件`stdint.h`和`inttypes.h`确保C语言的类型在各个**系统**中的**功能相同**。
* **精确宽度整数类型（*exact-width interger type*）**：`int32_t`表示32位的有符号整数类型。若在int位32位的系统中，头文件会把`int32_t`作为`int`的别名。
* **最小宽度类型（*minimum width type*）**：`int_least8_t`可容纳8位有符号整数值的类型中宽度最小的别名。若系统最小整数类型位16位，该系统仍可以使用`int_least8_t`,但可能把该类型实现位16位的整数类型。
* **最快最小宽度类型（*fast minimum width type*）**：`int_fast8_t`被定义为对8位有符号整数值而言运算最快的类型。
* **最大整数类型**：C99定义最大有符号整数类型`intmax_t`，可存储任何有效的有符号的值。可能比`long long`和`unsigned long`更大。
* **可移植类型打印**：例`int32_t`可以用`inttypes.h`中`PRId32`**字符串宏**

```c
/* altnames.c -- portable names for integer types */
#include <stdio.h>
#include <inttypes.h> // supports portable types
int main(void)
{
    int32_t me32;     // me32 a 32-bit signed variable
    
    me32 = 45933945;
    printf("First, assume int32_t is int: ");
    printf("me32 = %d\n", me32);
    printf("Next, let's not make any assumptions.\n");
    printf("Instead, use a \"macro\" from inttypes.h: ");
    printf("me32 = %" PRId32 "\n", me32);	// PRId32本质是"d";
    
    return 0;
}

/*
Result:
First, assume int32_t is int: me32 = 45933945
Next, let's not make any assumptions.
Instead, use a "macro" from inttypes.h: me32 = 45933945
*/
```

[^程序清单3.6]: altnames.c

* **C语言中可以把多个连续字符串合并位一个字符串**。

#### 2.4.6 `float`、`double`和`long double`

浮点数的表示类似科学计数法：

| 数字     | 科学计数法    | 指数计数法 |
| -------- | ------------- | ---------- |
| 322.56   | 3.2256 * 10^2 | 3.2256e2   |
| 0.000056 | 5.6 * 10^-5   | 5.6e-5     |

* `float`类型：`float`类型至少能表示**6位有效数字**，且其取值范围至少时**10^-37~10^+37**。（6位有效数字是指33.333333的前6位数字，而不是小数点后6位）`float`类型一般占**32位**，其中8位用于存放符号于指数，其余24位存放非指数部分（即尾数或有效数）。
* `double`类型：最小取值范围与`float`类型相同，至少能表示**10位**有效数字，一般占用**64**位。
* `long double`类型：C语言保证`long double`类型至少与`double`类型精度相同。

##### 2.4.6.1 声明浮点类型变量

```c
float test1 , test2;
double test3;
float test4 = 3.14e-34;
long double gnp;
```

##### 2.4.6.2 浮点型常量

* 基本类型：有符号的数字（包括小数点）后面紧跟`e`或`E`，最后一个是指数。

* 特殊情况：

  * 可以省略小数点（例：2E1）或省略指数部分（例：3.14）但两者不能同时省略。
  * 可以省略小数部分（例：3.E1）或者省略整数部分（例：.2E1），但二者不能同时省略。

  ```c
  3.1415926
  .2
  4e16
  .8E6
  100.
  ```

* **编译器默认将浮点常量存储为`dobule`类型**。

```c
float sum = 2.0 * 4.0;
```

上述例子就是将两个`double`类型进行双精度运算，将结果截断为`float`精度。

* 浮点类型后缀：可以添加`f`或`F`表示浮点常量是`float`类型，也可以添加`l`或`L`表示为`long double`类型。
* 十六进制表示浮点常量：C99引入新特性，在十六进制数前添加十六进制前缀`0x`或`0X`，用`p`或`P`代替`e`或`E`，用2的幂指数代替10的幂。

```c
0xa.1fp10
```

十六进制a等于十进制的10，`.1f`就是1/16+15/256，p是2^10或1024。最终结果为：10364.0。

##### 2.4.6.3 打印浮点值

| 十进制 | 指数 | 十六进制 | long double   |
| ------ | ---- | -------- | ------------- |
| %f     | %e   | %a       | %Lf、%Le、%La |

```c
/* showf_pt.c -- displays float value in two ways */
#include <stdio.h>
int main(void)
{
    float aboat = 32000.0;
    double abet = 2.14e9;
    long double dip = 5.32e-5;
    
    printf("%f can be written %e\n", aboat, aboat);
    // next line requires C99 or later compliance
    printf("And it's %a in hexadecimal, powers of 2 notation\n", aboat);
    printf("%f can be written %e\n", abet, abet);
    printf("%Lf can be written %Le\n", dip, dip);
    
    return 0;
}

/**
Result:
32000.000000 can be written 3.200000e+004
And it's 0x1.f40000p+14 in hexadecimal, powers of 2 notation
2140000000.000000 can be written 2.140000e+009
0.000000 can be written 3.172905e-317		// window下long double和double相同，Linux应该就正常了
*/
```

[^程序清单3.7]: showf_pt.c

##### 2.4.6.4 浮点数的上溢和下溢

* 上溢（*overflow*）：假设浮点数的指数和小数已经达到最大值，若在此时进行运算扩大，则会超出当前类型的表达范围，这就是上溢（*overflow*）。但C语言规定，超出后会给这个变量赋一个表示***无穷大***的特定值。
* 下溢（*underflow*）：假设浮点数的指数和小数已经达到最小值（假设：0.1234E-10）这时如果再除以10，可能得到的结果是0.0123E-10，这样虽然得到了结果，但是**损失末尾有效数字**，这被称位下溢（*underflow*）。

* 特殊浮点值NaN（*Not a number*）：`asin()`函数用来计算将传入的正弦值转换为角度，若传入数值大于1，这会出现NaN。

#### 2.4.7 复数和虚数类型

| 复数（`_Complex`）    | 虚数（`_Imaginary`）    |
| --------------------- | ----------------------- |
| `float_Complex`       | `float_Imaginary`       |
| `double_Complex`      | `double_Imaginary`      |
| `long double_Complex` | `long double_Imaginary` |

#### 2.4.8 其他类型

* 本章已经介绍了C语言的基本类型，后续会有基本类型衍生的其他类型：数组、指针、结构和联合。

#### 2.4.9 类型大小

```c
//* typesize.c -- prints out type sizes */
#include <stdio.h>
int main(void)
{
    /* c99 provides a %zd specifier for sizes */
    printf("Type int has a size of %zd bytes.\n", sizeof(int));
    printf("Type char has a size of %zd bytes.\n", sizeof(char));
    printf("Type long has a size of %zd bytes.\n", sizeof(long));
    printf("Type long long has a size of %zd bytes.\n",
           sizeof(long long));
    printf("Type double has a size of %zd bytes.\n",
           sizeof(double));
    printf("Type long double has a size of %zd bytes.\n",
           sizeof(long double));
    return 0;
}

/**
 Result:
 Type int has a size of 4 bytes.
 Type char has a size of 1 bytes.
 Type long has a size of 4 bytes.
 Type long long has a size of 8 bytes.	64位
 Type double has a size of 8 bytes.
 Type long double has a size of 16 bytes.
*/
```

[^程序清单3.8]: typesize.c

* `printf()`中`%zd`用来专门打印`size_t`或`ssize_t`的有符号十进制长度类型。

### 2.5 使用数据类型

在编写程序的过程中，声明、初始化、赋值与运算时，应注意选择合适的变量类型。

### 2.6 参数陷阱

```c
/* badcount.c -- incorrect argument counts */
#include <stdio.h>
int main(void)
{
    int n = 4;
    int m = 5;
    float f = 7.0f;
    float g = 8.0f;
    
    printf("%d\n", n, m);    /* too many arguments   */
    printf("%d %d %d\n", n); /* too few arguments    */
    printf("%d %d\n", f, g); /* wrong kind of values */
    
    return 0;
}

/**
Result:
4
4 -4 1731200256
0 0
*/
```

[^程序清单3.9]: badcount.c

* 开发者一定要保证转义说明的数量、类型与后面参数的数量、类型相匹配。
* `%d`显示`float`类型数据，其值不会被转换位int类型

### 2.7 转义序列示例

```c
/* escape.c -- uses escape characters */
#include <stdio.h>
int main(void)
{
    float salary;
    
    printf("\aEnter your desired monthly salary:");/* 1 */
    printf(" $_______\b\b\b\b\b\b\b");             /* 2 */
    scanf("%f", &salary);
    printf("\n\t$%.2f a month is $%.2f a year.", salary,
           salary * 12.0);                         /* 3 */
    printf("\rGee!\n");                            /* 4 */
    
    return 0;
}
/**
* Result:
Enter your desired monthly salary: $1500.00

Gee!    $1500.00 a month is $18000.00 a year.
*/
```

[^程序清单3.10]: escape.c

* 注意转义序列`\r`：将当前行的光标回归至**当前行开始的位置**。
* 缓冲区：最初`printf()`将打印的内容发送到一个缓冲区（*buffer*）中，然后缓冲区中的数据才会显示在屏幕上。
* C语言明确规定了缓冲区何时输出内容：缓冲区满、遇到换行和遇到输入的情况。

## 三、小结

* 本章主要介绍C语言的基本数据类型：整数型、浮点型和字符型。
* 常量：在C语言中数据不会发生变化，称之为常量。
* 变量：在程序运行过程中数据会发生变化的量称为变量。
* 编译器如何区分整数型或浮点型：书写形式或在变量声明时，明确其数据类型。**对于八进制和十六进制整形数据，系统优先解释为无符号类型**。
* 位（*bit*）：计算机最小的存储单元，只能存放一位数据0或1。
* 字节（*byte*）：计算机存储单元，1字节=8位（bit）。
* 字（word）：计算机自然字长。
* 整数：分为有符号类型与无符号类型，且与关键字`short`、`long`、`log long`等有关。
* 浮点数：可以存放小数，打印时可以使用`.2f`精确打印结果后2位。存储方式：符号+小数+指数。其表示范围较广，但在大型数据计算中精度较差，且表示范围有限。
* `int`类型：整数类型，AISO C规定`int`类型最小占位16位，所占内容不小于`short`类型，不多于`long`类型，一般在32位或16位。声明表示了变量在内存中存储空间的位置，初始化是将数值存放在对应位置上。打印时可使用`%d`转义说明。
* 八进制：使用`0`表示，例：010 （十进制的数值8），在打印时可用转义说明：`%o`和`%#o`。
* 十六进制：使用`0x`或`0X`表示，例：0x10 (十进制的数值16)，在打印时可以用转义说明：`%x`和`%#x`。
* 转义说明符：告诉函数在该行的那个位置输出数据。

* `short`、`long`和`long long`：`short`占位不能多于`int`类型，`long`类型占位不能少于`int`类型，`long long`类型占位不能少于**64**位。
* 后缀：`unsigned`、`long`和`long long`三种类型均有其对应后缀：`u`、`U、` `l`、`L`和`ll`、`LL`可以自由组合：例：30ul (unsigned long)
* 打印：`short `类型转义说明：`%h`,`long`类型使用`%l`，`long long`类似使用`%ll`，`unsigned`使用`%u`注：**后缀大小写随意，打印的转义说明必须小写**。可以和之前的`%d`、`%o`组合。
* `char`类型：C语言中用于表示字符的类型，取值范围0-127。C语言中用规定的数值表示字符，例：ASCII码表。
* 字符常量：用单引号括起来的单个字符叫做字符常量。
* C语言中常将字符常量视为整数，赋值给char类型时进行截断。
* 对不好输出的字符可以直接赋给整数。
* 程序中字符的数字和整数的数字时不一样的。
* `char`类型是否有符号取决于编译器本身，开发者可以使用`unsigned`和`signed`来明确是否有符号。
* `_Bool`类型用于表示逻辑`true`或`false`，原则上占用1位存储。
* 可以移植类型：各个操作系统对于个数据的大小均不相同，故C99和C11引入了inttypes.h和stdint.h，用来数据类型进行重命名，以此来保证不同系统中开发的程序移植后仍可继续工作。
* `float`类型：一般占用32位，至少表示6位有效数字，取值范围-10^37~10^37。8位用来存储指数位，其余24位存储尾数位或小数位。
* `double`类型：至少占位6至少表示10位有效数字，取值范围不小于`float`类型。
* `long double`：至少精度与`double`类型相同，转换说明修饰符是大写的`L`。
* 浮点值常量：十进制带小数点（3.14）指数写法（1.82E2）指数写法中特殊写法见对于章节，C99后可以使用十六进制表示小数（0xa.1fa5）。
* 浮点数打印：十进制（`%f`），指数（`%e`），十六进制（`%a`），`long double`类型在上述三者前添加字符`l`（`%lf , %le, %la`）。
* 浮点数上溢：浮点数上溢后，编译器会赋一个规定的最大值。
* 浮点数下溢：即损失有效数字位完成计算。
* 复数和虚数：工程数学中会用到，可与`float`、`double`和`long double`进行组合。
* `%z`用来配合`%d`或`%u`用来打印size_t类型的数据。
* 开发者要注意`printf()`函数中转换说明的类型、数量与其后参数的类型与数量相匹配。用`%d`打印`float`类型，不会做转换。