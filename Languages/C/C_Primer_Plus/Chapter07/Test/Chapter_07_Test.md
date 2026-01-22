# Chapter_07_Test

***---- Li Jinsheng***

## 一、概述

本章用于记录《C Prime Plus》（第6版）第7章复习题与编程练习。

注：本章开始课后题大部份将不会记录题目。

## 二、复习题

### 2.1

```c
a. true
b. false
c. false
```

### 2.2

```c
a. 90 <= number && 100 > number
b. 'q' != ch || 'k' != ch
c. 1 <= number && 9 >= number && 5 != number
d. 1 >= number || 9 <= number
```

### 2.3

```c
#include <stdio.h>
int main(void)
{
    int weight, height;
    
    scanf("%d , weight, height);		// scanf错误使用，修正：scanf("%d %d", weight, height);
    if (weight < 100 && height > 64)
    {
        
    }
}
```

### 2.4 

```c
a. 1

b. 0

c. 1

d. 6

e. 10

f. 0
```

### 2.5

```c
// 打印内容：*#%*#%$%*#%*#%$%*#%*#%$%*#%*#%
```

### 2.6

```
fat Oh no!
hat Oh no!
cat Oh no!
```

### 2.7

```c
#include <stdio.h>
int main(void)
{
    char ch;
    int lc = 0;
    int uc = 0;
    int oc = 0;
    
    while((ch = getchar()) != '#')
    {
        if ('a' <= ch >='z')				// 二元运算符 if('a' <= ch && 'z' >= ch)
        	lc++;
        else if (!(ch < 'A') || !(ch > 'Z'))	// else if ('A' <= ch && 'Z' >= ch)
        	uc++;
        oc++;
    }
    printf("%d lowercase, %d uppercase, %d other", lc, uc, oc);
    return 0;
}
```

### 2.8

```c
You are 21. Here is a raise.
You are 22. Here is a raise.
You are 23. Here is a raise.
... ... 
You are 65. Here is a raise.
You are 66. Here is a raise.
```

### 2.9

```c
// q
Step 1
Step 2
Step 3
// c
Step 1
// h
Step 1
Step 3
// b
Step 1
Done!
```

### 2.10

```c
#include <stdio.h>
int main(void)
{
    char ch;
    while((ch = getchar()) != '#')
    {
     	if ('\n' != ch)
        {
            printf("Step 1\n");
            if ('c' != ch)
            {
                if ('b' == ch)
                {
                    break;
                }
                else if ('h' != ch)
                {
                    printf("Step 2\n");
                }
                printf("Step 3\n");
            }
        }
    }
    printf("Done!\n");
    return 0;
}
```

