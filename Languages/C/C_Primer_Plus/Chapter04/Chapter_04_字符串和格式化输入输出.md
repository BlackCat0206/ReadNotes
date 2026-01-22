# Chapter_04_字符串和格式化输入输出

***---- Li Jinsheng***

***

## 一、概述

本章为《C Prime Plus》(第6版）第四章 字符串和格式化输入输出读书笔记。

## 二、内容

### 2.1 前导程序

```c
// talkback.c -- nosy, informative program
#include <stdio.h>
#include <string.h>      // for strlen() prototype
#define DENSITY 62.4     // human density in lbs per cu ft
int main()
{
    float weight, volume;
    int size, letters;
    char name[40];        // name is an array of 40 chars
    
    printf("Hi! What's your first name?\n");
    scanf("%s", name);
    printf("%s, what's your weight in pounds?\n", name);
    scanf("%f", &weight);
    size = sizeof name;
    letters = strlen(name);
    volume = weight / DENSITY;
    printf("Well, %s, your volume is %2.2f cubic feet.\n",
           name, volume);
    printf("Also, your first name has %d letters,\n",
           letters);
    printf("and we have %d bytes to store it.\n", size);
    
    return 0;
}

/*
Result:
Hi! What's your first name?
LI
LI, what's your weight in pounds?
74.5
Well, LI, your volume is 1.19 cubic feet.
Also, your first name has 2 letters,
and we have 40 bytes to store it.
*/
```

[^程序清单4.1]: talkback.c

### 2.2 字符串介绍

字符串：一个或多个的字符序列。

`"There is a string."`

双引号只是告诉编译器它括起来的内容是字符串。

#### 2.2.1 `char`类型数组和`null`字符

* C语言的字符串被存放在`char`类型的数组中。
* `\0`字符：每个字符串都必须以`\0`字符结尾，即**空字符（*null character*）**，表示字符串在这里结束。其ASCII的值是0。这意味这存储字符串的数组的大小要比待存储的字符串多1一个单元。
* 数组：同类型元素的有序序列。

#### 2.2.2 使用字符串

```c
/* praise1.c -- uses an assortment of strings */
#include <stdio.h>
#define PRAISE "You are an extraordinary being."
int main(void)
{
    char name[40];
    
    printf("What's your name? ");
    scanf("%s", name);
    printf("Hello, %s. %s\n", name, PRAISE);
    
    return 0;
}

/**
Result:
What's your name? Deny Stark
Hello, Deny. You are an extraordinary being.
*/
```

[^程序清单4.2]: praise1.c

* 字符串常量：不用在其后加`\0`，编译器会自动添加。
* `scanf()`函数在遇到空格（ ）就停止获取内容。
* 字符和字符串的区别是：

| 数据            | 存储 | 存储 |
| --------------- | ---- | ---- |
| ‘x’是一个字符   | x    |      |
| "x"是一个字符串 | x    | `\0` |

#### 2.2.3 `strlen()`函数

* `strlen()`函数：给出字符串中有效字符的长度（不包含`\0`）。
* 函数原型（*function prototype*）或函数声明在头文件`string.h`中。

```c
/* praise2.c */
// try the %u or %lu specifiers if your implementation
// does not recognize the %zd specifier
#include <stdio.h>
#include <string.h>      /* provides strlen() prototype */
#define PRAISE "You are an extraordinary being."
int main(void)
{
    char name[40];
    
    printf("What's your name? ");
    scanf("%s", name);
    printf("Hello, %s. %s\n", name, PRAISE);
    printf("Your name of %zd letters occupies %zd memory cells.\n",
           strlen(name), sizeof name);
    printf("The phrase of praise has %zd letters ",
           strlen(PRAISE));
    printf("and occupies %zd memory cells.\n", sizeof PRAISE);
    
    return 0;
}

/**
* Result:
What's your name? Deny Stark
Hello, Deny. You are an extraordinary being.
Your name of 4 letters occupies 40 memory cells.
The phrase of praise has 31 letters and occupies 32 memory cells.
*/
```

[^程序清单4.3]: praise2.c

* C99和C11专门为`sizeof`运算符的返回类型添加了`%zd`转换说明，这对于`strlen()`同样适用。早期的`sizeof`和`strlen()`返回结果通常是`unsigned`或`unsigned long`。
* `sizeof()`函数后是否加括号：若是类型，必须加（例：sizeof(char)），若是特定量，可以不加（例：sizeof name）。实际工作中都加上，不要做一些特殊操作，增加代码复杂度。

### 2.3 常量和C预处理器

* 相同数据多处使用：工作中经常遇到同一个数据，需要在程序中多次使用，为了避免这数据被他人无意篡改，不能将该类数据声明为变量。故大多数时候，会用到C预处理器声明常量：

```c
#define TAXRATE 0.015
```

* **编译时替换（*complie-time substitution*）**：在程序编译时，`TAXRATE`会被全部替换为`0.015`，程序运行时已经全部替换完毕，这样定义的常量叫**明示常量（*manifest constant*）**或符号常量。
* **命名规则**：与给标识符命名相同，字母大小写（建议都大写）、数字和下划线，不能以数字开头，不需要分号结尾。

```c
/* pizza.c -- uses defined constants in a pizza context */
#include <stdio.h>
#define PI 3.14159
int main(void)
{
    float area, circum, radius;
    
    printf("What is the radius of your pizza?\n");
    scanf("%f", &radius);
    area = PI * radius * radius;
    circum = 2.0 * PI *radius;
    printf("Your basic pizza parameters are as follows:\n");
    printf("circumference = %1.2f, area = %1.2f\n", circum,
           area);
    return 0;
}

/**
Result:
What is the radius of your pizza?
6.0
Your basic pizza parameters are as follows:
circumference = 37.70, area = 113.10
*/
```

[^程序清单4.4]: pizza.c

* 转换说明`%1.2f`：1表示**最小总宽度**1个字符，`%.2f`表示输出时保留两位小数，会对小数点后第3位进行四舍五入。例如：
  - 如果结果是`3.146`，输出为`3.15`
  - 如果结果是`3.144`，输出为`3.14`

#### 2.3.1 `const`限定符

* `const`限定符用于限定一个变量只是**可读**， C90添加。
* 工作中会将一些复杂的变量前加`const`限定符，例部分权限敏感的结构体。

#### 2.3.2 明示常量

* 整数类型和浮点类型大小限制通用明示常量：`limits.h`和`float.h`两个头文件中分别对整数类型和浮点数类型大小限制实现了相关的明示常量。

```c
#define INT_MAX +32767
#define INT_MIN -32768
```

* 如果操作系统使用32位的int，该头文件会为这些明示常量提供不同的值。

```c
// defines.c -- uses defined constants from limit.h and float.
#include <stdio.h>
#include <limits.h>    // integer limits
#include <float.h>     // floating-point limits
int main(void)
{
    printf("Some number limits for this system:\n");
    printf("Biggest int: %d\n", INT_MAX);
    printf("Smallest long long: %lld\n", LLONG_MIN);
    printf("One byte = %d bits on this system.\n", CHAR_BIT);
    printf("Largest double: %e\n", DBL_MAX);
    printf("Smallest normal float: %e\n", FLT_MIN);
    printf("float precision = %d digits\n", FLT_DIG);
    printf("float epsilon = %e\n", FLT_EPSILON);
    
    return 0;
}

/*
Result:
Some number limits for this system:
Biggest int: 2147483647
Smallest long long: -9223372036854775808
One byte = 8 bits on this system.
Largest double: 1.797693e+308
Smallest normal float: 1.175494e-038
float precision = 6 digits
float epsilon = 1.192093e-007
*/
```

[^程序清单4.5]: defines.c

### 2.4 `printf()`和`scanf()`

`printf()`函数与`scanf()`函数工作原理基本相同，它们都使用了格式字符串和参数列表。

#### 2.4.1 `printf()`函数

若需要`printf()`函数打印数据，需要指定使用**转换说明（*conversion specifcation*）**，它规定了如何将待打印数据正确的转化为其对应的格式。开发者要注意，转换说明的类型、数量要与待打印的数据相匹配。

| 转换说明 | 输出                                                         |
| -------- | ------------------------------------------------------------ |
| `%a`     | 浮点数、十六进制和p计数法（C99和C11）                        |
| `%A`     | 浮点数、十六进制和p计数法（C99和C11）                        |
| `%c`     | 单个字符                                                     |
| `%d`     | 有符号十进制整数                                             |
| `%e`     | 浮点数、e计数法 （转化时，默认为`double`类型）               |
| `%E`     | 浮点数、e计数法                                              |
| `%f`     | 浮点数、十进制计数法                                         |
| `%g`     | 根据值的不同，自动选择`%f`或`%e`。`%e`用于指数位小于-4或者大于或等于其精度 |
| `%G`     | 根据值的不同，自动选择`%f`或`%e`。`%e`用于指数为小于-4或者大于或等于其精度 |
| `%i`     | 有符号十进制整数                                             |
| `%o`     | 无符号八进制整数                                             |
| `%p`     | 打印指针                                                     |
| `%s`     | 打印字符串                                                   |
| `%u`     | 无符号十进制整数                                             |
| `%x`     | 无符号十六进制整数                                           |
| `%X`     | 无符号十六进制整数                                           |
| `%%`     | 打印一个百分号                                               |

#### 2.4.2 使用`printf()`

```c
/* printout.c -- uses conversion specifiers */
#include <stdio.h>
#define PI 3.141593
int main(void)
{
    int number = 7;
    float pies = 12.75;
    int cost = 7800;

    
    printf("The %d contestants ate %f berry pies.\n", number,
           pies);
    printf("The value of pi is %f.\n", PI);
    printf("Farewell! thou art too dear for my possessing,\n");
    printf("%c%d\n", '$', 2 * cost);
    
    return 0;
}

/*
Result:
The 7 contestants ate 12.750000 berry pies.
The value of pi is 3.141593.
Farewell! thou art too dear for my possessing,
$15600
*/
```

在使用过程中，函数`printf()`中双引号括起来的内容就被称为：**格式化字符串**。格式化字符串被分割为**字母字符**和**转换说明**。

#### 2.4.3 `printf()`的转换说明修饰符

**转换说明修复符**：在`%`和转换字符之间可以插入修复符来修饰转换说明，注意书写顺序应该遵循下表原则，不能随意组合。

| 修饰符 | 含义                                                         |
| ------ | ------------------------------------------------------------ |
| 标记   | -、+、空格、数字、#                                          |
| 数字   | 最小字段宽度，若该字段不能容纳待打印的内容，系统会选择更宽的字段。例：%4d |
| .数字  | `%e`、`%E`和`%f`转换，表示小数点右边的数字位数。（指数计数后尾数部分，见例4.7）<br />`%g`和`%G`转换，表示有效数字的最大位数。<br />`%s`转换，表示要打印的最大字符数量。（**不算`\0`这个空字符**，见例4.10）<br />整形转换，打印数字的最小位数。（**多出位数用0填充**，见例4.8）<br />例：`%5.2f`：打印的最小宽度是5，输出浮点数小数点后两位。 |
| `h`    | 和整形转换说明一起使用：`%hd`表示`signed short int`类型，`%hu`表示`unsigned short int`<br />例：n=7; %5.2d打印的结果是 ` 空格空格空格07`; 若是-7， 结果为：` 空格空格-07` |
| `hh`   | 和整形转换说明一起使用，表示`signed char`或者`unsigned char`类型<br />例：`%hhd`、`%hhu` |
| `j`    | 和整形转换说明一起使用，表示`intmax_t`类型或`uintmax_t`类型，上述类型在`stdint.h`中<br />例：`%jd`、`%ju` |
| `l`    | 和整形转换说明一起使用，表示`long int`或`unsigned long int`。<br />例：`%ld`、`%8lu` |
| `ll`   | 和整形转换说明一起使用，表示`long long int`或`unsigned long long int`。<br />例：`%lld`、`%8llu` |
| `L`    | 和浮点转换说明一起使用，表示`long double`类型的值。<br />例：`%Lf`、`%10.4Le` |
| `t`    | 和整形转换说明一起使用，表示`prtdiff_t`类型的值。`ptrdiff_t`表示两个指针的差值（C99）。<br />例：`%td`、`%12ti` |
| `z`    | 和整形转换说明一起使用，表示`size_t`类型。`size_t`是`sizeof`返回值类型（C99）。<br />例：`%zd` |

* `printf()`的标记

| 标记 | 含义                                                         |
| ---- | ------------------------------------------------------------ |
| -    | 待打印项左对齐，即从字段的左侧开始打印。<br />例：n=7，`%-3.2d`结果为：`07空格` |
| +    | 若符号值为正，则在值前显示正号，若为负，则在值前显示符号。<br />例：`%6.2f` |
| 空格 | 若符号值为正，则在值前添加前导空格，若为负，则在值前添加减号并覆盖空格。<br />例：n=7 `% d` 结果：`空格7`， i=-7 `% d` 结果：`-7` |
| `#`  | 将结果转换为另外一种形式，<br />`%o`，结果以0开始。<br />`%x`：结果以`0x`开始，`%X`结果以`0X`开始。<br />对于所有的浮点类型，保证即使没有任何一位数字，也输出一位小数。<br />`%g`或`%G`来讲，防止结果后的0被删除。 |
| `0`  | 对数值格式，前导0用于替换空格填充字段。对**整数格式**，若出现`-`标记或指定精度则忽略该标记。<br />例：n=7, `%3d`的结果是：`空格空格7`，`%03d`的结果是`007`， `%3.2d`的结果是`空格07` |

* 转换修饰和标记的示例：

```c
/* width.c -- field widths */
#include <stdio.h>
#define PAGES 959
int main(void)
{
    printf("*%d*\n", PAGES);
    printf("*%2d*\n", PAGES);
    printf("*%10d*\n", PAGES);
    printf("*%-10d*\n", PAGES);		// 左对齐
    
    return 0;
}

/*
Result:
*959*
*959*
*       959*
*959       *
*/
```

[^程序清单4.7]: width.c

```c
// floats.c -- some floating-point combinations
#include <stdio.h>

int main(void)
{
    const double RENT = 3852.99;  // const-style constant
    
    printf("*%f*\n", RENT);
    printf("*%e*\n", RENT);
    printf("*%4.2f*\n", RENT);		// 保留
    printf("*%3.1f*\n", RENT);
    printf("*%10.3f*\n", RENT);
    printf("*%10.3E*\n", RENT);		// 指数计数后，保留小数点后三位
    printf("*%+4.2f*\n", RENT);
    printf("*%010.2f*\n", RENT);	// 小数类型是适配的。
    
    return 0;
}

/*
Result:
*3852.990000*
*3.852990e+003*
*3852.99*
*3853.0*
*  3852.990*
*3.853E+003*
*+3852.99*
*0003852.99*
*/
```

[^程序清单4.8]: floats.c

```c
/* flags.c -- illustrates some formatting flags */
#include <stdio.h>
int main(void)
{
    printf("%x %X %#x\n", 31, 31, 31);
    printf("**%d**% d**% d**\n", 42, 42, -42);
    printf("**%5d**%5.3d**%05d**%05.3d**\n", 6, 6, 6, 6);
    
    return 0;
}

/*
Result:
1f 1F 0x1f
**42** 42**-42**
**    6**  006**00006**  006**
*/
```

[^程序清单4.9]: flags.c

```c
/* stringf.c -- string formatting */
#include <stdio.h>
#define BLURB "Authentic imitation!"
int main(void)
{
    printf("[%2s]\n", BLURB);
    printf("[%24s]\n", BLURB);
    printf("[%24.5s]\n", BLURB);
    printf("[%-24.5s]\n", BLURB);
    return 0;
}

/**
Result:
[Authentic imitation!]
[    Authentic imitation!]
[                   Authe]
[Authe                   ]
*/
```

[^程序清单4.9]: stringf.c

#### 2.4.4 转换说明的意义

转换说明不是将原始数据替换位对应结果，而是将计算机中存储的二进制内容翻译为给定类型。

##### 2.4.4.1 转换不匹配

```c
/* intconv.c -- some mismatched integer conversions */
#include <stdio.h>
#define PAGES 336
#define WORDS 65618
int main(void)
{
    short num = PAGES;
    short mnum = -PAGES;
    
    printf("num as short and unsigned short:  %hd %hu\n", num,
           num);
    printf("-num as short and unsigned short: %hd %hu\n", mnum,
           mnum);
    printf("num as int and char: %d %c\n", num, num);
    printf("WORDS as int, short, and char: %d %hd %c\n",
           WORDS, WORDS, WORDS);
    return 0;
}

/*
Result:
num as short and unsigned short:  336 336
-num as short and unsigned short: -336 65200
num as int and char: 336 P
WORDS as int, short, and char: 65618 82 R
*/
```

[^程序清单 4.11]: intconv.c

* 系统使用二进制补码的方式表达有符号整数，故第二行输出的结果是65200。
* 第三行按照`%c`转换说明，只会读取变量`num`的第一个字节内容，即数据被截断了。

```c
/* floatcnv.c -- mismatched floating-point conversions */
#include <stdio.h>
int main(void)
{
    float n1 = 3.0;
    double n2 = 3.0;
    long n3 = 2000000000;
    long n4 = 1234567890;
    
    printf("%.1e %.1e %.1e %.1e\n", n1, n2, n3, n4);
    printf("%ld %ld\n", n3, n4);
    printf("%ld %ld %ld %ld\n", n1, n2, n3, n4);
    
    return 0;
}


/*
Result:
3.0e+000 3.0e+000 9.9e-315 6.1e-315
2000000000 1234567890
0 0 2000000000 1234567890
*/
```

[^程序清单 4.12]: floatcnv.c

* 第三章中有提到，浮点数的存储方式与整数不同，使用浮点数转化说明转化整数，输出结果会异常。

##### 2.4.4.2 `printf()`的返回值

```c
/* prntval.c -- finding printf()'s return value */
#include <stdio.h>
int main(void)
{
    int bph2o = 212;
    int rv;
    
    rv = printf("%d F is water's boiling point.\n", bph2o);
    printf("The printf() function printed %d characters.\n",
           rv);
    return 0;
}

/**
Result:
212 F is water's boiling point.
The printf() function printed 32 characters.
*/
```

[^程序清单 4.13]: prntval.c

* `printf()`返回值：打印输出的所有字符，包括空格和换行符。

##### 2.4.4.3 打击较长的字符串

```c
/* longstrg.c –– printing long strings */
#include <stdio.h>
int main(void)
{
    printf("Here's one way to print a ");
    printf("long string.\n");
    printf("Here's another way to print a \
long string.\n");
    printf("Here's the newest way to print a "
           "long string.\n");      /* ANSI C */
    
    return 0;
}

/*
Result:
Here's one way to print a long string.
Here's another way to print a long string.
Here's the newest way to print a long string.
*/
```

[^程序清单 4.14]: longstrg.c

* C语言中换行符：在代码中可以使用**反斜杠（`\`）**来表示代码换行，增强代码可读性。**但下一行代码必须从最左边开始**，若进行缩进，输出结果也会有对应空格。

* ANSI C规定，两个用双引号括起来的字符串空开，C编译器会认为是一个字符串。

#### 2.4.5 使用`scanf()`

* 使用`scanf()`读取一个基本类型数据，在变量名前加`&`。
* 使用`scanf()`把字符串读入字符数组中，不要使用`&`。

```c
// input.c -- when to use &
#include <stdio.h>
int main(void)
{
    int age;             // variable
    float assets;        // variable
    char pet[30];        // string
    
    printf("Enter your age, assets, and favorite pet.\n");
    scanf("%d %f", &age, &assets); // use the & here
    scanf("%s", pet);              // no & for char array
    printf("%d $%.2f %s\n", age, assets, pet);
    
    return 0;
}

/**
Result:
Enter your age, assets, and favorite pet.
38 32360.88 llama
38 $32360.88 llama
*/
```

[^程序清单 4.15]: 

* `scanf()`使用空白（**换行符，空格，制表符**）将输入的内容进行分割，分别匹配到对应内容。

* `%c`字符较为特殊，
* `scanf()`函数的转换说明

| 转化说明            | 含义                                                         |
| ------------------- | ------------------------------------------------------------ |
| `%c`                | 将输入内容解释为字符                                         |
| `%d`                | 将输入内容解释为有符号十进制整数                             |
| `%f` `%e` `%a` `%g` | 将输入内容解释为浮点数（C99 添加了`%a`）                     |
| `%F %E %A %G`       | 将输入内容解释为浮点数（）                                   |
| `%i`                | 将输入内容解释为有符号十进制整数                             |
| `%o`                | 将输入内容解释为有符号八进制整数                             |
| `%p`                | 将输入内容解释为指针（地址）                                 |
| `%s`                | 将输入内容解释为字符串。从一个非空白开始，到下一个空白结束。 |
| `%u`                | 将输入内容解释为无符号十进制整数                             |
| `%x %X`             | 将输入内容解释为有符号十六进制整数                           |

* `scanf()`转换说明的修饰符

| 转换说明      | 含义                                                         |
| ------------- | ------------------------------------------------------------ |
| `*`           | 抑制赋值                                                     |
| 数字          | 最大字段宽度。输入达到最大输入宽度或第一次遇到空白停止。     |
| `hh`          | 把整数作为`unsigned char`和`signed char`读取。<br />例：`%hhd``%hhu` |
| `ll`          | 把整数作为`long long`或`unsigned long long`类型读取          |
| `h`、`l`或`L` | "%hd"和"%hi"将数据作为`short int`类型存储。<br />"%ho"、"%hx"和"%hu"将数据作为`unsigned short int`类型存储。<br />"%ld"和"%li"将数据作为`long int`类型存储。<br />"%lo" "%lx" "%lu"将数据作为`unsigned long int`类型存储。<br />"%le"、"%lf"和"%lg"将数据作为`double`类型进行存储。<br />若`e f g`前不是`l`而是`L`，则存储类型为`long double`类型。<br />**若`o` `d` `i` `x`前没有修复表明对应值被存储为`int`类型**。<br />**`f g`表示存储为`float`类型。** |
| `j`           | 在整形转换说明后时，表明使用`intmax_t`和`uintmax_t`。<br />例："%jd","%ju" |
| `z`           | 在整形转换说明后面时，说明使用`sizeof`的返回值类型。<br />例：“%zd”, "%zu" |
| `t`           | 在整数转换说明后面时，说明使用`ptrdiff_t`类型。<br />例：“%td”,"%tu" |

##### 2.4.5.1 格式字符串中的普通字符

* `scanf()`函数允许在格式字符串中使用普通字符。

```c
int m, n;
scanf("%d,%d", &m, &n);

/**
1,2

1,   2	scanf()会跳过输入前的空白

1,
2

*/
```

`scanf()`会理解为：用户输入一个数字、一个逗号、一个数字。

* 格式字符中的空格意味着跳过下一个输入项前面的所有空白。

```c
int m, n;
scanf("%d ,%d", &m, &n);

/**
以下输入均没有问题：
88，121
88 ，121
88 ， 121
*/
```

* `%c`前若有空格，`scanf()`则会跳过空格，从一个非空白字符开始读取。

```c
char test1;
scanf("%c", &test1);		// 从一个输入字符读取。

char test2;
scanf(" %c", &test2);	 	// 从1个非空白字符开始读取。
```

2.4.5.2 `scanf()`的返回值

* 若没有读取到任何内容，返回0。若检测到“文件结尾”返回`EOF`(stdio.h中定义的特殊符号，通常定义为-1)。

#### 2.4.6 `printf()`和`scanf()`修饰`*`修饰符

* `printf()`中`*`修饰符可以看作一种在转换说明中的赋值。

```c
/* varwid.c -- uses variable-width output field */
#include <stdio.h>
int main(void)
{
    unsigned width, precision;
    int number = 256;
    double weight = 242.5;
    
    printf("Enter a field width:\n");
    scanf("%d", &width);
    printf("The number is :%*d:\n", width, number);		// 这里替换了输入的width
    printf("Now enter a width and a precision:\n");
    scanf("%d %d", &width, &precision);
    printf("Weight = %*.*f\n", width, precision, weight);	// 这里替换了输入的width和precision
    printf("Done!\n");
    
    return 0;
}


/**
Result:
Enter a field width:
6
The number is :   256:
Now enter a width and a precision:
8 3
Weight =  242.500
Done!
*/
```

[^程序清单 4.16]: varwid.c

* `scanf()`中被`*`修饰的，会跳过该输入项：

```c
/* skiptwo.c -- skips over first two integers of input */
#include <stdio.h>
int main(void)
{
    int n;
    
    printf("Please enter three integers:\n");
    scanf("%*d %*d %d", &n);
    printf("The last integer was %d\n", n);
    
    return 0;
}

/**
Result:
Please enter three integers:
11 22 33
The last integer was 33
*/
```

[^程序清单 4.17]: skiptwo.c

## 三、小结

本章主要表述了：

* C语言中没有特定的关键字用来存放字符串，故当前是将字符串存放在`char`类型的数字中，每个字符串必须以`\0`结尾，即字符数组至少要比待存储的字符串的长度+1相同。
* `strlen()`函数：函数原型在`string.h`头文件中，用来计算字符数组中的有效字符的数量（不包括'\0'）。
* 在日常的开发中，经常会遇到一个常量在程序中被多处使用，为了避免但该常量变化时，程序大改。C语言中使用预处理器将`#define`后声明的明示常量（*manifest constant*）在编译时进行编译时替换（*complie-time subsitution*）。
* `const`可以用来修复只有权限的变量。例：程序中软件厂商的部分信息时只能读取但不能修改的。
* `printf()`函数的转换说明（特别时八进制与十六进制，输出时作为无符号类型），修饰符和标志。内容较为复杂，在日常开发中可以多用，多记。
* `scanf()`函数的转换说明、修饰符和标志。在日常开发中多使用，多尝试。**其返回值会成功转换的个数**。
* `*`修复符在`printf()`函数与`scanf()`函数中在转换说明中作用不同，一个类似替换，一个是跳过。