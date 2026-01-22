# Chapter_11_字符串和字符串函数

***---- Li Jinsheng***

## 一、概述

本章为《C Primer Plus》（第6版）第10章读书笔记

## 二、内容

### 2.1 字符串和指针

* `puts()`函数会对输出的字符串字面量添加结束符`\0`。

#### 2.1.1 字符串字面量

字符串字面量（*string literal*）也被称为字符串常量（*string constant*）。双引号中字符串编译器会自动添加结束符`\0`。

**字符串串联特性**：ANSI C标准，若字符串直接没有间隔或空白符号，则将其视为一个串联的字符串常量。

**存储位置**：字符串常量是**静态存储类型**（*static storage type*），即程序中使用的字符串常量都只会被存储一次，**双引号括起来字符串被视为指向该字符串的指针**。

```c
/* strptr.c -- strings as pointers */
#include <stdio.h>
int main(void)
{
    printf("%s, %p, %c\n", "We", "are", *"space farers");
    
    return 0;
}

/*
Result:
We, 0000000000404000, s
*/
```

[^程序清单 11.2]: strptr.c

#### 2.1.2 字符串数组和初始化

```c
const char *c1 = "Test String.";

char c2[16] = {'T', 'e', 's', 't', ' ', 'S', 't', 'r', 'i', 'n', 'g'};		// 这里没结束符\0,故被视为字符数组！！！
```

* 与其他数组相同，未被使用的元素默认初始化为`\0`，**数组名被视为数组首元素的地址**。

#### 2.1.3 数组和指针

字符串数组的内存分配过程：

* 将字符串常量存储在静态内存中。
* 为数组分配内存。
* 将字符串常量拷贝值数组中。
* 编译器将数组名视为首元素地址，**故不能使用数组名进行递增与递减运算（改变了存储的首元素地址，可能存储的字符串无法引用）**。

指针：

* 将字符串常量存储在静态内存中。
* 为指针分配内存。
* 将字符串常量地址赋给指针。
  * 字符串字面量是常量，故需要用常量指针`const char* p`，即不能通过指针修改字符串字面值。


```c
//  addresses.c  -- addresses of strings
#define MSG "I'm special"

#include <stdio.h>
int main()
{
    char ar[] = MSG;
    const char *pt = MSG;
    printf("address of \"I'm special\": %p \n", "I'm special");
    printf("              address ar: %p\n", ar);
    printf("              address pt: %p\n", pt);
    printf("          address of MSG: %p\n", MSG);
    printf("address of \"I'm special\": %p \n", "I'm special");

    return 0;
}

/**
Result:
address of "I'm special": 0000000000404000 
              address ar: 000000000061FE3C
              address pt: 0000000000404000
          address of MSG: 0000000000404000
address of "I'm special": 0000000000404000 
*/
```

[^程序清单 11.3]: addresses.c

#### 2.1.4 指针和数组的区别

* 指针可以进行递增和递减操作。
* **数组中的元素的变量，数组名不是变量**。
* 使用指针修改字符串字面值其结果是**未定义**的。
* **如果打算修改字符串，就不要用指针指向字符串字面量**。

#### 2.1.5 字符串数组

```c

//  arrchar.c -- array of pointers, array of strings
#include <stdio.h>
#define SLEN 40
#define LIM 5
int main(void)
{
    // 数组指针
    const char *mytalents[LIM] = {
        "Adding numbers swiftly",
        "Multiplying accurately", "Stashing data",
        "Following instructions to the letter",
        "Understanding the C language"
    };
	
    // 指针数组
    char (*yourtalentsT)[SLEN] = NULL; 
	
    // 二维数组
    char yourtalents[LIM][SLEN] = {
        "Walking in a straight line",
        "Sleeping", "Watching television",
        "Mailing letters", "Reading email"
    };

    yourtalentsT = yourtalents;

    int i;
    
    puts("Let's compare talents.");
    printf ("%-36s  %-25s\n", "My Talents", "Your Talents");
    for (i = 0; i < LIM; i++)
        printf("%-36s  %-25s\n", mytalents[i], yourtalentsT[i]);
    printf("\nsizeof mytalents: %zd, sizeof yourtalents: %zd\n",
           sizeof(mytalents), sizeof(yourtalents));
    
    return 0;
}


/*
Result:
Let's compare talents.
My Talents                            Your Talents
Adding numbers swiftly                Walking in a straight line
Multiplying accurately                Sleeping
Stashing data                         Watching television      
Following instructions to the letter  Mailing letters
Understanding the C language          Reading email

sizeof mytalents: 40, sizeof yourtalents: 200
*/
```

* 若只是展示字符串字面量，建议用数组指针，其是内涵了N个`char`类型的指针。较二维数组会减少内存消耗。
* 若需要修改字符串，建议使用字符串数组。

### 2.2 字符串输入

#### 2.2.1 分配空间

在获取输入字符串是，建议提前为其分配内存，若直接使用指针可能会有编译器报错。

#### 2.2.2 `gets()`函数

`gets()`函数会将整行的输入（除换行符）之后再添加一个**结束符**，存储再所指定的位置。但其不能感知所存储位置的大小，故若分配空间不足，它也无法感知。即使用时可能会访问未分配的内存（段错误）。故C99标准建议不要再使用，C11建议废除该函数。

#### 2.2.3 `fgets()`函数

```c
fgets(char* str, int size, FILE * stream);
```

* `fgets()`函数指定读入字符的数量，若是n，则`fgets()`函数会读取`n-1`个字符，并自动添加终止符。
* 若`fgets()`读取到一个换行符，会将其**存储**在字符串中。
* 要指明要读入的文件，若是标准输入使用`stdin`。

#### 2.2.4 `gets_s()`函数

C11 标准新增`gets_s()`函数（可选）。

```c
gets_s(char *str, int size);
```

* `gets_s()`只从标准输入中读取数据，不需要指定读入文件。
* `gets_s()`不会存储换行符。
* 若读取至最大字符数还是没有读到换行符，会丢弃多余输入字符。调用依赖实现的处理函数，（可能会导致程序中断）。

### 2.3 字符串输出

#### 2.3.1 `puts()`函数

* 其会给输出的字符串添加一个换行符。

#### 2.3.2 `fputs()`函数

```c
fputs(const char* str, FILE* stream);
```

* `fputs()`与`puts()`函数不同，不会对输出的字符串添加换行符。
* 要指明输出的文件，若是标准输出使用`stdout`。

### 2.4 字符串函数

#### 2.4.1 `strlen()`函数

用于统计字符串长度，返回类型是`size_t`，或无符号整数。

#### 2.4.2 `strcat()`函数

用于拼接字符，其接受两个字符串作为参数，将第二个字符串拼接至第一个字符串中，并将第一个字符串作为一个新的字符串。

```c
char str1[20] = "Hello";
char str2[20] = "Lee";

strcat(str1, str2);

fputs(str1, stdout);	// HelloLee
```

* 此函数不安全，其无法判断第一个参数是否可以容纳第二个参数。

#### 2.4.3 `strncat()`函数

指定要拼接内容的大小

```c
char str1[8] = "Hello";
cahr str2[8] = "Lee";

strncat(str1, str2, 1);

fputs(str1, stdout);	// HelloL
```

#### 2.4.4 `strcmp（）`函数

比较输入的两个字符串（遇到一个结束符就停止）是否相等。若相同则返回0。若不同则返回非0值。

```c
strcmp("A", "A");	// 0
strcmp("A", "B");	// -1
strcmp("B", "A");	// 1
strcmp("C", "A")	// 2
strcmp("apples", "apple");	// 115
```

`strcmp()`是会逐个比较每一个字符，直至遇到一个不相同的字符，并输出其差值。

#### 2.4.5 `strncmp()`函数

比较两个输入字符串中指定范围内的字符。

```c
strncmp(const char* str1, const char* str2, size_t n);
```

#### 2.4.6`strcpy()`和`strncpy()`函数

`strcpy()`拷贝字符串，其返回值为一个参数的值，且第一个参数不必指向数组的开始，可以用于拷贝字符串的一部分。

```c
/* copy2.c -- strcpy() demo */
#include <stdio.h>
#include <string.h>    // declares strcpy()
#define WORDS  "beast"
#define SIZE 40

int main(void)
{
    const char * orig = WORDS;
    char copy[SIZE] = "Be the best that you can be.";
    char * ps;
    
    puts(orig);
    puts(copy);
    ps = strcpy(copy + 7, orig);
    puts(copy);
    puts(ps);
    
    return 0;
}

/*
Result:
beast
Be the best that you can be.
Be the beast
beast
*/
```

[^程序清单 11.26]: copy2.c

* 它会将原字符中的**空字符也拷贝**过来。

`strncpy()`指定拷贝字符串的大小。

```c
/* copy3.c -- strncpy() demo */
#include <stdio.h>
#include <string.h>  /* declares strncpy() */
#define SIZE 40
#define TARGSIZE 7
#define LIM 5
char * s_gets(char * st, int n);

int main(void)
{
    char qwords[LIM][TARGSIZE];
    char temp[SIZE];
    int i = 0;
    
    printf("Enter %d words beginning with q:\n", LIM);
    while (i < LIM && s_gets(temp, SIZE))
    {
        if (temp[0] != 'q')
            printf("%s doesn't begin with q!\n", temp);
        else
        {
            strncpy(qwords[i], temp, TARGSIZE - 1);
            qwords[i][TARGSIZE - 1] = '\0';
            i++;
        }
    }
    puts("Here are the words accepted:");
    for (i = 0; i < LIM; i++)
        puts(qwords[i]);
    
    return 0;
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    int i = 0;
    
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        while (st[i] != '\n' && st[i] != '\0')
            i++;
        if (st[i] == '\n')
            st[i] = '\0';
        else // must have words[i] == '\0'
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}

/*
Result:
Enter 5 words beginning with q:
qeeeeeeee
qeeeeeee
qeeeeee
qeeeee
qeeee
Here are the words accepted:
qeeeee
qeeeee
qeeeee
qeeeee
qeeee
*/
```

[^程序清单 11.27]: copy3.c

#### 2.4.7 `spinrtf()`函数

* 声明：`stdio.h`头文件中。将多种元素组成一个字符串。

```c
/* format.c -- format a string */
#include <stdio.h>
#define MAX 20
char * s_gets(char * st, int n);

int main(void)
{
    char first[MAX];
    char last[MAX];
    char formal[2 * MAX + 10];
    double prize;
    
    puts("Enter your first name:");
    s_gets(first, MAX);
    puts("Enter your last name:");
    s_gets(last, MAX);
    puts("Enter your prize money:");
    scanf("%lf", &prize);
    sprintf(formal, "%s, %-19s: $%6.2f\n", last, first, prize);
    puts(formal);
    
    return 0;
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    int i = 0;
    
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        while (st[i] != '\n' && st[i] != '\0')
            i++;
        if (st[i] == '\n')
            st[i] = '\0';
        else // must have words[i] == '\0'
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}

/*
Result：
Enter your first name:
JS
Enter your last name:
Lee
Enter your prize money:
10000000
Lee, JS                 : $10000000.00
*/
```

[^程序清单 11.28 ]: format.c

### 2.5 把字符串转换为数字

* `stdlib.h`中有如下函数：

```c
atoi(); 将字符串转换为int类型整数
atof(); 将字符串转换为double类型浮点数
atol(); 将字符串转换为long类型
```

具体用法如下：

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i, time;

    if (argc < 2 || (time = atoi(argv[1]) < 1))
    {
        fprintf(stderr, "Usage: %s positive-integer\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        for (i = 0; i < time; i++)
            puts("Hello, World!");
    }

    return 0;
}
```

* ANSI C还提供了一些更智能的函数：

```c
/*
* @brief 将字符串转换为longe类型的值
* @param [in] const char* 待转换的字符
* @param [in] char** 输入数字的结束地址
* @param [in] int 转换进制
* @return [out] 转换的结果
*/
long strtol(const char* restrict, char ** restrict endptr, int base);	// 将字符串转换为long类型的值


strtoul();	// 将字符串转换为无符号long类型的值
strtod();	// 将字符串转换为double类型的值
```

示例：

```c
/* strcnvt.c -- try strtol()  */
#include <stdio.h>
#include <stdlib.h>
#define LIM 30
char * s_gets(char * st, int n);

int main()
{
    char number[LIM];
    char * end;
    long value;
    
    puts("Enter a number (empty line to quit):");
    while(s_gets(number, LIM) && number[0] != '\0')
    {
        value = strtol(number, &end, 10);  /* base 10 */
        printf("base 10 input, base 10 output: %ld, stopped at %s (%d)\n",
               value, end, *end);
        value = strtol(number, &end, 16);  /* base 16 */
        printf("base 16 input, base 10 output: %ld, stopped at %s (%d)\n",
               value, end, *end);
        puts("Next number:");
    }
    puts("Bye!\n");
    
    return 0;
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    int i = 0;
    
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        while (st[i] != '\n' && st[i] != '\0')
            i++;
        if (st[i] == '\n')
            st[i] = '\0';
        else // must have words[i] == '\0'
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}

/*
Result:
Enter a number (empty line to quit):
10
base 10 input, base 10 output: 10, stopped at  (0)
base 16 input, base 10 output: 16, stopped at  (0)
Next number:
10atom
base 10 input, base 10 output: 10, stopped at atom (97)
base 16 input, base 10 output: 266, stopped at tom (116)	// 按16进制，识别为10a，故停止在tom
Next number:

Bye!

*/
```

## 三、小结

本章继续深入阐述了字符串和指针的关系，以及日常开发中常用的字符串相关函数。注：在日常开发或工作中，需要使用安全的函数（`strncmp`、`strncpy`等）其余不安全函数在开源扫描时会告警，部分程序会进行安全检查。

`gets()`：不读取换行符，并添加结束符。

`puts()`：为输出内容添加换行符。

`fgets()`：读取换行符，并限制输入大小。

`fputs()`：不为输出内容添加换行符。





