# Chapter_05_Test

***---- Li Jinsheng***

***

## 一、概述

本章用于存放《C Prime Plus》(第6版)第5章复习题与编程练习

## 二、内容（时间关系，题目后续补充）

### 2.1 

答：

a. x = 30

b. x = 27

c. x = 1, y = 1

d. x = 3, y = 9

### 2.2 

答：

a. x = 6

b. x = 52

c. x = 0

d. x = 13

### 2.3 

a. 37.5

b. 1.5

c. 35

d. 37

e. 37.5

f. 35.0

### 2.4 请找出下面的程序中的错误

```c
// 缺少头文件引用
int main(void)
{
    int i = 1;
    float n;
    printf("Watch out! Here come a bunch of fractions!\n");
    while(i < 30)						// i的值不会变化，死循环
        n = 1/i;
    	printf(" %f", n);				// while迭代语句后若只有一句，可以省略花括号，超过一句则需要用花括号括起来。
    printf("That's all, folks\n");
    return ;							// 返回内容与函数定义的返回值类型不匹配。
}
```

### 2.5 程序清单5.9的另一个版本。从表面上看只使用了一条`scanf()`语句，比程序清单5.9简单。请找出不如原版之处。

```c
#include <stdio.h>
#define SEC_PER_MIN 60            // seconds in a minute
int main(void)
{
    int sec, min, left;
    
    printf("This program converts seconds to minutes and seconds\n");
    printf("Enter the number of seconds (<=0 to quit):\n");
   	// 声明时未对变量sec进行初始化，这里无法保证进入while循环。
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
```

### 2.6 下面程序将打印出什么内容

```c
#include <stdio.h>
#define FORMAT "%s! c is cool\n"
int main(void)
{
    int num = 10;
    
    printf(FORMAT, FORMAT);
    
    printf("%d\n", ++num);
    printf("%d\n", num++);
    printf("%d\n", num--);
    printf("%d\n", num);
    return 0;
}
```

答：

```c
%s! c is cool
! c is cool
11
11
12
11
```

### 2.7 下面程序将打印出什么内容

```c
#include <stdio.h>
int main(void)
{
    char c1, c2;
    int diff;
    float num;

    c1 = 'S';
    c2 = 'O';
    diff = c1 - c2;		// 4
    num = diff;			// 4.00
    printf("%c%c%c:%d %3.2f\n", c1, c2, c1, diff, num);
    return 0;
}
```

答：

```c
SOS:4 4.00
```

### 2.8 下面的程序将打印出什么内容？

```c
#include <stdio.h>
#define TEN 10
int main(void)
{
    int n = 0;
    
    while(n++ < TEN)
    	printf("%5d", n);
    printf("\n");
    return 0;
}
```

答：

```c
    1    2    3    4    5    6    7    8    9   10
```

### 2.9 修改上一个程序，使其可以从打印字母a~g

答：

```c
#include <stdio.h>
int main(void)
{
    char n = 'a' - 1;
    
    while(n++ < 'g')
    	printf("%5c", n);
    printf("\n");
    return 0;
}

/*
Result:
    a    b    c    d    e    f    g
*/
```

### 10. 假设下面时完整程序中的一部分，他们分别打印什么？

答：

a.：

  ```c
      1    2
  ```

b.：   

```c
 101
 102
 103
 104
```

c.：

```c
stuvw
```

### 2.11 下面程序会打印出什么？

```c
#define MESG "COMPUTER BYTES DOG"
#include <stdio.h>
int mian(void)
{
    int n = 0;
    
    while (n < 5)
        printf("%s\n", MESG);
    	n++;
    printf("That's all.\n");
    return 0;
}
```

答：

```c
COMPUTER BYTES DOG
```

### 2.12 分别编写一条语句，完成下列各项任务（或使其具有以下副作用）

答：

a.：

```c
x = x + 10;
```

b.:

```c
++x;
```

c.:

```c
c = (a + b) * 2;
```

d.:

```c
c = a + 2 * b;
```

### 2.13 分别编写一条语句，完成下列各任务

答：

a.：

```c
x = x - 1;
x--;
--x;
```

b. :

```c
m = n % k;
m = n - (n/k)*k;
```

c. ：

```c
p = q / (b - a);
```

d.:

```c
x = (a + b) / (c * d);
```

## 三、编程练习

见本目录05_01_Test.c

