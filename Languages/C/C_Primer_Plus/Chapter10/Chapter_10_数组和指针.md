# Chapter_10_数组和指针

***---- Li Jinsheng***

***

## 一、概述

本章为《C Primer Plus》（第6版）第10章读书笔记

## 二、内容

### 2.1 数组

#### 2.2.1 数组的初始化

* 参数列表

```c
int arrayNums[4] = {1, 2, 3, 4};
// 结果： 1， 2，3，4

int arrayNums2[4] = {1 , 2};
// 结果： 1，2，0，0
```

* 参数列表中参数数量小于数组元素个数时，编译器也会将其他元素初始化为0。

#### 2.2.2 C99指定初始化

* C99引入新特性，可以**指定数组元素**进行初始化：

```c
int nums[3] = {[2] = 3};	// 这里使用的时数组的下标
// 结果：0，0，3


int nums2[8] = {100, [2] = 2, 3, 4, [0] = -1};
// 结果：-1 0 2 3 4 0 0 0

int nums3[] = {1, [3] = 7}
// 结果：1， 0， 0， 7
```

* 指定下标后的初始化元素会在**指定下标后依次填充**。
* 若再次初始化指定元素，之前初始化的值会被覆盖。
* 若未指定元素大小，C编译器会将数组设置为可容纳的大小。

#### 2.2.3 数组边界

* 强调：C编译器无法感知使用数组下标正确性，在使用数组时一定要注意数组的边界。

#### 2.2.4 指定数组的大小

* C99之前，值允许使用**整形常量表达式**指定数组的大小，且表达式必须大于0。注：**`sizeof`也被视为整形常量表达式，与C++不同const值不是。**

```c
int n = 5;
int m = 9;
int a1[5];
int a2[5*2 + 1];
int a3[sizeof(int) + 1];
int a4[-4];						// 不被允许
int a5[0];						// 不被允许
int a6[2.5];					// 不被允许
int a7[(int)2.5];				// 允许，
int a8[n];						// C99之前不被允许
int a9[m];						// C99之前不被允许
```

* C99引入可变数组（*variable-length array*）标准。C11 将可变数组（VLA）列为 “可选特性”，而非完全删除，部分编译器（如 GCC）仍支持。了解即可，若要使用，切记对变量进行符号判断。

### 2.2 多维数组

#### 2.2.1 二位数组及其初始化

```c
int nums[2][3] = {
    {1, 2},
    {3, 4}
};

/*结果：
1， 2， 0
3， 4， 0
*/ 

int nums1[2][3] = {
    1, 2, 3, 4
};

/*
结果：
1， 2， 3
4， 0， 0
*/
```

### 2.3 指针和数组

* 指针的值是其所指向对象（可以结合第5章中2.2.1.1 左值来理解对象，数据存储）的地址。地址的格式根据不同的系统会有所不同，大部分系统是按字节规划。
* 需要通过解引用运算符`*`获得指针所指向对象的值。
* 指针+1：偏移指针所指向对象的类型大小（以字节为单位）。

```c 
// pnt_add.c -- pointer addition
#include <stdio.h>
#define SIZE 4
int main(void)
{
    short dates [SIZE];
    short * pti;
    short index;
    double bills[SIZE];
    double * ptf;
    
    pti = dates;    // assign address of array to pointer
    ptf = bills;
    printf("%23s %15s\n", "short", "double");
    for (index = 0; index < SIZE; index ++)
        printf("pointers + %d: %10p %10p\n",
               index, pti + index, ptf + index);
    
    return 0;
}

/**
Result:
                  short          double
pointers + 0: 000000000061FE30 000000000061FE10
pointers + 1: 000000000061FE32 000000000061FE18
pointers + 2: 000000000061FE34 000000000061FE20
pointers + 3: 000000000061FE36 000000000061FE28
*/
```

[^程序清单 10.8]: pnt_add.c

* 解引用运算符优先级：参考C运算符优先级表格。

```c
int num1[10] = {0};
int *pnum1 = num1;

*(pnum1 + 2);		// 偏移两个元素大小后的地址，等价于 num1[2]
*pnum1 + 2;		// *优先级高于+，当前元素+2
```

### 2.4 函数、数组和指针

* 数组名即使数组的首地址，若需使用函数处理数组，可将函数的形参声明为指针类型

```c
void test(int* nums);
```

* `[]`在工作中，在部分功能函数头文件中会遇到如下函数声明：

```c
int sum(int ar[], int size);
```

* 注：`[]`符号只能用于**函数原型**或**函数定义头**中（即函数形参声明时）。它提醒用户，ar是一个指向`int`类型数组的指针。

```c
int sum(int *ar, int size);
int sum(int *, int);
int sum(int ar[], int size);
int sum(int [], int);

/* 函数定义时不能省略参数名 */
int sum(int* ar, int size)
{
    statement;
}

int sum(int ar[], int size)
{
    statement;
}
```

* 一元运算符`*`和`-- ++`混合使用

```c
int nums[5] = {0, 1, 2, 3, 4};
int *p = nums;

*++p;		// 两个运算符优先级相同，根据结合律从右向左。先指针递增再取值，结果：1;
*p++;		// 两个运算符优先级相同，根据结合律从右向左。先指针递增（后缀，还是使用原有的值）再取值：结果：1；
(*p)++;		// 先取值，再递增。改变了数组元素！！！
```

| 数组形式  | 指针形式（假设`p = arr`） |
| --------- | ------------------------- |
| `arr[i]`  | `*(p + i)`                |
| `&arr[i]` | `p + i`                   |
| `arr`     | `&arr[0]`                 |

示例：

```c
/* order.c -- precedence in pointer operations */
#include <stdio.h>
int data[2] = {100, 200};
int moredata[2] = {300, 400};
int main(void)
{
    int * p1, * p2, * p3;
    
    p1 = p2 = data;
    p3 = moredata;
    printf("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3);		// 注意前后地址的变化
    printf("  *p1 = %d,   *p2 = %d,     *p3 = %d\n",
           *p1     ,   *p2     ,     *p3);
    printf("*p1++ = %d, *++p2 = %d, (*p3)++ = %d\n",
           *p1++     , *++p2     , (*p3)++);
    printf("  *p1 = %d,   *p2 = %d,     *p3 = %d\n",
           *p1     ,   *p2     ,     *p3);
    printf ("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3);
    
    return 0;
}

/*
Result:
p1 = 0000000000403010, p2 = 0000000000403010, p3 = 0000000000403018
  *p1 = 100,   *p2 = 100,     *p3 = 300
*p1++ = 100, *++p2 = 200, (*p3)++ = 300
  *p1 = 200,   *p2 = 200,     *p3 = 301
p1 = 0000000000403014, p2 = 0000000000403014, p3 = 0000000000403018
*/
```

[^程序清单 10.12]: order.c

* 注：在递增或递减指针时要注意，C编译器只保证原始指针的第一个元素有效，其他不保证。故要确保不会用到**越界指针**。

#### 2.4.1 指针的计算

```c
// ptr_ops.c -- pointer operations
#include <stdio.h>
int main(void)
{
    int urn[5] = {100,200,300,400,500};
    int * ptr1, * ptr2, *ptr3;
    
    ptr1 = urn;         // assign an address to a pointer
    ptr2 = &urn[2];     // ditto
    // dereference a pointer and take
    // the address of a pointer
    printf("pointer value, dereferenced pointer, pointer address:\n");
    printf("ptr1 = %p, *ptr1 =%d, &ptr1 = %p\n",
           ptr1, *ptr1, &ptr1);
    
    // pointer addition
    ptr3 = ptr1 + 4;
    printf("\nadding an int to a pointer:\n");
    printf("ptr1 + 4 = %p, *(ptr4 + 3) = %d\n",
           ptr1 + 4, *(ptr1 + 3));
    ptr1++;            // increment a pointer
    printf("\nvalues after ptr1++:\n");
    printf("ptr1 = %p, *ptr1 =%d, &ptr1 = %p\n",
           ptr1, *ptr1, &ptr1);
    ptr2--;            // decrement a pointer
    printf("\nvalues after --ptr2:\n");
    printf("ptr2 = %p, *ptr2 = %d, &ptr2 = %p\n",
           ptr2, *ptr2, &ptr2);
    --ptr1;            // restore to original value
    ++ptr2;            // restore to original value
    printf("\nPointers reset to original values:\n");
    printf("ptr1 = %p, ptr2 = %p\n", ptr1, ptr2);
    // subtract one pointer from another
    printf("\nsubtracting one pointer from another:\n");
    printf("ptr2 = %p, ptr1 = %p, ptr2 - ptr1 = %td\n",
           ptr2, ptr1, ptr2 - ptr1);						// 相差两个int单位，不是相差两个字节！！！
    // subtract an integer from a pointer
    printf("\nsubtracting an int from a pointer:\n");
    printf("ptr3 = %p, ptr3 - 2 = %p\n",
           ptr3,  ptr3 - 2);
    
    return 0;
}

/*
Result:
pointer value, dereferenced pointer, pointer address:
ptr1 = 000000000061FE30, *ptr1 =100, &ptr1 = 000000000061FE28

adding an int to a pointer:
ptr1 + 4 = 000000000061FE40, *(ptr4 + 3) = 400

values after ptr1++:
ptr1 = 000000000061FE34, *ptr1 =200, &ptr1 = 000000000061FE28

values after --ptr2:
ptr2 = 000000000061FE34, *ptr2 = 200, &ptr2 = 000000000061FE20

Pointers reset to original values:
ptr1 = 000000000061FE30, ptr2 = 000000000061FE38

subtracting one pointer from another:
ptr2 = 000000000061FE38, ptr1 = 000000000061FE30, ptr2 - ptr1 = 2

subtracting an int from a pointer:
ptr3 = 000000000061FE40, ptr3 - 2 = 000000000061FE38
*/
```

[^程序清单10.13]: ptr_ops.c

* **指针相减的结果于数组类型的单位相同**。

* 注：数组名不是指针，数组名递增递减时无效的。
* 注：**不要解引用未初始化的指针**

```c
int *p;
*p = 5;		// 错误用法！！！， 指针中地址是未知的。
```

* 无效语句

```c
int nums[3];
int *pNums1， *pNums2;

nums++;		// 无效；
pNums2 = pNums2 + pNums1;	// 无效
pNUms2 = nums * pNums1;		// 无效
```

#### 2.4.2 `const`其他内容

* `const` 修饰形参：告知编译器该函数不能修改`const`所修饰的实参。
* **指向`const`指针**：不能通过指针修改所指向内容。

```c
int nums[3] = {1, 2, 3};
const int *pNums = nums;

(*p) = 3;		// 不允许
```

* `const`指针可以通过`const`数据或非`const`数据的地址进行初始化。

```c
int nums[3] = {1, 2, 3};
cosnt int nums2[3] = {4, 5, 6};

const int *pNums;
pNums = nums;		// 允许
pNums = nums2;		// 允许
```

* 普通指针只能接受相同类型的非`const`数据地址进行初始化。
* 将`const`数据传递给非`const`的形参，其结果是未知的。
* **`const`指针**：不能修改指针所存储的地址，可以修改其所指对象的数据。

```c
int nums[3] = {1, 2, 3};
int* const pNums = &nums[2];

pNums = &nums[0];	// 不允许
*pNums = 4;			// 允许
```

### 2.5 多维数组和指针

```c
/* zippo1.c --  zippo info */
#include <stdio.h>
int main(void)
{
    int zippo[4][2] = { {2,4}, {6,8}, {1,3}, {5, 7} };
    
    printf("   zippo = %p,    zippo + 1 = %p\n",
           zippo,         zippo + 1);
    printf("zippo[0] = %p, zippo[0] + 1 = %p\n",
           zippo[0],      zippo[0] + 1);
    printf("  *zippo = %p,   *zippo + 1 = %p\n",
           *zippo,        *zippo + 1);
    printf("zippo[0][0] = %d\n", zippo[0][0]);
    printf("  *zippo[0] = %d\n", *zippo[0]);
    printf("    **zippo = %d\n", **zippo);
    printf("      zippo[2][1] = %d\n", zippo[2][1]);
    printf("*(*(zippo+2) + 1) = %d\n", *(*(zippo+2) + 1));
    
    return 0;
}

/*
注解：
1.
zippo == &zippo[0]
zippo[0] == &zippo[0][0]
因为是首地址，所以zippo == zippo[0]
2.
zipp0 + 1会偏移2个int类型大小（若第二维度是3，则偏移3个int类型），zippo[0] + 1会偏移1个int类型大小。

3.
*zippo == *(&zipp0[0]) == &zippo[0][0]
故若需要获取其原始值
**zippo == *(*(&zipp0[0])) == *(&zippo[0][0]) = zippo[0][0]

*/


/*
   zippo = 000000000061FE30,    zippo + 1 = 000000000061FE38
zippo[0] = 000000000061FE30, zippo[0] + 1 = 000000000061FE34
  *zippo = 000000000061FE30,   *zippo + 1 = 000000000061FE34
zippo[0][0] = 2
  *zippo[0] = 2
    **zippo = 2
      zippo[2][1] = 3
*(*(zippo+2) + 1) = 3

*/
```

* zippo是地址的地址，必须解引用两次，这种操作被称为**双重间接**（*double indirection*）。

#### 2.5.1 指向多维数组的指针

如何声明一个指针指向一个二维数组？

```c
int (*pz)[2];		// 指向一个含有两个int类型值的数组

int* pt[2];			// 含有两个int类型指针的数组
```

* （）先解释为指针，指向数组。
* `[]`运算优先级高于解引用`*`故先于pt结合，先解释为数组。

```c
/* zippo2.c --  zippo info via a pointer variable */
#include <stdio.h>
int main(void)
{
    int zippo[4][2] = { {2,4}, {6,8}, {1,3}, {5, 7} };
    int (*pz)[2];
    pz = zippo;
    
    printf("   pz = %p,    pz + 1 = %p\n",
           pz,         pz + 1);
    printf("pz[0] = %p, pz[0] + 1 = %p\n",
           pz[0],      pz[0] + 1);
    printf("  *pz = %p,   *pz + 1 = %p\n",
           *pz,        *pz + 1);
    printf("pz[0][0] = %d\n", pz[0][0]);
    printf("  *pz[0] = %d\n", *pz[0]);
    printf("    **pz = %d\n", **pz);
    printf("      pz[2][1] = %d\n", pz[2][1]);
    printf("*(*(pz+2) + 1) = %d\n", *(*(pz+2) + 1));
    
    return 0;
}

/*
Result:
   pz = 000000000061FE20,    pz + 1 = 000000000061FE28
pz[0] = 000000000061FE20, pz[0] + 1 = 000000000061FE24
  *pz = 000000000061FE20,   *pz + 1 = 000000000061FE24
pz[0][0] = 2
  *pz[0] = 2
    **pz = 2
      pz[2][1] = 3
*(*(pz+2) + 1) = 3
*/
```

#### 2.5.2 函数和多维数组

函数的形参若需要使用多维数组主要格式如下：

```c
// 1.
void test(int (*pt)[4]);

// 2.
void test2(int [][4]);
```

- 多维数组形参必须指定除第一维外的所有维度（如`void f(int arr[][3])`，`3`不可省略）。

```c
int sum(int [][4][5], int, rows);

int sum (int (*fp)[4][5], int rows);
```

* 对于N维数组，只能省略左边方括号中的值。

### 2.6 复合字面量

C99引入标准。了解即可。

```c
int diva[2] = {10, 20};

(int [2]){10, 20};
```

## 三、小结

本章主要对数组进一步讨论，多维数组可以将看为：**其内涵以数组为元素的数组，内涵数组中内涵着声明类型的元素**。

指针（pointer）是C语言以重要概念，需要反复学习与掌握。其存储着指向对象的地址，使用解引用符号获取内容。指针做差一般的结果是它们所差的类型个数。

下面是一些常用但绕口的名称：（根据运算优先级计算）

* **指针常量**：`int* const p`;本身是常量的指针，即指针的**地址不能修**。

* **常量指针**：`const int* p;`指向常量的指针，即不能通过指针修改其所指向内容。

* **指向const的const指针**：`const int* const p;`

* **指针数组**：`int *p[2];`本身是数组，其内涵个对应类型的x个指针。

* **数组指针**：`int (*p)[2];`本身是指针，指向数组。

* 二维数组，可以将其理解为一个指针数组。

**数组名作为函数参数时退化为指针**

* 当数组名作为`sizeof`操作数时（`sizeof(arr)`计算整个数组大小）；
* 当数组名作为`&`操作数时（`&arr`获取整个数组的地址，类型为 “指向数组的指针”）。