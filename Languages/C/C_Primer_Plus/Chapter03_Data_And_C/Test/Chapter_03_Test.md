# Chapter_03_Test

***---- Li Jinsheng***

***

## 一、概述

本文用于存放《C Primer Plus》（第6版）第三章课后练习内容

## 二、复习题

### 2.1 指出下面各种数据使用的合适数据类型（有些可使用多种数据类型）

a. East Simpleton的人口	（`long`或`long long`）

b. DVD影碟的价格		      (`float`或`double`)

c. 本章出现次数最多的字母	(`char`)

d. 本章出现次数最多的字母次数	(`int`或`long`)

### 2.2 什么情况下需要用long类型的变量替代int类型的变量

当变量所需要表示的值超出`int`类型所表示的范围时，需要考虑使用`long`类型。

### 2.3 使用哪些可以移植的数据类型可以获得32位有符号整数？选择的理由什么？

C语言中，数据类型的精度与占用空间与平台有关，故C语言在移植的过程中会出现不兼容的情况。为解决上述问题，C语言在可移植类型中stdint.h何inttypes.h中规定了**精确宽度整数**，以确保C语言类型各个系统相同。若要获取32为有符号的整数，可以使用`int32_t`作为标准的整形，使用`int32_least_t`类型获取至少32为整形。`int32_fast_t`获取32位最快计算速度。

### 2.4 指出下列常量的类型和含义（如果有的话）

a. '\b'   字符常量，转义序列，表示退格。

b. 1066  `int`类型整数有符号常量

c. 99.44  `double`类型常量

d. 0XAA  表示十六进制`unsigned int`类型数据常量，**对于八进制和十六进制整形数据，系统优先解释为无符号类型**。

e. 2.0e30 `double`类型常量的指数写法，表示2.0 * 10^30

### 2.5 Dottie Cawm编写了一个程序，请找出错误

```c
include <stdio.h>		// #include<stdio.h>
main					// 缺少函数返回类型与圆括号
{
    float g; h;			// 变量h声明方式错误
    float tax, rate;
    
    g = e21;			// 指数写法不对，可以省略整数但是不能忽略小数点。
    
    tax = rate*g;		// rate并没有初始化，只是完成了声明
    
    // 缺少return 0;
}
```

### 2.6 写出下列常量在声明中使用的类型和在`printf()`中对应的转换说明：

| 常量      | 类型           | 转换说明（%转换字符）                                        |
| --------- | -------------- | ------------------------------------------------------------ |
| 12        | `int`          | `%d`                                                         |
| 0X3       | `unsigned int` | `%x`或`%X`若打印需要输出符号可添加`%#x`，八进制和十六进制默认是无符号类型 |
| 'C'       | `char`         | `%c`                                                         |
| 2.34e07   | `doouble`      | `%e`                                                         |
| '\040'    | `char`         | `%c`                                                         |
| 7.0       | `double`       | `%f`                                                         |
| 6L        | `long`         | `%ld`                                                        |
| 6.0f      | `float`        | `%f`                                                         |
| 0x5.b6p12 | `double`       | `%a`                                                         |

### 2.7 写出下列常量在声明中使用的类型和在`printf()`中对应的转换说明（假设`int`是16位）

| 常量            | 类型          | 转换说明                  |
| --------------- | ------------- | ------------------------- |
| 012             | `int`         | `%o`若需要单位：`%#o`     |
| 2.9e05L         | `long double` | `%le`                     |
| 's'             | `char`        | `%c`                      |
| 100000(>2^15-1) | `long`        | `%ld`                     |
| '\n'            | `char`        | `%c`                      |
| 20.0f           | `float`       | `%f`                      |
| 0x44            | `int`         | %x或`%X`若需要单位：`%#x` |
| -40             | `int`         | `%d`                      |

### 2.8 建设程序的开头有如下声明：

```c
#include <stdio.h>

int main(void)
{
    int imate = 2;
    long shot = 53456;
   	char grade = 'A';
    float log = 2.71828;
    
    // 把下面printf()语句中的内容的转换字符补充完整：
    printf("The odds against the %d  were %ld  to 1.\n", imate, shot);
    printf("A score of %f  is not an %c  grade.\n", log, grade);
    
    return 0;
}
```

### 2.9 假设ch是`char`类型的变量。分别使用转义序列，十进制，把进制字符常量和十六进制字符常量把回车赋值给ch（假设使用ASCII编码值）

```c
#include <stdio.h>

int main(void)
{
    char test1 = '\r';
    char test2 = 13;
    char test3 = '\015';
    char test4 = '\x0D';

    printf("test1 = %d; test2 = %d; test3 = %d; test4 = %d\n", test1, test2, test3, test4);

    return 0;
}

/*
Result:

test1 = 13; test2 = 13; test3 = 13; test4 = 13
*/
```

### 2.10 修正下面的程序

```c
void main(void)	/ this program is perfect/
{
    cows, legs integer;
    printf("How many cow legs did you conut?\n");
    scanf("%c", legs);
    cows = legs / 4;
    printf("That implies there are %f cows.\n", cows);
}
```

修正程序：

```c
#include <stdio.h>

int main(void)
{
    int cows, legs;
    printf("How many cow legs did you conuts\?\n");
    scanf("%d", &legs);
    cows = legs / 4;
    printf("That implies there are %d cows.\n", cows);

    return 0;
}

/*
Reuslt:
How many cow legs did you conuts?
12
That implies there are 3 cows.
*/
```

### 2.11 指出下列转义序列的含义：

a. `\n`: 换行

b. `\\`反斜杠

c. `\"`双引号

d.`\t`横向制表符

## 三、编程练习

### 3.1 通过编写程序，观察系统如何处理整数上溢，浮点数上溢出和浮点数下溢的情况。

```c
#include <stdio.h>

/**
 * @brief 通过试验，观察系统如何处理整数上溢出和浮点数上溢出和下溢出
 * @return int
 */

int main(void)
{
    int max = 2147483647; // 32位系统的int最大值
    float fmax = 3.4E+38; // float最大值
    float fmin = 1.2E-38; // float最小值

    printf("最大值:  (%d)\n", max);
    printf("+1后:(%d)\n",max + 1);
    printf("float最大值: %e\n", fmax);
    printf("float最大值*2: %e\n", fmax * 2.0);
    printf("float最小值: %e\n", fmin);
    printf("float最小值/2: %e\n", fmin / 2.0);


    return 0;
}

/*
Result:
最大值:  (2147483647)
+1后:(-2147483648)
float最大值: 3.400000e+038
float最大值*2: 6.800000e+038
float最小值: 1.200000e-038
float最小值/2: 6.000000e-039
*/
```

### 3.2 编写一个程序要求输入一个ASCII值（如，66），然后打印输入的字符

```c
#include <stdio.h>

int main(void)
{
    int ch;

    printf("Please input a ASCII character. if you want to input an integer, please care it must be in the range of 0 to 127.\n");
    scanf("%d", &ch);
    printf("The ASCII code for %c is %d.\n", ch, ch);

    return 0;
}

/*
Result:
Please input a ASCII character. if you want to input an integer, please care it must be in the range of 0 to 127.
66
The ASCII code for B is 66.
*/
```

### 3.3 编写一个程序，发出一声警报，然后打印下面的文本：

`Startled by the sudden sound, Sallly shouted,`

`"By the Great Pumpkin, what was that!"`

```c
#include <stdio.h>

int main(void)
{
    printf("\aStartled by the sudden sound, Sallly shouted,\n");
    printf("\"By the Great Pumpkin, what was that!\"\n");

    return 0;
}

/**
Result:
Startled by the sudden sound, Sallly shouted,
"By the Great Pumpkin, what was that!"
*/
```

### 3.4 编写一个程序，读取一个浮点数，先打印为小数形成，在打印成指数的形式。若系统支持在打印为p计数法。按如下格式输出：

`Enter a flaoting-point value: 64.25`

`fixed-point notation: 64.250000`

`exponential notation: 6.425e+01`

`p notation: 0x1.01p+6`

```c
#include <stdio.h>

int main(void)
{
    float test1;

    printf("Enter a floating-point value: ");
    scanf("%f", &test1);
    printf("fixed-point notation: %f\n", test1);
    printf("exponential notation: %e\n", test1);
    printf("p notation: %a\n", test1);

    return 0;
}

/*
Result:
Enter a floating-point value: 64.25
fixed-point notation: 64.250000
exponential notation: 6.425000e+001
p notation: 0x1.010000p+6
*/
```

### 3.5 一年大约有3.156 * 10^7秒，编写一个程序，提示用户输入年龄，然后显示该年龄对应的秒数。

```c
#include <stdio.h>

int main(void)
{
    float secondsOfYear = 3.156e7;

    int ageOfUser = 0;
    printf("Please enter your age: ");
    scanf("%d", &ageOfUser);

    printf("Your age is %d, which is about %.2e seconds, or %a seconds.\n", ageOfUser, ageOfUser * secondsOfYear, ageOfUser * secondsOfYear);

    return 0;

}

/*
Result:
Please enter your age: 25
Your age is 25, which is about 7.89e+008 seconds, or 0x1.78397ap+29 seconds.
*/
```

### 3.6 一个水分子的质量约为3.0 * 10 ^-23克。1夸脱水大约是950克。编写一个程序，提示用户输入水的夸克托书，并显示水分子的数量。

```c
#include <stdio.h>

int main(void)
{
    // 水分子的质量是3.0 * 10^(-23)克
    float mass = 3.0e-23;

    // 一夸脱水是950克
    float volume = 950.0e0;

    // 提示用户输入水的跨脱数
    printf("Enter the number of quarts of water: ");
    float quarts;
    scanf("%f", &quarts);

    // 计算水的质量
    float total = quarts * volume;

    // 计算水分子的数量
    float molecules = total / mass;

    printf("You have %.2e molecules of water.\n", molecules);
    return 0;

}

/*
Result:
Enter the number of quarts of water: 1
You have 3.17e+025 molecules of water.
*/
```

### 3.7 一英寸相当于2.54厘米。编写一个程序，提示用户输入身高（/英尺），软后一厘米为单位显示升高。

```c
#include <stdio.h>

int main(void)
{
    float yinCun = 2.54;

    float highOfUser;

    printf("请输入你的身高(英寸):");
    scanf("%f", &highOfUser);

    printf("你的身高是 %.2f 英寸, 即 %.2f 厘米.\n", highOfUser, highOfUser * yinCun);

    return 0;

}

/**
Result:
请输入你的身高(英寸):80
你的身高是 80.00 英寸, 即 203.20 厘米.
*/
```

### 3.8 在美国的体积测量系统中，1品脱等2杯，1杯等于8盎司，1盎司等于2大汤勺，1大汤勺等于三茶勺。编写一个程序，提示用户输入杯数，并以品脱、盎司、汤勺、茶勺为单位显示等价容量。思考为何对于该程序，为何使用浮点类似比整数类型更合适？

```c
#include <stdio.h>

int main(void)
{
    float cups;
    printf("Please input cups:");
    scanf("%f", &cups);
    
    float pints, ounces, tablespoons, teaspoons;
    
    pints = cups / 2.00f;
    ounces = cups * 8.00f;
    tablespoons = ounces * 2e0f;
    teaspoons = tablespoons * 3e0f;
    
    printf("%.2f(c) is %.2f(pt), %.2f(oz), %.2f(tbsp), %.2f(tsp)", cups, pints, ounces, tablespoons, teaspoons);
    
    return 0;
}

/*
Result:
Please input cups:2.00
2.00(c) is 1.00(pt), 16.00(oz), 32.00(tbsp), 96.00(tsp)
*/
```

* 输入的杯数不一定可以是2的倍数，若使用整数，转换为pint时会出现精度损失。