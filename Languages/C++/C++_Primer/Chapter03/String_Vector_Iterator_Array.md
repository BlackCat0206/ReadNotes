# 字符串、向量和数组

***---- Li Jinsheng***

## 一、概述

本章为《C++ Primer》（第5版）第三章读书笔记与拓展。

## 二、内容

### 2.1 命名空间`using`声明

```cpp
using 命名空间::名称;
```

使用`using`和域作用符（`::`）表明使用名称，用分号结尾。

### 2.2 标准库类型`string`

可变长度字符序列，使用时可使用如下头文件内容。

```cpp
#include <string>
using std::string;
```

#### 2.2.1 定义和初始化`string`对象

```cpp
string s1;				// 默认初始化，s1是一个空字符串
string s2 = s1;			// s2是s1的副本
string s3("Hello");		// s3构造为"Hello"
string s4 = "Hello";	// s4是字符串字面值副本
string s5(n, 'c');		// s5是由n个字符c初始化
```

* 拷贝初始化：使用赋值运算符`=`初始化一个对象，就是拷贝初始化（*copy initialization*）。若不使用等号，就是直接初始化（*direct initialization*）。

#### 2.2.2 `string`对象上的操作

```c
is >> s;					// 输入，遇到空白停止（空格、换行、制表）
os << s;					// 输出，将s输出到os流
getline(is ,s);				// 从is流读取一行，赋值给s。（不保留换行）
s.empty();					// 判断s是否为空
s.size();					// string::size_type 类型，无符号类型。返回s中字符个数
s[n];						// 访问其中第n个字符的引用。
s1+s2;						// 将s1与s2连接
s1 == s2;					// s1与s2长度与元素是否相同
s1 != s2;					// 对大小写敏感
<、<=、>=、>;				  // 按字典中顺序进行比较，对大小写敏感。
```

* 对`string`类型的读写

```cpp
#include<string>
#include<iostream>

using std::string;

using std::cin;
using std::cout;
using std::endl;

/*
* @brief 按单词读入
*/
void stringReadIn_1(void)
{
    string str;
    while(cin >> str)		// 按单词读入
    {
        cout << str << " ";
    }
    cout << "" << endl;
}

/*
* @brief 按行
*/
void stringReadIn_2(void)
{
    string str;
    while(getline(cin, str))		// 按行读入（不保存换行符）
    {
        cout << str << " ";
    }
    cout << "" << endl;
}
```

* `string`的`size()`

#### 2.2.3 `auto`和`decltype`可以推测`string::size_type`类型

* C++11 允许使用`auto`和`decltype`推测出`string`对象的长度类型

```cpp
string str(10, 'n');

auto size = str.size();			// string::size_type

decltype(str.size()) size = str.size();
```

* `string::size_type`是一个**无符号整数**，建议**不要和有符号一起运算**。第2章 （2.1.2 类型转换）说明，有符号和无符号混合运算，会将有符号转换为无符号，负数转换为无符号会是一个很大的整数。

#### 2.2.4 `string`对象的比较

* 若长度不同，但已有元素相同，则较短的字符< 较长的字符。
* 若元素不同，则结果为第一个不同字符的比较结果。

```c
string str1 = "Hello World!";
string str2 = "Hello";
string str3 = "Hiya";

/* str1 > str2、 str2 > str3  */
```

#### 2.2.5`string`对象和字符串字面值相加

* 赋值运算符右侧第一个应该是`string`类型，`+`两侧至少有一个是`string`类型。

```cpp
string str1 = "Hello";
string str2 = str1 + "World!!!" + '\n';
```

#### 2.2.6 对`string`对象的单独处理（《C Primer Plus》第11章 11.7 `cytpe.h`字符串函数）

#### 2.2.7 范围`for`语句处理每个字符（C++11）

* C++11中提供**范围for（*range for*）**，用来遍历给定序列的内容。

```CPP
for (declaration : expression)
{
    statement;
}
```

* 示例（）

```cpp
string str1("Hello World!!!");
for (auto c : str1)				// c是`char`类型
{
    cout << c << endl;
}

// 若想修改string对象的内容，需要声明c是引用
for (auto &c : str1)
{
    c = toupper(c);
}
```

* 这里是将`string`对象中每个对象**拷贝**给变量c；
* 若想修改string对象的内容，需要声明是**引用**；

### 2.3 标准库类型`vector`

`vector`表示对象的合集，其包含元素相同，若需使用要包含如下头文件与声明：

```cpp
#include <vector>
using std::vector;
```

#### 2.3.1 定义与初始化

```c
vector<T> v1;					// v1是一个空的vector,它潜在的元素类型是T，默认初始化
vector<T> v2(v1);				// v2中包含有v1所有的副本
vector<T> v3 = v1;				// v3是v1的拷贝，包含v1所有的副本
vector<T> v4(n, val);			// v4中包含了n个元素，每个元素都是val
vector<T> v5(n);				// v5中包含了n个元素，都进行了默认初始化
vector<T> v6{a, b, c, ...}		// v6初始值个数的元素，每个元素都会初始化
vector<T> v7 = {a, b, c, ...}	// 等同于v6
vector(开始位置指针，结束位置的下一个位置)。		// 数组初始化vector
```

#### 2.3.2 向`vector`中添加元素（`push_back`）

`push_back`向vector的尾部添加元素

```cpp
vector<string> vstr;

string strTemp;

while(getline(cin, strTemp))
{
    vstr.push_back(strTemp);
}
```

* **不要用范围`for`对`vector`添加元素**。（具体原因在第5章 2.2.2 范围`for`语句（不能使用其给`vector`添加元素的原因））

#### 2.3.4 `vector`对象能高效增长

* `C++`中`vector`对象可以实现高效增长，不必按照`C`规则，对`vector`先声明其大小，这样反而会导致降低其效率。

#### 2.3.5 `vector`对象操作

```cpp
v.empty();							// 判断是否为空
v.size();							// vector<T>::size_type类型，返回vector中元素的个数
v.push_back(t);						// 向v的尾端添加一个元素t;
v[n];								// 返回v中第n个元素的引用
v1 = v2;							// v2中的元素全部拷贝至v1
v1 = {a, b, c};						// 将列表中元素拷贝至v1并替换之前的元素
v1 == v2;							// v1与v2中元素数量与元素值都相同
v1 != v2;							//
<、<=、>、>=						  // 
```

* 注：`vector`返回的元素个数类型是：**`vector<T>::size_type`**，一定要带元素。
* 注：**不能用下标运算符添加元素，会导致缓冲溢出（*buffer overflow*）**。

### 2.4 迭代器

可以通过迭代器间接访问容器中的元素（类似于用指针访问数组一般）。其语法为：

```cpp
vector<int> v1(10, 0);		// 10个元素，均为0

auto it_begin = v1.begin();
auto it_end = v1.end();
```

* `v.begin()`：指向容器的首元素。
* `v.end()`：**指向容器的尾元素的下一个元素（*one past the end*）**。
* 若**容器为空**，则首迭代器和尾迭代器都都是**尾后迭代器**。

#### 2.4.1 迭代器的操作符

```cpp
*iter;					// 返回迭代器所指元素的引用
iter->mem;				// 解引用并访问所指元素的成员mem，等价于（*it）.mem
++iter;					// 令迭代器指向下一个元素
--iter;					// 令迭代器指向上一个元素
iter1 == iter2			// 判断两个迭代器是否指向同一个元素。
iter1 != iter2
```

* 尾迭代器不指向实际内容，不能进行解引用。

#### 2.4.2 迭代器的类型（C++ 11）

若希望通过迭代器只访问元素但不修改，可以使用`类型::const_iterator`类型，和**指向常量的指针（*pointer to `const`*）**相似。

```cpp
vector<int>::const_iterator it;		// vector<int>的迭代器，只能读
string::const_iterator it2;			// stringe类型的迭代器，只能读
```

* 若使用`auto`或者`decltype`推测迭代器类型，由对象本身决定是否是常量迭代器。

```cpp
vector<int> v1;
const vector<int> v2;

auto t1 = v1.begin();	// vector<int>::iterator
auto t2 = v2.begin();	// vector<int>::const_iterator类型
```

* C++11新增了`cbegin()`与`cend()`来获取迭代器的常量类型。

```cpp
auto t3 = v1.cbegin();	// vector<int>::const_iterator
```

* 注：用迭代器解引用一定要加括号，或者用箭头运算符（`->`）。

```cpp
*it.empty();	// 错误！！！！

(*it).empty();	// 正确
it->empty();	// 正确
```

#### 2.4.3 迭代器运算

```cpp
iter + n;					// 与指针类似
iter - n;					// 与指针类似
iter += n;					// 与指针类似
iter -= n;					// 与指针类似
iter2 - iter1;				// 类型::difference_type类型，参与运算的迭代器必须指向同一个容器。有符号整形
<、<=、>、>=				  // 判断指向位置
```

* 迭代器相减，要保证参与运算的迭代器指向同一个容器，其结果时`类型::difference_type`有符号类型。

```cpp
/*
 *@brief 用迭代器实现的二分法（后续完成）
 *@param [in] vector<int> v 有序数组
 *@param [in] int foundNum  要查找的数字
 *@return vector<int>::difference_type 下标，若无返回-1
 */
vector<int>::difference_type find_sum(vector<int>&v, const int &traget)
{
    auto begin = v.begin();
    auto end = v.end();
    auto mid = begin + (end - begin) / 2;
    while (begin < end && traget != (*mid))
    {
        if (traget < *mid)
        {
            end = mid;
        }
        else
        {
            begin = mid + 1;
        }
        mid = begin + (end - begin) / 2;		// C++编译器没有定义迭代器的加法运算
        
        
    }

    if (traget == *mid)
    {
        cout << "find it" << std::endl;
        return (mid - v.begin());
    }
    else
    {
        cout << "not find it" << std::endl;
        return -1;
    }
}

int main(void)
{
    vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
    int traget = 5;
    
    auto res = find_sum(v, traget);
    cout << "the traget index is " << res << std::endl;

    return 0;
}

/*
Result:
find it
the traget index is 4
*/
```

#### 2.4.4 迭代器的有效性

若容器`vector`或`string`类型的**大小有变化**，则原来的迭代器可能会**失效**。

### 2.5 数组

大致内容与`C`语言相同，具体可以参考《C Primer Plus》第10章 数组和指针，与《Pointers On C》《C Traps and Pitfalls》中内容，本次只列举一些`C++`独有和`C`中重点内容。

#### 2.5.1 数组不能拷贝

与`C`相同数组名是指向数组首元素的地址，若直接赋值是不允许的。（有的编译器可以，但在实际开发过程中要考虑到各种兼容情况，不能只依赖于最新的编译器）

```cpp
int a[] = {0, 1, 2};
int b[] = a;		// 错误；
b = a;				// 错误
```

#### 2.5.2 引用和数组

```cpp
int (&ref)[10];		// 引用一个函数10个int类型数组

int &ref1[10] = /* ? */;	// 不存在！！！
```

* **不存在引用的数组！！！**

#### 2.5.4 指针和数组（C++11）

使用`auto`或者`decltype`结合**数组名**会有不同的情况：

* `auto` 转换为指针

```c
int iarr[] = {0, 1, 2};

auto iap(arr);			// 指向int类型的指针
auto iap(&arr[0]);		// 与上面类型
```

* `decltype`转换为数组

```c
int iarr[] = {0, 1, 2};

decltype(iarr) iarr1 = {0, 1, 2};		// 解释为一个内含3个ine类型元素的数组。
```

#### 2.5.5 数组迭代器（C++ 11）

使用`begin(数组名)`与`end(数组名)`其返回是**元素的指针类型**。

```cpp
int ia[] = {0, 1, 2, 3, 4};

int *ia_begin = begin(ia);
int *ia_end = end(ia);		// 指向尾元素的下一位，不能解引用。
```

#### 2.5.6 `ptrdiff_t`类型

和C标准库一致，定义在`stddef.h`文件中，**有符号类型**。

#### 2.5.7 `string`对象和`C`风格的字符串

* 可以用C风格字符串（以结束符结尾的字符数组）初始化`string`对象，反之不行。

```c
char cstr1[] = {"Hello World!!!\n"};

string str = cstr1;		// 允许；

char* cstr2 = str;		// 不允许
```

* 可以通过`c_str()`来返回`string`对象的C风格字符串，其返回值是一个指针。注，**不能保证该数组一直有效**，若`string`对象的大小有变化可能会失效，**建议使用时拷贝一份**。

```c
string str("Hello World!!!");

char *p_cstr = str.c_str();
```

#### 2.5.8 用数组初始化`vector`

```cpp
vector(开始位置指针，结束位置的下一个位置)。
    
int int_arr[4] = {0, 1, 2, 3};
vector(int_arr+1, int_arr+4);		// 其中元素为int_arr[1]、int_arr[2]、int_arr[3]
```

### 2.6 多维数组

大致内容与`C`语言相同，具体可以参考《C Primer Plus》第10章 数组和指针，与《Pointers On C》中内容，本次只列举一些`C++`独有内容。

* C++ 11可以使用`auto`和`decltype`来推测指向数组的指针（*pointer to array*）类型。

```cpp
int ia[3][4];

for (auto p = ia; p != a + 3; ++p)
{
    for (auto q = *p; q != *p + 4; ++q)
    {
        cout << *q << ' ';
    }
    cout << endl;
}
```

* 类型别名简化指针

```c
typedef int int_array[10];		// int array是内含10个类型的数组
using int_array int[10];

int ia[3][4];
for (int_array *p = ia; p != ia + 4; ++p)
{
    for (int *q = *p; q != *p + 4; ++q)
    {
         cout << *q << ' ';
    }
    cout << endl;
}
```

## 三、C和C++差异

2.2.6 对`string`对象的单独处理（《C Primer Plus》第11章 11.7 `cytpe.h`字符串函数）对应读书笔记 **2.1.2 `ctype.h`系列字符函数** 判断字符是空格、数字、空字符、表达符合、字母，大小写变化等函数。

## 四、小结

本章主要介绍`using`命名空间，其结尾要加分号。

`string`对象的初始化（使用圆括号`(n, 'c')`），操作与遍历。其`size()`返回类型是`string::size_type`类型无符号整数，不要和有符号类型混合运算。可以用C风格字符串初始化`string`对象，反之不行。可以用`c_str()`将`string`对象转换为`C`风格字符串，但要注意若对象长度改变，原指针可能会无效。

`vector`序列的声明和初始化（若不明示，其内容会默认初始化），操作（其返回的长度是vecotr\<T\>::size_type， 无符号类型），不能通过下标直接初始化`vector`要通过`push_back`操作。

迭代器：与指针类似可以间接访问序列，其有具体类型`const_iterator`可以通过`auto`和`decltype`对其对象推测。也可以使用`cbegin()`和`cend()`。其差值是`类型::difference_type`有符号类型。

数组：同`auto`和`decltype`结合数组名推测，而且结果不同，`auto`会推测为指针、`decltype`推测为数组。