# Chapter_14_结构和其他数据形式

***---- Li Jinsheng***

***

## 一、概述

本章为《C Prime Plus》（第6版）第14章读书笔记

## 二、内容

### 2.1 结构体

C语言提供了结构变量（*structure variable*），用来适用于更多的开发场景。

#### 2.1.1 结构体的声明和初始化

* 使用关键字`struct`表示类型，标识符、花括号中填充成员数据、用分号结尾。

```c
// 声明
struct structTest{
    int num1;
    char str1[10];
};

// 声明其变量
struct structTest t1;

// 上述声明可以转换为：
struct structTest{
    int num1;
    char str1[10];
}t1;

// 声明结构模板和声明变量可以组合为一部
struct {	// 无需结构标识符
    int num1;
    char str1[10];
}t1;
```

* 初始化，用花括号括起来，用逗号分隔。

```c
// 声明
struct structTest{
    int num1;
    char str1[10];
};

// 声明其变量
struct structTest t1 = {1, "Test"};
```

#### 2.1.2 访问结构体成员

* 结构体运算符（`.`）

```c
struct structTest{
    int num1;
    char str1[10];
};

// 声明其变量
struct structTest t1 = {1, "Test"};

printf("%d\n", t1.num1);
```

#### 2.1.3 结构的初始化器

初始化器（*designated initalizer*）：C99和C11，可用结构体运算符指定成员进行初始化

```c
struct structTest{
    int num1;
    char str1[10];
};

// 声明其变量
struct structTest t1 = {
    .num1 = 1,
	.str1 = "Test"
};
```

#### 2.1.4 结构体数组

可以将结构看作一个自定义的类型，其声明数组于之前由很多相似指出。

```c
struct book{
    char name[32];
    float price;
}

// 声明
struct book cartoonBooks[12];
```

* 对于其数组元素中成员的访问逻辑为：数组成员 结构体运算符（`.`） 结构体成员。

```c
struct book{
    char name[32];
    float price;
}

// 声明
struct book cartoonBooks[12];

cartoonBooks[0].price = 12.50f;
```

#### 2.1.5 结构体嵌套

日常开发中，很多数据内容有不同的处理模式，故一个结构体中使用常规类型很难表述清楚。例：图书馆中书名、分类、价格、位置、是否借阅、借阅人信息等。

```c
struct student{
	char name[32];
    char idNums[32];
};

struct book{
    char name[32];
    float price;
};

struct libiray{
	struct student studts;
    struct book books;
};
```

#### 2.1.6 指向结构体的指针

* 声明

```c
struct student{
	char name[32];
    char idNums[32];
};

struct book{
    char name[32];
    float price;
};

struct student s1 = {"Anna", "00001"};
struct boos books[2];

// 对于单个变量
struct student *ps1 = &s1;

// 对于数组
struct student *pb1 = &books[0];
```

* 通过指针访问成员：指针`->`成员名

```c
struct book{
    char name[32];
    float price;
};

struct boos books[2];
struct student *pb1 = &books[0];

// 访问成员
printf("book name :%s\n", pb1->name);
printf("book name :%s\n", (*pb1).name);
printf("book name :%s\n", books[0].name);
```

#### 2.1.7 将结构体作为参数传递给函数

传递方式分为三种：传递结构体成员、传递结构体地址、传递结构

```c
struct book{
    char name[32];
    float price;
};

void printfBookName1(char* name)
{
    printf("book name :%s\n", name);
}
void printfBookName2(struct book* b1)
{
    printf("book name :%s\n", b1->name);
}
void printfBookName2(struct book b2)
{
    printf("book name: %s\n", b2.name);
}

struct book b1;
// 传递结构体成员
printfBookName1(b1.name);

// 传递结构体地址
printfBookName2(&b1);

// 传递结构
printfBookName2(b1);
```

#### 2.1.8 其他结构特性

* 相同类型的结构可以相互赋值。
* 可以用同类型的已初始化的结构初始同类型的结构。
* 结构可以作为返回值。

```c
/* names2.c -- passes and returns structures */
#include <stdio.h>
#include <string.h>

#define NLEN 30
struct namect {
    char fname[NLEN];
    char lname[NLEN];
    int letters;
};

struct namect getinfo(void);
struct namect makeinfo(struct namect);
void showinfo(struct namect);
char * s_gets(char * st, int n);

int main(void)
{
    struct namect person;
    
    person = getinfo();			// 结构作为返回值，同类型的结构赋值
    person = makeinfo(person);
    showinfo(person);
    
    return 0;
}

struct namect getinfo(void)
{
    struct namect temp;
    printf("Please enter your first name.\n");
    s_gets(temp.fname, NLEN);
    printf("Please enter your last name.\n");
    s_gets(temp.lname, NLEN);
    
    return temp;
}

struct namect makeinfo(struct namect info)
{
    info.letters = strlen(info.fname) + strlen(info.lname);
    
    return info;
}

void showinfo(struct namect info)
{
    printf("%s %s, your name contains %d letters.\n",
           info.fname, info.lname, info.letters);
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;
    
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n');   // look for newline
        if (find)                  // if the address is not NULL,
            *find = '\0';          // place a null character there
        else
            while (getchar() != '\n')
                continue;          // dispose of rest of line
    }
    return ret_val;
}
```

[^程序清单 14.8]: names2.c

#### 2.1.9 结构和结构指针的选择

要编写一个结构相关的函数，形参选择结构还是结构指针，开发者要更具其对于特性与当前场景结合进行分析。

* 结构：函数中获取是数据的拷贝，保护原数据，且观感上更复合逻辑。
* 结构指针：执行块，且能减少执行函数时的内存消耗。若虚保护原始数据，可以使用`const`限定符修饰结构指针。

#### 2.1.10 结构中的字符数组和字符指针

结构体中若要使用字符指针，**切记将其初始化**，未初始化的指针其所存地址所指向的内容是未知的。

```c
struct book{
    char* name;
    float price;
};

struct book b1 = {"Anna", 12.50f};	// 这里的字符指针指向静态存储区

struct book b2;

fgets(b2.name, stdin);		// 存放字符串的位置是未知的
```

#### 2.1.11 结构、结构指针和`malloc`

上述的结构中使用字符指针的风险，可以使用`malloc()`在动态内存中申请字符指针所需要的内存，并将首地址赋值给字符指针，即可解决相关风险。

* 个人经验：在工作中尽量字符数组：1.显著的表明字符串的容量；2.初始化方便；3.使用时会方便关注边界问题；缺点：若需要扩容字符数组，前后兼容很困难，字符指针可以没有这个问题。

#### 2.1.12 伸缩型数组成员 （C99）

伸缩型数组成员（*flexible array member*）：该数组不会立刻存在，使用这个成员可以编写合适的代码。

规则：

* 必须在结构末尾；
* 结构中至少有一个成员；
* 类型与普通数组，其`[]`中是空的;

该规则的本意是结合`malloc`函数，申请结构中的**常规内容**和**伸缩内容**的存储空间（`malloc(sizeof(struct flex) + n * sizeof(double))`）。

```c
// flexmemb.c -- flexible array member (C99 feature)
#include <stdio.h>
#include <stdlib.h>

struct flex
{
    size_t count;
    double average;
    double scores[];   // flexible array member
};

void showFlex(const struct flex * p);

int main(void)
{
    struct flex * pf1, *pf2;
    int n = 5;
    int i;
    int tot = 0;
    
    // allocate space for structure plus array
    pf1 = malloc(sizeof(struct flex) + n * sizeof(double));
    pf1->count = n;
    for (i = 0; i < n; i++)
    {
        pf1->scores[i] = 20.0 - i;
        tot += pf1->scores[i];
    }
    pf1->average = tot / n;
    showFlex(pf1);
    
    n = 9;
    tot = 0;
    pf2 = malloc(sizeof(struct flex) + n * sizeof(double));
    pf2->count = n;
    for (i = 0; i < n; i++)
    {
        pf2->scores[i] = 20.0 - i/2.0;
        tot += pf2->scores[i];
    }
    pf2->average = tot / n;
    showFlex(pf2);
    free(pf1);
    free(pf2);
    
    return 0;
}

void showFlex(const struct flex * p)
{
    int i;
    printf("Scores : ");
    for (i = 0; i < p->count; i++)
        printf("%g ", p->scores[i]);
    printf("\nAverage: %g\n", p->average);
}

/*
Result:
Scores : 20 19 18 17 16 
Average: 18
Scores : 20 19.5 19 18.5 18 17.5 17 16.5 16 
Average: 17
*/
```

* 注意：1.不能用此结构进行**赋值**；2.不能传递该结构作为**函数参数**，要使用地址；3.此结构**不能成为内嵌结构**。

#### 2.1.13 匿名结构（C11）

```c
struct person{
  int id;
  struct {char first[32], char last[32]};
};

// 初始化与其他结构相同
strcut person p1 = {100， {"Anna", "Grass"}};

// 成员访问
puts(p1.first);
```

### 2.2 联合

联合（*union*）：使用相同的内存，存储不同类型的成员。所占内存为最大成员的大小。其一次只能存储一个值。

#### 2.2.1 初始化方式：

* 使用另一联合初始化同一类型的联合；
* 初始化第一个元素；
* C99使用指定初始化器；

```c
union hold{
  	int digit;
    double bigfl;
    char letter;
};

union hold valA;
valA.letter = 'A';
union hold valB = valA;					// 使用另一联合初始化同一类型的联合
union hold valC = {89};					// 初始化第一个元素
union hold valD = {.bigfl = 118.20f};	// C99使用指定初始化器
```

#### 2.2.2 使用联合：使用结构运算符（`.`）若时指针的使用（`->`）

```c
union hold{
  	int digit;
    double bigfl;
    char letter;
};

union hold valA;
valA.letter = 'A';

union hold *pval = &valA;

printf("%c\n", valA.letter);
printf("%c\n", pval->letter);
```

#### 2.2.3 匿名联合

```c
strcut cat_data{
	char make[15];
    int statue;
    union{
     	char owner[32];
        int digist[8];
    }
};
```

### 2.3 枚举

枚举（*enumerated type*）：整形**常量**（从底层看是**`int`类型常量**，故很多时候会把表示程序状态枚举中的异常枚举声明为-1），可用于分类、程序状态等多种常见，提高程序可读性。

#### 2.3.1 枚举的声明与使用

```c
enum color{
    red,
    yello,
    bule,
    green
};

enum color currentColor;

scanf("%d", &currentColor);

if (red == currentColor)
{
    ... ...
}
else
{
    ... ...
}
```

#### 2.3.2 枚举默认值

默认情况下，枚举列表中的常量被赋予0、1、2、3。

#### 2.3.3 枚举赋值

可以在声明是对其中常量进行赋值

```c
enum level{
  low = 500,
  medium = 700,
  high = 2000,
  big				// 2001
};
```

对于未指定的常量，会根据距离他最近的赋值的常量依次赋值

### 2.4 `typedef`简介

`typedef`并不是创造一个类型，而是为已有类型创建一个标识符。它与`#define`有一些区别：

* `typedef`有编译器解释，`#define`由预处理器解释。
* `typedef`只能用于类型，不能用于常量。

**作用域**：取决于其**定义的位置**，若在函数中则是块作用域，若在所有函数外，则是文件作用域。

**命名规则**：遵循变量的命名规则。

#### 2.4.1 `#define`宏替换陷阱

```c
#define STRING char*

STRING p1, p2;		// char *p1, p2;

/*
p1是字符指针，p2是字符类型变量。
*/
```

若使用`typedef`

```c
typedef char* STRING;
STRING p1, p2;			// char *p1, *p2;
```

#### 2.4.2 为结构体命名，省略结构标签

```c
// 声明并定义标签rect
typedef struct Rect
{
    double x;
    double y;
}rect;

// 定义其变量是就可以不用加`struct`
rect r1 = {3.0, 6.0};

// 以上代码被翻译为
struct Rect r1 = {3.0, 6.0};
```

### 2.5 其他复杂类型

#### 2.5.1 复杂的数组与指针

```c
int *oof[3][4];		// 声明一个 3 x 4的二维数组，每个元素都是int类型的指针
int (*uff)[3][4];	// 声明一个指向3 x 4的二维数组的指针，该数组中包含int类型元素
int (* iof[3])[4];	// 声明一个内含3个指针元素的数组，每个指针元素都指向内含4个int类型的数组
```

#### 2.5.2 函数指针

```c
char* funp(int);		// 返回字符指针的函数		()运算符优先级高于*
char (*furmp)(int);		// 指向函数的指针，返回值为char类型
char (* flump[3])(int)	// 内含3个元素的数组，每个元素都是指向返回值类型为char的函数
```

#### 2.5.3 数组指针

```c
typedef int  arr5[5];
typedef arr5 *p_arr5;
typedef p_arr5 arrp10[10];

arr5 togs;		// 内涵5个int类型元素的数组
p_arr p2;		// 指向数组的指针，指向数组中内含5个int类型元素。
arrp10 p3;		// 内含10个元素的数组，每个元素都是指向内含5个int类型元素的指针。
```

### 2.6 函数和指针

#### 2.6.1 函数指针的声明

* 使用场景：常用于作为函数的参数，即给一个处理函数传入一个异常处理函数。
* 声明方式：返回类型、（* 标识符）（函数参数类型）

```c
void (*pf)(char* );		// 指向一个返回类型为void, 形参是char*类型的函数
```

* 技巧：若要声明一个特定函数的函数指针，将其函数名替换为`（*pf）`即可。

#### 2.6.2 函数指针的赋值

```c
void ToUpper(char* );
void (*pf)(char*);


pf = ToUpper;	// 有效
pf = ToUpper();	// 无效，Topper()不是地址，其返回值是void;
```

#### 2.6.3 函数指针的使用

```c
void ToUpper(char* );
void (*pf)(char*);


pf = ToUpper;	// 有效

char testC = 'A';

(*pf)(testC);		// 有效，pf指向ToUpper，所以（*pf）就是函数本身，调用合理；贝尔实验室C和UNIX开发执行第一标准
pf(testC);			// 有效，函数名是指针，指针可以和函数名互换，调用合理；伯利克的UNIX开发执行第二标准。 ANSI C均认可。
```

## 三、小结

本章主要说明了新增类型结构体、联合、枚举。C99中新增的指定初始化器。使用`typedef`为类型新增标识符。简单介绍函数指针。