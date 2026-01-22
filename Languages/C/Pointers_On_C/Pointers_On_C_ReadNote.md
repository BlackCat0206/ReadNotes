# Chantper_0X

***---- Li Jinsheng***

***

## 一、概述

本章为《Pointers On C》重点章节章内容，因多数内容在《C Primer Plus》中相同，本章只记录笔者认为的重点内容

## 二、内容

### 2.1 一维数组

* 数组名：数组名在**表达式**中使用时，它的值是一个**指针常量**。但不要混淆“数组和指针时相同的”，数组有确定的元素数量，而指针只是一个标量。在下列两种情况下，数组名不作为表达式：
  * `sizeof`运算符：其返回的是整个数组的大小。
  * `&`取地址运算符：返回的是一个指向数组的指针。
* 同类型的数组不能进行赋值，要使用`for`循环进行逐个赋值。

* **数组访问的本质**：“指针算数”。

```c
int array[10];

2[array];			// 会被编译器解释为：*(2 + array)
```

* 下标和指针：指针有时比下标更高效。在`for`循环中下标访问每次需要对下标进行求值，而指针可位移后直接访问。
* 作为函数参数的数组名：函数参数都是通过传值的方式，而数组名本身就是存放首元素地址的常量，函数可通过间接访问实现对数据的访问。传参的本身是对数组名的拷贝（**对指向数组首元素地址的指针的副本**），函数通过副本来操作数组，而不必担心对实参（指针）有修改，但可能会对其所指的内容。

```c
int strlen(char *string);
int strlen(char string[]);
```

* 数组作为函数的参数：函数不会为数组再次分配内存，函数接受的是一个指针，其已经指向分配好的空间。

### 2.2 多维数组

```c
int matrix[3][10];

int (*cp)[10];

cp = matrix;
```

* 多维数组可以理解为：
  * 指向内含N个元素的数组的指针。
  * 一个一维数组，其每个元素是内含N个元素的数组。

### 2.3 字符串

#### 2.3.1 查找一个字符

```c
char* strchar(char const *str, int ch);
char* strrchr(char const *str, int ch);
```

#### 2.3.2 查找几何字符串

```c
char* strpbrk(char const *str, char const* group);
```

* 在`str`中第一个匹配到`group`中任意一个字符。

#### 2.3.3 查找一个子串

```c
char* strstr(char const *s1, char const *str2);
```

#### 2.3.4 查找一个字符前缀

```c
size_t strspn(char const *str, char const *group);
size_t strcspn(char const *str, char const *group);
```

示例：

```c
#include <stdio.h>
#include <string.h>

int main() {
    const char *str = "abc123def";
    const char *group1 = "abc";   // 包含'a','b','c'
    const char *group2 = "abc12"; // 包含'a','b','c','1','2'
    
    // 计算str中前缀包含在group1中的长度
    size_t len1 = strspn(str, group1);
    printf("前缀包含在\"%s\"中的长度: %zu\n", group1, len1); // 输出3（'a','b','c'）
    
    // 计算str中前缀包含在group2中的长度
    size_t len2 = strspn(str, group2);
    printf("前缀包含在\"%s\"中的长度: %zu\n", group2, len2); // 输出5（'a','b','c','1','2'）
    
    return 0;
}
```

* 不是`strstr()`函数，只要字符匹配即可，不在意顺序。

#### 2.3.5 查找标记

```c
char* strtok(char* str, char const *sep);
```

* 通过查找出`str`中的第一个分隔符，将其置为`NULL`,将后续内容存储，并返回指向该字符的指针。若`str`入参是`NULL`则继续查找，并返回指针，若没有了，则返回空。

### 2.4 高级指针话题

#### 2.4.1 高级声明

```c
int *f, g;
```

* 解引用运算符`*`只能单独作用，故上述示例中是一个指针和一个变量。

```c
int *f();
```

* `()`函数调用操作符优先级高于解引用运算符，故这是一个返回值为`int`指针类型的函数。

```c
int (*f)();
```

* 一个指向返回值为`int`类型的函数的指针。

```c
int *(*f)();
```

* 一个指向返回值为指向`int`类型指针的函数的指针。

```c
int f[];
```

* 整形数组

```c
int *f[];
```

* 一个内含有N个指向`int`类型指针的数组。

```c
// 非法声明
int f()[];
int f[]();
```

* 上述两种是非法声明

```c
int (*f[])();
```

* 一个数组，元素类型为指向返回值为`int`类型的函数的指针。

```c
int *(*f[])();
```

* 一个数组，元素类型为指向返回值为指向`int`类型的指针函数的指针。

#### 2.4.2 函数指针数组的示例

```c
double add (double, double);
double sub (double, double);
double mul (double, double);
double div (double, double);

double (*oper_funs[])(double, double) = {add, sub, mul, div};
```

#### 2.4.3 字符串常量

字符串常量在表达式中，它的值是一个**常量指针**。

```c
"xyz" + 1;
```

它的结果是一个指针，指向的内容是`y`;

```c
*"xyz";
```

解引用，即访问第一个元素，结果是`x`;

```c
"xyz"[2];
```

数组下标访问，其结果是`z`;

注：若越界访问其结果是未定义的。
