# Chapter_12_存储类型、链接和内存管理

***---- Li Jinsheng***

***

## 一、概述

本章为《C Prime Plus》（第6版）第12章读书笔记

## 二、内容

### 2.1 存储类别

#### 2.1.1 作用域

作用域用来表述可访问标识符的区域。

* 块作用域：在花括号括起来中声明的局部变量。注形参也具有块作用域，其归于函数。
* C99标准允许在块中**任意位置**声明变量。将块作用域的引入**`for`、`while`和`do while`循环与`if`语句**中，即这些代码即使没有被花括号括起来，仍然算是块的一部分。故下面示例的变量`i`的作用域是`for`循环，若脱离`for`循环，则无法访问变量`i`。

```c
for (int i = 0; i < 3; i++)
{
    statement;
}
```

* 函数作用域（*function scope*）：用于`goto`语句，若其声明的标签（*lable*）在函数中，则其作用域就是整个函数。其本质是为了规避两个函数管理相同的标签导致的混乱。
* 函数原型作用域（*function prototype scope*）：**函数原型的形参名**。其范围是从形参定义开始到函数原型声明结束。意味着编译器只关注函数参数的类型，而不关注参数名称。（不知道有什么用）

```c
void test(int num1, num2, num3);
```

* 文件作用域（*file scope*）：也被称为**全局变量**（*global value*）定义在函数外的变量，具有文件作用域，即整个文件中的函数均可使用它。

```c
#incldue <stdio.h>
int nums1 = 22;

void test(void)
{
    printf("%d\n", nums1);
}
```

* 翻译单元（*translation unit*）：一个代码文件和它所包含的都文件。

#### 2.1.2 链接

函数具有三种链接：**外部链接**、**内部链接**、**无链接**。

外部链接：可以在多个程序文件中使用。

内部链接：只能在一个翻译单元中使用。

无连接：在其声明的作用域中使用。

块作用域、函数作用域、函数原型作用域都是无链接。文件作用域可能是外部链接或内部链接。其区分方式为是否有`static`

```c
int giants = 5;				// 文件作用域，外部链接
static int dodgers = 3; 	// 文件作用域，内部链接
```

#### 2.1.3 存储期

描述可访问数据的生存器，C分为四种：

* **静态存储期**：程序执行期间一直存在。文件作用域变量具有静态存储期。
* **线程存储期**：用于并发程序设计，从声明到结束。用关键字`_Thread_local`声明的变量，每个线程都会有其**私有备份**。
* **自动存储期**：块作用域变量都是，从进入声明开始分配内存，退出这个块时被销毁。
* **内存分配存储期**：后续介绍

**五种存储类别**：自动、寄存器、静态外部链接、静态内部链接、静态无链接

| 存储类型     | 作用域     | 链接     | 存储期     | 声明方式                       |
| ------------ | ---------- | -------- | ---------- | ------------------------------ |
| 自动         | 块作用域   | 无链接   | 自动存储期 | 块内                           |
| 寄存器       | 块作用域   | 无链接   | 自动存储期 | 块内，使用关键字`register`     |
| 静态外部链接 | 文件作用域 | 外部链接 | 静态存储期 | 所有函数外                     |
| 静态内部链接 | 文件作用域 | 内部链接 | 静态存储期 | 所有函数外，使用关键字`static` |
| 静态无链接   | 块作用域   | 无连接   | 静态存储期 | 块内，使用关键字`static`       |

#### 2.1.4 自动变量

自动变量：块作用域、无链接、自动存储期。

* 内侧块会隐藏外层块的定义。C99中没有花括号的块后的内容，即子块（*sub-block*）也遵循该规则。
* 自动变量不会自动初始化，其内存中是随机值。

```c
// forc99.c -- new C99 block rules
#include <stdio.h>
int main()
{
    int n = 8;
    
    printf("   Initially, n = %d at %p\n", n, &n);
    for (int n = 1; n < 3; n++)
        printf("      loop 1: n = %d at %p\n", n, &n);
    printf("After loop 1, n = %d at %p\n", n, &n);
    for (int n = 1; n < 3; n++)
    {
        printf(" loop 2 index n = %d at %p\n", n, &n);
        int n = 6;
        printf("      loop 2: n = %d at %p\n", n, &n);
        n++;
    }
    printf("After loop 2, n = %d at %p\n", n, &n);
    
    return 0;
}

/*
Result:
   Initially, n = 8 at 000000000061FE4C
      loop 1: n = 1 at 000000000061FE48
      loop 1: n = 2 at 000000000061FE48
After loop 1, n = 8 at 000000000061FE4C
 loop 2 index n = 1 at 000000000061FE44
      loop 2: n = 6 at 000000000061FE40
 loop 2 index n = 2 at 000000000061FE44
      loop 2: n = 6 at 000000000061FE40
After loop 2, n = 8 at 000000000061FE4C
*/
```

[^程序清单 12.2]: forc99.c

**编译时如何使用C99/C11的规则:**

```shell
gcc -std=c11 forc99.c -o a.out

gcc -std=c99 forc99.c -o a.out
```

#### 2.1.5 寄存器变量

```c
register int num = 1;
```

* 存放位置：CPU的寄存器中。
* 特点：使用关键字`register`声明，无法获取其地址，声明时编译器会根据寄存器空间衡量请求，可能不被允许。
* 注：不能对寄存器变量使用取地址运算符。

#### 2.1.6 块作用域的静态变量

* 静态变量（*static value*）：只存储数据的地址不变。
* 块作用域的静态变量：具有**块作用域**、**无链接**、**静态存储期**。
* 存储类型的静态无链接，中提到在函数中声明一个静态变量，从块离开后，其变量是无法访问，但是其具有静态存储期，即其值是不会消失的，再次执行函数时其值会保持上一次存储的内容。

```c
/* loc_stat.c -- using a local static variable */
#include <stdio.h>
void trystat(void);

int main(void)
{
    int count;
    
    for (count = 1; count <= 3; count++)
    {
        printf("Here comes iteration %d:\n", count);
        trystat();
    }
    
    return 0;
}

void trystat(void)
{
    int fade = 1;
    static int stay = 1;
    
    printf("fade = %d and stay = %d\n", fade++, stay++);
}

/*
Result:
Here comes iteration 1:
fade = 1 and stay = 1
Here comes iteration 2:
fade = 1 and stay = 2
Here comes iteration 3:
fade = 1 and stay = 3
*/
```

[^程序清单 12.3]: loc_stat.c

#### 2.1.7 外部链接的静态变量

外部链接的静态变量特性：文件作用域（从声明到文件结尾）、外部链接、静态存储期。

* 声明与初始化：在左右函数外进行声明，若未初始化则默认为0。
  * 定义声明：定义外部静态变量。
  * 引用声明：`extern`告诉编译器`main()`函数中使用**定义在外部**的`num1`变量。
  * 若定义声明时未初始化，则默认未0。
  * 只能用**常量表达式初始化静态变量**。
* 标识符名称:
  * 老版本，局部变量识别前31个字符，外部变量识别前6个字符。
  * C99/C11：局部变量识别前63个字符，外部变量识别前31个字符。

```c
// 定义声明（defining declaration）
int num1 = 1;

int num2 = num1;	// 不允许的

int main(void)
{
    // 引用声明（referencing declaration）
    extern int num1;
}
```

#### 2.1.8 内部链接的静态变量

内部链接的静态变量特性：文件作用域、内部链接、静态存储期、`static`定义

* 声明和初始化：与外部链接的静态变量相比，需要新增一个`static`关键字

* 使用时也可通过`extern`关键字进行引用声明，但只能在**同一个翻译单元**中。

```c
static int num1 = 1;

int main(void)
{
    extern int num1;
}
```

#### 2.1.9 多文件

开发过程中需要多个文件控制一个数据时，就需要使用外部链接变量。一个外部变量定义在一个文件中，其他文件若要使用该变量，需要使用关键字`extern`进行引用声明。

#### 2.1.12 存储类别的函数

函数的存储类型：外部函数（默认）、静态函数和内联函数（C99）

```c
int test(void);				// 外部函数
static int test2(void);		// 内部函数
extern int test3(void);		// 引用声明：表明当前使用的test3()函数定义在别处。
```

### 2.2 随机函数和静态变量

`rand()`函数：ANSI C提供生成随机数，事实上它是一个"伪随机数生成器"，其结果是可预测的。若要进行不可预测性，需要重置种子。

### 2.3 分配内存：`malloc()`和`free()`

#### 2.3.1 `malloc（）`和`free()`函数

```c
void* malloc(size_t size);
```

`malloc()`入参所需要内存的字节数，返回分配动态内存的首地址，需要用所需类型指针去接受，返回类型需要进行强制转换。

```c
int *pNum = (int*)malloc(4 + sizeof(int));
```

* ANSI C中引入新类型，`void`类型指针，可以理解为通用指针，可以转换为任何类型。

```c
free(void* ptr);
```

* `free()`函数的入参是指针类型，是之前`malloc()`函数所申请返回内存的首地址，由`free()`函数释放申请的内存。
* `malloc()`与`free()`均声明在`stdlib.h`头文件中。

* 内存泄漏（*memory leak*）：程序在运行的过程中内存耗尽。即可能没有即使的释放`malloc()`申请的动态内存。

#### 2.3.2 `calloc()`函数

```c
void* calloc(size_t baseNums, size_t baseSize);
```

* `calloc()`使用两个参数，第一个是说明所需要存储单元的数量，第二个是存储单元的大小（以字节为单位）。
* 它会将申请内存中的所位置初始化为0。

```c
long *lptr = NULL;
lptr = (long*)calloc(100, sizeof(long));
free(lptr);
```

#### 2.3.3 动态内存和变长数组

* 根据指针所学内容，其和数组有很大的相同之处。故也可以使用`malloc()`来申请数组内存。即现在有三种数组的声明方式：
  * 使用常量表达式声明数组的维度，使用数组名访问数组元素。
  * 变长数组，是变量表达式声明数组的维度，使用数组名访问数据元素。
  * 声明一个指针，使用`malloc()`，返回分配内存地址，使用指针访问数组元素。指针可以是自动或静态的。
* 二维数组示例：

```c
#include <stdlib.h>

int main(void)
{
	int n = 5;
    int m = 6;
    
    int ar2[n][m]; 		// 变长数组
    int (*p2)[6];		// C99之前的写法
    int (*p3)[m];		// C99支持变长数组
    
    p2 = (int (*)[6])malloc(n * 6 * sizeof(int));	// n x 6 数组
    p3 = (int (*)[m])malloc(n * m * sizeof(int));	// m x n 数组
}
```

### 2.4 ANSI C类型限定符

C90新增属性：恒常性（*constancy*）与可变性（*volatility*），可以分别用`const`和`volatile`修饰。

C99限定符：`restrict`用于提高编译效率。

C11限定符：`_Atomic`，提供一个可选库。`stdatomic.h`用于支持并发程序设计。

#### 2.4.1 `const`类型限定符

* 在指针和形参中使用`const`
  * 形参中使用表明其对应函数只能使用实参的数据，但不能修改。
  * 指针：
    * 指向常量的指针（*pointer to const*）：`const int* p;`即不能通过指针修改其所指向内容。
    * 常量指针（*const pointer*）：`int* const p;`不能修改指针所保存的地址。
    * `const int* const p`:不能修改指针地址与不能通过指针修该其所指内容。
* 对全局变量使用`const`：全局变量存在数据安全风险，使用const限定即可规避风险。

```c
const int num1 = 2;		// 定义声明

extern const int num2;	// 引用声明
```

* 将数据存放在头文件中用`static`和`const`修饰，若需要则包含对应头文件即可。

```c
// testConst.h

static const double PI = 3.1415926;
static const char* name = {"Anna", "Boob"};
```

* 注：**一定要用`static`进行限定，否则包含了该头文件的文件都有一个同名的全局变量**。

#### 2.4.2 `volatile`类型限定符

`volatile`限定符，用来告知编译器，其所限定的变量可能会被“意想不到”地修改，应该**每次访问变量从内存中读取和写入**。

目的：**规避编译器优化**（从内存中读取一次并存入寄存器中）。

示例：硬件设备的寄存器地址多数会映射到内存的特定位置，这些寄存器的值可能会被硬件修改（与程序无关），故需要使用`volatile`修饰，避免编译器认为变量不会被修改，只读取一次并缓存在寄存器中，无法感知硬件状态。

```c
#include <stdio.h>

// 假设0x12345678是一个硬件状态寄存器的地址
// 该寄存器的最低位表示设备是否就绪（1=就绪，0=忙碌）
volatile unsigned int *status_reg = (unsigned int *)0x12345678;

int main() {
    // 等待硬件就绪
    while ((*status_reg & 0x01) == 0) {
        // 循环等待，直到最低位变为1
    }
    printf("硬件已就绪\n");
    return 0;
}
```

* 可以与`const`联合使用：`const volatile int x;`表示该变量只读可能被外部修改。

#### 2.4.3 `restrict`类型限定符

只能修饰**指针**：表示该指针是访问其所指**对象唯一且初始的方式**（**没有其他指针访问同一内存**）。告知编译器可以假定一些优化方案，即**消除编译器对指针别名带来的优化限制**。

```c
#include <stdio.h>

// 自定义内存复制函数：将src指向的n字节数据复制到dest
void my_memcpy(void *restrict dest, const void *restrict src, size_t n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i]; // 编译器知道d和s不指向同一块内存，可优化
    }
}

int main() {
    int src[] = {1, 2, 3, 4, 5};
    int dest[5];
    
    my_memcpy(dest, src, sizeof(src));
    
    for (int i = 0; i < 5; i++) {
        printf("%d ", dest[i]); // 输出：1 2 3 4 5
    }
    return 0;
}

```

* 注：这是一个**承诺**，若开发者不遵循，其结果是未定义的。
* 可以与`const`公用：`const int* restrict p;`访问。
* `C++`兼容性：`C++`中没有`restrict`限定符，但部分编译器（GCC、Glang有`_restrict_`）

#### 2.4.4 `_Atomic`类型限定符（C11）

* 只能使用宏函数（`stdatomic.h`中）访问原子类型。
* 当一个线程操作一个原子类型的对象时，其他线程不能访问该对象。

## 三、小结

本章主要介绍了存储的类别：静态内存的数量在编译时确定，静态数据在载入程序时载入，自动变量在程序运行只声明块时分配，运行至对应块结束释放，动态内存由函数空。使用`malloc`、`free()`与`calloc()`进行动态内存管理。介绍了ANSI C中常用的限定符与对应用法。





