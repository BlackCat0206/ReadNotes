# Chapter_06_Test

***---- Li Jinsheng***

## 一、概述

本章用于记录《C Prime Plus》（第6版）第6章复习题与编程练习。

注：本章开始课后题大部份将不会记录题目。

## 二、复习题

### 2.1 

答：

```c
int quack = 2;
quack += 5;		// 7
quack *= 10;	// 70
quack -= 6;		// 64
quack /= 8;		// 8
quack %= 3;		// 2
```

### 2.2 

```c
//int 类型输出 C99的趋零截断
 36
 13
  7
  3
  1

// 若是double类型，则会称为死循环
```

### 2.3 

```c
a. x > 5;
b. 0 == scanf("%lf", &x)
c. 5 == x
```

### 2.4 

```c
a. 1 == scanf("%d", &num)
b. 5 != x
c. 20 <= x 
```

### 2.5 

```c
#include <stdio.h>
int main(void)
{
    int i, j , list(10);			// 声明list数组：list[10];
    
    for (i = 1, i <= 10; i++)
    {
        list[i] = 2 * i + 3;
        for (j = 1, j >= i, j++)
            printf(" %d", list[j]);
        printf("\n");				// for循环缺少花括号
}									// main函数缺少返回语句：returen 0;
```

### 2.6 

```c
#include <stdio.h>
int main(void)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
        {
            printf("%c", '$');
        }
        printf("\n");
    }
    return 0;
}
```

### 2.7 

```c
a. Hi! Hi! Hi! Bye! Bye! Bye! Bye! Bye! 
b. ACGM
```

### 2.8 

```C
a. Gowest,youn
b. Hpxftu-zpvo
c. Gowest,young
d. $owest,youn
```

### 2.9 

```c
31|32|33|30|31|32|33|
***
1
5
9
13
    
***
2 6
4 8
8 10
    
***
======
=====
====
===
==

```

### 2.10

```c
a. 数组名：mint
b. 该数字有10个元素
c. 每个元素可以存放double类型的值
d. scanf("%lf", &mint[2]);
```

### 2.11 

```c
#include <stdio.h>
#define SIZE 8
int main(void)
{
    int by_twos[SIZE];
    int index;
    
    for (index = 1; index <= SIZE; index++)			// trouble: 数组的下标从0开始，C编译器不会检查下标的访问是否正确，这里会存在访问越界。
    {
        by_twos[index] = 2 * index;
    }
    
    for (index = 1; index <= SIZE; index++)			// trouble: 数组的下标从0开始，C编译器不会检查下标的访问是否正确，这里会存在访问越界。
    {
        by_twos[index] = 2 * index;
    }
    
    printf("\n");
    return 0;
}
```

### 2.12

```c
// 函数返回类型为：long，函数返回值为long类型。
long test(void)
{
    return 0l;
}
```

### 2.13

```c
#include <stdio.h>

long squa(int num)
{
    long SquNum = 0l;
    SquNum = num * num;
    
    return SquNum;
}
```

### 2.14 

```c
Hi!

k = 1

k is 1 in the loop

Now k is 3

k = 3

k is 3 in the loop

Now k is 5

k = 5

k is 5 in the loop

Now k is 7

k = 7
```



## 三、编程题