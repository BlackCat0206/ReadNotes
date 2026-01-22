# Capter 02 Test

***----Li Jinsheng***

***

## 一、概述

本文用于存放《C Primer Plus》（第6版）第二章课后练习内容

## 二、复习题

### 2.1 C语言的基本模块是什么？

函数

### 2.2 什么时语法错误？写一个例子

```c
#include <stdio.h>

int main(void)
(		
	return 0;
)
```

### 2.3 什么是语义错误？写一个例子

```c
#include <stdio.h>

int main(void)
{
	int n1;
    int n2;
    int sum;
    
    n1=1;
    n2=1;
    
    sum = n1 * n2;
    
    printf("sum result is :%d", sum);
    
    return 0;
}
```

### 2.4 Indiana Sloth编写了如下程序，并征求你的意见。

```c
include stdio.h
int main（void）
(
    int s
    s := 56;
    printf(There are s weeks in a year.);
    return 0;
```

修改结果：

```c
#include <stdio.h>	// 添加# 与‘<’、‘>’执行预处理命令

int main(void)
{
    int s;		// 声明语句添加‘;’
    s = 56;		// 赋值语句
    
    printf("There are %d weeks in a year.", s); // 正确使用printf()函数与占位符
    
    return 0;
}	// 用{}作为函数体开始与结束s
```

### 2.5 假设下面的4个例子都是完整程序的一部分，他们都输出什么结果？

```c
// Q:
/*
a. printf("Baa Baa Black Sheep.");
   printf("Have you any wool?\n");
   
b. printf("Begone!\nO creature of lard!\n");
c. printf("What?\nNo/nfish?\n");
d. int num;
   num = 2;
   printf("%d + %d = %d", num, num, num + num);
*/
```

A:

```c
/**
 Result a:
 Baa Baa Black Sheep.Have you any wool?
 
 Result b:
 Begone!
 O creature of lard!
 
 Result c:
 What?
 No/nfish?
 
 Result d:
 2 + 2 = 4
*/
```

### 2.6 在main、int、function、char、=中，那些是C语言的关键字

main：是C语言程序开始的位置。

int：关键字，表示数据为整数类型。

function：非关键字。

char：关键字，表示数据类型为字符类型。

=：赋值运算符。

### 2.7 如何以下面的格式输出变量word和lines的值（3020和350代表两个变量的值）！

`There were 3020 words and 350 lines`

```c
#include <stdio.h>

int main(void)
{
    int words;
    int lines;
    
    words = 3020;
    lines = 350;
    
    printf("There were %d words and %d lines", words, lines);
    
    return 0;
}
```

### 2.8 考虑下面程序，在第7、第8、第9行后，程序分别是什么状态？

```c
#include <stdio.h>
int main(void)
{
    int a, b;
    
    a = 5;
    b = 2;	// 第7行：a==5, b==2;
    b = a;	// 第8行：a==5, b==5
    a = b;	// 第9行：a==5, b==5
    printf("%d, %d\n", a, b);
    
    return 0;
}
```

### 2.9 考虑下面程序，在第7、第8、第9行后，程序分别是什么状态？

```c
#include <stdio.h>
int main(void)
{
    int x, y;
    x = 10;
    y = 5;		// 第7行：x==10, y==5;
    y = x + y;	// 第8行：x==10, y==15;
    x = x*y;	// 第9行：x==150, y==15;
    printf("%d, %d", x, y);
    
    return 0;
}
```

## 三、编程题

### 3.1 编写一个程序，调用一次printf()函数，把你的名和姓氏打在一行。在调用一次printf()函数，把你的名和姓分别打印在两行。然后在调用两次printf()函数，把你的名和姓打在一行。

```c
#include <stdio.h>
int main(void)
{
    printf("Li Jinsheng\n");

    printf("Li\n Jinsheng\n");
    
    printf("Li");
    printf("Jinsheng\n");
    
    return 0;
}
```

### 3.2 编写一个程序，打印你的姓名和地址

```c
#include <stdio.h>

/*
 * @brief 名称打印输出
 * @param [in]  void
 * @param [out] void
 */
void name_Output(void)
{
    printf("Name:Li Jinsheng ");
    return;
}

/*
 * @brief 地址打印输出
 * @param [in]  void
 * @param [out] void
 */
void addrs_Output(void)
{
    printf("Addrs: Gansu.");
    return;
}

/*
 * @brief 名称与地址打印输出主函数
 * @param [in]  void
 * @param [out] int
 */
int main(void)
{
    name_Output();
    addrs_Output();
    
    return 0;
}
```

### 3.3 编写一个程序把你的年龄转换为天数，并显示这个值。这里不用考虑闰年。

```c
#incldue <stdio.h>

/*
 * @brief 年龄打印输出
 * @param [in]  int age 年龄
 * @param [out] void
 */
void age_Output(int age)
{
    printf("Age: %d ", age);
    return;
}

/*
 * @brief 年龄对应天数打印输出
 * @param [in]  int age 年龄
 * @param [out] void
 */
void days_Output(int age);
{
    int days = age * 365;
    printf("Days: %d\n", days);
    return;
}

/*
 * @brief 年龄对应天数打印输出主函数
 * @param [in]  void
 * @param [out] int
 */
int main(void)
{
    age_Output();
    days_Output();
    
    return 0;
}
```

### 3.4 编写程序，生成如下输出，除`main()`函数以外，该程序还要调用两个自定义函数，一个名为`jolly()`用于打印前三条信息，调用一次打印一条；另一个函数名为`deny()`打印最后一条。

`For he's a jolly good fellow!`

`For he's a jolly good fellow!`

`For he's a jolly good fellow!`

`Which nobody can deny!`

```c
#include <stdio.h>

/*
 * @brief 指定内容打印
 * @param [in] void
 * @param [out] void
 */
void jolly(void)
{
    printf("For he's a jolly good fellow!\n");
    return;
}

/*
 * @brief 指定内容打印
 * @param [in] void
 * @param [out] void
 */
void deny(void)
{
   printf("Which nobody can deny!\n");
    return;
}

/*
 * @brief 指定内容打印主程序
 * @param [in] void
 * @param [out] int
 */
int main(void)
{
    jolly();
    jolly();
    jolly();
    deny();
    return 0;
}
```

### 3.5 编写程序，生成如下输出，除`main()`函数以外，该程序还要调用两个自定义函数，一个名为`br()`调用一次打印一次”Brazill, Russia“；另一个函数名为`ic()`调用一次打印一次”India, China“。其余内容在`main()`函数中完成。

`Brazill, Russia, India, china`

`India, China`

`Brazill, Russia`

```c
#include <stdio.h>

/*
 * @brief 指定内容打印
 * @param [in] void
 * @param [out] void
 */
void br(void)
{
    printf("Brazill, Russia");
    return;
}

/*
 * @brief 指定内容打印
 * @param [in] void
 * @param [out] void
 */
void ic(void)
{
   printf("India, China");
   return;
}

/*
 * @brief 指定内容打印主程序
 * @param [in] void
 * @param [out] int
 */
int main(void)
{
    /*第一行*/
    br();
    printf(", ");
    ic();
    printf("\n");
    
    /*第二行*/
    ic();
    printf("\n");
    
    /*第三行*/
    br();
    
    return 0;
}
```

### 3.6 编写一个程序，创建一个整形变量toes，并将整形变量toes设置为10.程序中还要计算toes的两倍和toes的平方。该程序打印三个值，并分别描述以示区别。

```c
#include <stdio.h>

/*
 * @brief 打印指定数据的两倍与平方主程序
 * @param [in] void
 * @param [out] int
 */
int main(void)
{
    int toes, double_toes, squared_toes;
    
    toes = 10;
    double_toes = 2 * toes;
    squared_toes = toes * toes;
    
    printf("toes is :%d, double toes is :%d, squared toes is :%d", toes, double_toes, squared_toes);
    return 0;
}
```

### 3.7 编写一个程序，生成如下内容。该程序要定义一个函数，该函数被调用一次打印一次“Smile!”，根据程序需要调用该函数。

`Smile!Smile!Smile!`

`Smile!Smile!`

`Smile!`

```c
#include <stdio.h>

/*
 * @brief 指定内容打印
 * @param [in] void
 * @param [out] void
 */
void SmileOutput(void)
{
    printf("Smile!");
    return;
}

/*
 * @brief 指定内容打印主程序
 * @param [in] void
 * @param [out] int
 */

int main(void)
{
    int i, j;
    i = 3;
    j = 0;
    for (i = 3; i > 0; i--)
    {
        for (j = i; j > 0; j-- )
        {
            SmileOutput();
        }
        printf("\n");
    }
    return 0;
}
```

### 3.8 编写一个程序，生成如下内容。调用一个名为`one_three()`函数。该函数在一行打印“one”，在调用第2个函数`two()`，然后在另一行打印“three”。`two()`函数在一行显示单词“two”。`main()`函数在调用`one_three()`函数前要打印短语“starting now：”，并在调用完毕后显示短语“done!”。

`starting now:`

`one`

`two`

`three`

`done!`

```c
#include <stdio.h>

/*
 * @brief 指定内容打印
 * @param [in] void
 * @param [out] void
 */

void two(void)
{
    printf("two\n");
    return;
}


/*
 * @brief 指定内容打印
 * @param [in] void
 * @param [out] void
 */
void ont_three(void)
{
    printf("one\n");
    two();
    printf("three\n");
    return;
}

/*
 * @brief 指定内容打印主程序
 * @param [in] void
 * @param [out] int
 */
int main(void)
{
    printf("starting now:\n");
    ont_three();
    printf("done!\n");
    return 0;
}
```

