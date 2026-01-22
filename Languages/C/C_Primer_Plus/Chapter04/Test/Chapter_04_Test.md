# Chapter_04_Test	

---- Li Jinsheng

***

## 一、概述

本章用于《C Prime Plus》（第6版）复习题与编程练习

***

## 二、复习题

### 2.1 再次运行程序清单4.1.但是在要求输入名时，输入名和姓，看看是什么情况，为什么？

输出结果为：
`Hi! What's your first name?
Js Lee
Js, what's your weight in pounds?
Well, Js, your volume is 0.00 cubic feet.
Also, your first name has 2 letters,
and we have 40 bytes to store it.`

答：代码中调用了两次`scanf()`函数，一个使用`%s`转换说明，遇到第一空白（空格、换行或制表符）将之前读取的字符类型存放给`name`变量。将后续的数据存放起来，存放在第二个`scanf()`函数调用时，存储在对应变量中，第二个使用`%f`转换说明，字符串类型和浮点类型所占用内存与存储方式不同，故可能出现异常。

### 2.2 假设下列示例都是完整程序中的一部分，他们打印的结果是什么？

`a .printf("He sold the painting for $%2.2f.\n", 2.345e2);`

`b .printf("%c%c%c\n", 'H', 105, '\41');`

`c. #define Q "His Hamlet was funny without being vulgar."`

​      `printf("%s\n has %d characters.\n",  Q, strlen(Q));`

`d. printf("Is %2.2e the same as %2.2f?\n", 1201.0, 1201.0);`

答：

a: He sold the painting for 234.50.

b: Hi!

c:  His Hamlet was funny without being vulgar.

​     has 42 characters.

d. Is 1.20e3 the same as 1201.00?

### 2.3 在第2体中的c中，要输出包含双引号的字符串Q，应该如何修改？

```c
#define Q "His Hamlet was funny without being vulgar."`

printf("\"%s\"\n has %d characters.\n",  Q, strlen(Q));
```

### 2.4 找出下面程序中的错误

```c
define B booboo												// 预处理命令缺少`#`, 定义的明示变量为字符串类型，要用`""`括起来
define X 10													// 预处理命令缺少`#`
main()														// 缺少函数返回类型和传入参数类型
{
    int age;
    char name;												// 若存放名字，应该声明为字符数组即字符串
    printf("Please enter your first name.");
    sacnf("%s", name);							// 之前声明为`char`类型，字符变量，应该用`%c`为转换说明， 且须在变量名前加`&`，但考虑到程序整体逻辑，此处不必修改。
    pritnf("All right, %c, what's your age？\n", name);
    scanf("%f", age);							// 之前声明变量为`int`类型，此处应该使用`%d`为转换说明，且应该在变量名前加`&`
    xp = age + X;								// 变量xp未被声明，不能使用。
    printf("That's a %s ! You must be at least %d.\n", B, xp);
    return 0;
}
```

### 2.5 假设一个程序的开头时这样：

```c
#define BOOK "War and Peace"
int main(void)
{
    float cost = 12.99;
    float percent = 80.0;
}
```

请构造一个使用BOOK、cost和percent的`printf()`语句，打印以下内容：

`This copy of "War and Peace" sells for $12.99.`

`That is 80% of list.`

答：

```c
#include <stdio.h>
#define BOOK "War and Peace"
int main(void)
{
    float cost = 12.99;
    float percent = 80.0;
    
    printf("This copy of \"%s\" sells for %.2f\n", BOOK, cost);
    printf("That is %.0f%% of list", percent);
    
    return 0;
}

/*
Result:
This copy of "War and Peace" sells for 12.99
That is 80% of list
*/
```

### 2.6 打印下列各项内容分别使用什么转换说明？

a. 一个字段宽度与位数相同的十进制整数

b. 一个形如`8A`、字段宽度为4的十六进制整数

c. 一个形如`232.346`、字段宽度为10的浮点数

d. 一个`2.33e+002`、字符宽度为12的浮点数

e. 一个字段宽度为30，左对齐的字符串

答：

a. `%d`、b.`%#4x`、c.`%10.3f`、d.`%12.2e`、e.`%-30s`

### 2.7 打印下列各项内容分别使用什么转换说明？

a. 字段宽度为15的`unsigned long`类型的整数

b. 一个形如`0x8a`、字段宽度为4的十六进制整数

c. 一个形如`2.33E+02`、字段宽度为12、左对齐的浮点数

d. 一个形如`+232.346`、字符宽度为10的浮点数

e. 一个字段宽度为8的字符串的前8个字符

答：a.`%15lu` 、b.`%#4x`、c.`%-12.2E`、d`%+10.3f`、e.`%8.8s`

### 2.8 打印下列各项内容分别使用什么转换说明？

a. 字段宽度为6、最少有4位数字的十进制整数

b. 一个在参数列表中给定字段宽度的八进制整数

c. 一个字段宽度位2的字符

d. 一个形如`+3.13`、字段宽度等于数字中字符数的浮点数

e.  一个字段宽度为7、左对齐字符串中的前5个字符

答：a. `%6.4d`、b.`%*o`、c.`%2c`、d.`%+0.2f`、e.`%-7.5s`

### 2.9 分别写出读取下列个输入行的`scanf()`语句，并声明语句中用到变量和数组

a. 101

b. 22.32 8.34E-09

c. linguini

d. catch 22

e.  catch 22 (但是跳过22)

答：

```c
#include <stdio.h>

int main(void)
{
    int num;
    printf("Please input 'a'\n");
    scanf("%d", &num);
    printf("%d is 'a' input\n", num);
    
    double f1, f2;
    printf("Please input 'b'\n");
    scanf("%lf %le", &f1, &f2);
    printf("%.2f and %.2e is 'b' input\n", f1, f2);
    
    char c1[16];
    printf("Please input 'c'\n");
    scanf("%s", c1);
    printf("\"%s\" is 'c' input\n", c1);
    
    char c2[8];
    int num2;
    printf("Please input 'd'\n");
    scanf("%s %d", c2, &num2);
    printf("%s and %d is 'd' input\n ", c2, num2);
    
    char c3[8] = "null";
    int num3;
    printf("Please input 'd'\n");
    scanf("%*s %d", &num3);
    printf("%s and %d is 'e' input\n", c3, num3 ;
    
    return 0;
}
           
           
/**
Result:
Please input 'a'
101
101 is 'a' input
Please input 'b'
22.32 8.37E-09
22.32 and 8.37e-009 is 'b' input
Please input 'c'
linguini
"linguini" is 'c' input
Please input 'd'
catch 22
catch and 22 is 'd' input
Please input 'd'
catch 22
null and 22 is 'e' input
*/
```

### 2.10 什么时空白？

制表符、换行符和空格。

### 2.11 下面的语句有什么问题？如何修正?

`printf("The double type is %z beytes..\n", sizeof(double));`

答：`%z`它是转换说明的修饰符，需要和整形转换说明一起使用。

`printf("The double type is %zu beytes..\n", sizeof(double));`

### 2.12 假设在程序中用圆括号替代花括号，以下方法是否可行？

`#define ( {`

`#define ) }`

答：不可行，明示变量的命名规则为：由大小写字母、数字和下划线组成，且第一个字符为字母或下划线。不能使用符号。

## 三、编程练习

### 3.1 编写一个程序，提示用户输入名和姓，然后以“名，姓”的格式打印出来

```c
#include <stdio.h>

int main(void)
{
    char firstName[16];
    char secondName[8];
    
    printf("Please input you secondName and firstName.\n");
    scanf("%s %s", firstName, secondName);
    printf("Your name is \"%s,%s\".", firstName, secondName);
    
    return 0;
}

/*
Result:
Please input you secondName and firstName.
Jinsheng Lee
Your name is "Jinsheng,Lee".
*/
```

### 3.2 编写一个程序，提示用户输入名字，并执行以下操作：

a. 打印名字，包括双引号；

b. 在宽度为20的字段右端打印名字，包括双引号；

c.  在宽度为20的字段左端打印名字，包括双引号；

d.  在比姓名宽度宽3的字段中打印名字；

```c
#include <stdio.h>
#include <string.h>

int main(void)
{
    char Name[32];
    
    printf("Please input you Name\n");
    scanf("%s", Name);
    
    // a.
    printf("Your name is \"%s\".\n", Name);
    
    // b. 
    printf("Your name is \"%20s\".\n", Name);
    
    // c.
    printf("Your name is \"%-20s\".\n", Name);
    
    // d.
    size_t sizeName = strlen(Name) + 3;
    
    printf("Your name is \"%*s\".\n", sizeName, Name);
    
    return 0;
}

/*
Result:
Please input you Name
Lee
Your name is "Lee".
Your name is "                 Lee".
Your name is "Lee                 ".
Your name is "   Lee".
*/
```

### 3.3 编写一个程序，读取一个浮点数，首先以小数记法打印，然后以指数记法打印。用下面的格式进行输出：

`The input is 21.3 or 2.1e+001`

`The input is +21.290 or 2.123E+001`

答：

```c
#include <stdio.h>

int main(void)
{
    double d1;

    printf("Enter a floating-point value: ");

    scanf("%lf", &d1);
    
    printf("The input is %.1f or %.1e\n", d1, d1);
    
    printf("The input is %+.3f or %.3E\n", d1, d1);
    
    return 0;
}

/*
Result:
Enter a floating-point value: 21.290
The input is 21.3 or 2.1e+001
The input is +21.290 or 2.129E+001
*/
```

### 3.4 编写一个程序，提示用户输入身高（单位：英尺）和姓名，然后以下面的格式显示用户刚输入的信息：

`Dabney, you are 6.208 feet tall`

使用`float`类型，并用`/`作为除号。如果你愿你，可以要求用户以厘米为单位输入身高，并以米为单位显示出来。

答：

```c
#include <stdio.h>

int main(void)
{
    float tall;
    char name[32];
    
    printf("Please input you name.\n");
    scanf("%s", name);
    
    printf("Please input you tall\n");
    scanf("%f", &tall);
    
    float tallM = tall / 100.0f;
    
    printf("%s, you are %.3f m tall", name, tallM);
    
    return 0;
}

/*
Result:
Please input you name.
Lee
Please input you tall
184.35
Lee, you are 1.844 m tall
*/
```

### 3.5 编写程序，提示用户输入以兆每秒（Mb/s)为单位的下载速度和以兆字节（MB）为单位的文件大小。程序中应计算文件的洗澡时间。注意，这里1个字节等于8位。使用`float`类型，并用`/`作为除号。该程序要以下面的格式打印3个变量的值（下载速度、文件大小和下载时间），显示小数点后面两位数字。

`At 18.12 megabits per second, a file of 2.20 megabytes`

`downloads in 0.97 seconds.`

答：

```c
#include <stdio.h>

int main(void)
{
    float downdSpeed, fileSize;

    printf("Please input the downdSpeed(Mbps) and fileSize(MB):\n");
    scanf("%f %f", &downdSpeed, &fileSize);

    float time = fileSize * 8 / downdSpeed; // 换算单位：1 MB = 8 Mb（因为1字节=8比特）
    printf("At %.2f megabits per second, a file of %.2f megabytes\n", downdSpeed, fileSize);
    printf("downloads in %.2f seconds.\n", time);
    
    return 0;
}

/*
Result:
Please input the downdSpeed(Mbps) and fileSize(MB):
18.12 2.20
At 18.12 megabits per second, a file of 2.20 megabytes
downloads in 0.97 seconds.
*/
```

### 3.6 编写程序，先提示用户输入名，然后提示用户输入姓。在一行打印用户输入的名和姓，下一行分别打印名和姓的字母数。字母数要与相应名和新的结尾对齐，如下所示：

`Melissa honeybee`

`7      8       `

答：

```c
#include <stdio.h>
#include <string.h>

int main(void)
{
    char firstName[16];
    char secondName[16];
    
    printf("Please input your first name:");
    scanf("%s", firstName);
    
    printf("Please input your second name:");
    scanf("%s", secondName);
    
    size_t lengthFirstName = strlen(firstName);
    size_t lengthSecondName = strlen(secondName);
    
    printf("%s %s\n", firstName, secondName);
    printf("%-*d %d\n", lengthFirstName, lengthFirstName, lengthSecondName);
    
    return 0;
}

/**
Result:
Please input your first name:Melissa
Please input your second name:Honeybee
Melissa Honeybee
7       8
*/
```

### 3.7 编写类型，将一个`double`类型的变量设置为1.0/3.0，一个`float`类型的变量设置为1.0/3.0。分别显示两次计算的结果各三次：一次显示小数点后面6位数字；一次显示小数点后面12位数字；一次显示小数点后面16位数字。程序要包含`float.h`头文件，并显示`FLT_DIG`和`DBL_DIG`的值。1.0/3.0的值与这些值一致吗？

```c
#include <stdio.h>
#include <float.h>

int main(void)
{
    float f1 = 1.0f / 3.0f;
    
    double d1 = 1.0 / 3.0;
    
    printf("float 1/3 with 6 digits: %.6f\n", f1);
    printf("double 1/3 with 6 digits: %.6f\n", d1);
    printf("float 1/3 with 12 digits: %.12f\n", f1);
    printf("double 1/3 with 12 digits: %.12f\n", d1);
    printf("float 1/3 with 16 digits: %.16f\n", f1);
    printf("double 1/3 with 16 digits: %.16f\n", d1);

    printf("FLT_DIG = %d\n", FLT_DIG);
    printf("DBL_DIG = %d\n", DBL_DIG);
    
    return 0;
}


/**
Result:
float 1/3 with 6 digits: 0.333333
double 1/3 with 6 digits: 0.333333
float 1/3 with 12 digits: 0.333333343267
double 1/3 with 12 digits: 0.333333333333
float 1/3 with 16 digits: 0.3333333432674408
double 1/3 with 16 digits: 0.3333333333333333
FLT_DIG = 6
DBL_DIG = 15
*/
```

* 计算结果与设定中的值多处一位有效结果。

### 3.8 编写程序，提示用户输入旅行的里程和消耗的汽油量，然后计算并显示消耗每加仑汽油行驶的英里数，显示小数点后面一位数字。接下来，使用1加仑大约消耗3.785升，1英里大约为1.609千米，把单位是英里/加仑的值转换为升/100公里，并显示结果，显示小数点后面1位数字。注意：美国采用单位燃料的行程（越大越好），而欧洲显示单位距离消耗的燃料测量方案（越小越好）。使用#define创建明示常量或使用`const`限定符创建变量来表示两个转换系数。

```c
#include <stdio.h>

#define MILESTOKM	1.609
#define GALLONETOL  3.785


int main(void)
{
    float miles, gallons;
    printf("Please input your jounray miles and cost gallons:\n");
    scanf("%f %f", &miles, &gallons);
    
    float mg = miles / gallons;
    
    float kl = (gallons * GALLONETOL * 100) / (miles * MILESTOKM);
    
    printf("mi/gal:%.1f, km/L:%.1f\n", mg, kl);
    
    return 0;
}

/**
Result:
Please input your jounray miles and cost gallons:
1.609 3.785
mi/gal:0.4, km/L:5.5
*/
```



