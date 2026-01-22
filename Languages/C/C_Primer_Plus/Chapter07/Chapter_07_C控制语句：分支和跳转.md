# Chapter_07_C控制语句：分支和跳转

***---- Li Jinsheng***

***

## 一、概述

本章为《C Primer Plus》（第6版）第7章读书笔记

## 二、内容

### 2.1 `if`语句

* `if`语句格式：

```c
if (expression)
{
    statement;
}
```

* `if`语句被称为：选择语句（*selsection statement*）或分支语句（*branching statement*）。

#### 2.1.1 `getchar()`和`putchar()`函数

* 定义：`stdio.h`头文件中，按书中描述是一个预处理宏而不是一个函数。（TODO：后续确认补充）
* `getchar()`：获取下一个输入的字符并返回。（在示例中，可以看出。类似于`scanf()`会将输入缓存，等调用时继续读取。）
* `putchar()`：打印输出传入的字符，可以传入整数（第5章所说的自动类型转换，`int`类型会被截断），但可能会有截断风险。

使用示例：

```c
// cypher1.c -- alters input, preserving spaces 
#include <stdio.h>
#define SPACE ' '             // that's quote-space-quote 
int main(void)
{
    char ch;
    
    ch = getchar();           // read a character         
    while (ch != '\n')        // while not end of line    
    {
        if (ch == SPACE)      // leave the space          
            putchar(ch);      // character unchanged      
        else
            putchar(ch + 1);  // change other characters  计算时自动类型转换，传参时自动类型转换
        ch = getchar();       // get next character       
    }
    putchar(ch);              // print the newline        
    
    return 0;
}

/*
Result:
CALL ME HAL
DBMM NF IBM
*/
```

### 2.1.2 `ctype.h`系列字符函数

* `ctype.h`中映射的测试函数

| 函数名     | 如果是下列参数，返回为真                                     |
| ---------- | ------------------------------------------------------------ |
| isalnum()  | 字母或数字                                                   |
| isalpha()  | 字母                                                         |
| isblank()  | 标准空白符（水平制表、空格和换行符）                         |
| iscntrl()  | 控制字符，Ctrl + B                                           |
| isdigit()  | 数字                                                         |
| isgraph()  | 除空格外的可打印字符                                         |
| islower()  | 小写字母                                                     |
| isprint()  | 可打印字符                                                   |
| ispunct()  | 标点符号                                                     |
| isspace()  | 空白字符（水平制表、垂直制表、空格、换行、换页、回车或其他本地化定义字符） |
| isupper()  | 大写字母                                                     |
| isxdigit() | 十六进制字符                                                 |

* `ctype.h`中映射的函数

| 函数名      | 行为                                                   |
| ----------- | ------------------------------------------------------ |
| `tolower()` | 如果参数是大写字母，该函数返回小写字母。否则返回原值。 |
| `toupper()` | 如果参数是小写字母，该函数返回大写字母。否则返回原值。 |

* 上述内容就目前经验来看，可以适用于用户名和密码校验时对输入字符的规则限制。

### 2.1.3 多重选择`if-else`

* 格式：

```c
// 规范写法
if ( expression )
{
    statement;
}
else
{
    statement;
}


// 书中其他写法：(了解就好，实际开发中严格按照规范执行)
if ( expression )
    if (expression)
        statement;
else
    statement;
```

* 匹配逻辑：若无花括号（`{}`）`else`会匹配最近的`if`。若有花括号，`else`会匹配内嵌if语句的第一个语句。

```c
/* 无花括号 */
if ( expression )
    statement;

if (expression)
    statement;
else
    statement;

/* 有花括号  */
if ( expression )
{
    statement;
	if (expression)
        ... ...
}
else
 	statement;   
```

### 2.2 逻辑运算符

| 逻辑运算符 | 含义 |
| ---------- | ---- |
| `&&`       | 与   |
| `||`       | 或   |
| `!`        | 非   |

* 拓展：并非所有的键盘是美式键盘，故有头文件`iso646.h`。其可以使用`and`、`or`、`not`替换逻辑运算符。

* 优先级：

| 优先级 | 运算符                                                       | 结合律   |
| ------ | ------------------------------------------------------------ | -------- |
| 1      | （）、[]、.、->                                              | 从左向右 |
| 2      | -、+（一元）、--、++、`sizeof`、&（取地址）、！、*（解引用）、（类型） | 从右向左 |
| 3      | *、/、%                                                      | 从左向右 |
| 4      | - + （二元）                                                 | 从左向右 |
| 5      | << 左移、>> 右移                                             | 从左向右 |
| 6      | <、>、 >=、 <=                                               | 从左向右 |
| 7      | == !=                                                        | 从左向右 |
| 8      | & （按位与）                                                 | 从左向右 |
| 9      | ^ （按位异或）                                               | 从左向右 |
| 10     | \|（按位或）                                                 | 从左向右 |
| 11     | &&                                                           | 从左向右 |
| 12     | \|\|                                                         | 从左向右 |
| 13     | ?:                                                           | 从右向左 |
| 14     | =、/=、*=、%=、+=、-=、<<=、>>=、&=、^=、\|=                 | 从右向左 |
| 15     | ，                                                           | 从左向右 |

**简单记**：！> 算数运算符 > 关系运算符 > 逻辑运算符 > 赋值运算符 > ,(逗号)。

### 2.3 条件运算符：？：

条件运算符`?:`：也被称为三元表达式。其格式如下：

```c
expression1 ? expression2 : expression3;	// 若expression1为真，则整个表达式的结果为expression2; 若为假，则整个表达式的结果为expression3。
```

### 2.4 循环辅助：`continue`和`break`

#### 2.4.1 `continue`

* `continue`语句：在执行后表示跳过当前循环的剩余部分，进入下一次循环。若使用在嵌套内侧，则只影响内侧循环。

* 对不同的循环逻辑，`continue`在执行后的行为有所不同：
  * `while`和`do...while`：`continue`执行后的下一个行为是对循环测试表达式求值。
  * `for`：`continue`执行后的下一个行为：1.对更新表达式求值。2.对测试表达式求值。

#### 2.4.2 `break`

* `break`语句：执行后表示跳出当前循环。

### 2.5 多重选择：`switch`和`break`

* `swich-case`语句：可以表示对同一类型的不同状态进行不同的处理（工作中对于功能的状态，下方配置的不同操作）
* 格式：`switch`后圆括号中填充求值表达式（必须是整数类型`int`、`char`和`enum`），程序**扫描标签**，若条件满足执行其中语句。若都不满足，则执行`default`中语句。

```c
switch (expression1)
{
    case label1:
    {
     	statement;
        break;
    }
    case label2:
    {
        statement;
        break;
    }
    // ... ...
    default:
    {
        statement;
        break;        
    }
}
```

* 注：日常开发中将`switch-case`都加上花括号（{}），增强代码可读性！，`default`后建议也加上`break`。
* `break`的作用：跳出`switch`循环，否则`switch`语句会一直执行下去。

### 2.6 `goto`语句

* 背景：早期`BASIC`语言对此依赖，K&R C标准中提到`goto`容易被滥用，建议“**谨慎使用**”。
* 格式：`goto`和标签名（与变量命名规则相同）。

```c
goto lable:

lable :
	statement;
```

* 注：当前C语言的各种循环、`continue`和`break`可以完成绝大部分开发的使用。
* 个人经验：
  * 在工作中为保证对数据的内存掌控，多数变量是申请的堆内存。
  * 多数情况下，开发不能只考虑功能执行顺利的情况，要多考虑异常处理（例：所需功能为处理一个正整数，但实际传入功能函数的实参是一个负数），这时就需要对申请的堆内存进行释放。
  * 在一个功能模块中，这种异常处理是很多的。但每个异常处理都编写内存释放语句，代码会显的臃肿（一个小功能可能会需要很多个申请了堆内存的变量），对所有变量都编写对应的内存释放函数会增大工作量与后期维护成本。
  * 故可以将内存释放这样的重复操作放在函数末尾，前加一个`lable`，在异常处理时使用`goto`语句直接跳转（一定要保证释放的指针不为空，`double free`查起来很痛苦！！！，若无法保证，还是老老实实每次异常处理逐个释放，程序稳定性高于一切！）。
  * 使用时加注释说明 ！！！
  * 在使用该语句后，建议和组内开发前辈或同事进行代码评审，多聆听他人的经验与建议。

## 三、小结

本章主要介绍了选择语句·`if-else`与`switch-case`与跳转语句`continue`、`break`和`goto`。`getchar()`与`putchar()`函数，逻辑运算符

* `ctype.h`头文件中声明的函数可以用于开发过程中用户名和密码格式的校验与转换。

* `continue`语句在不同循环中执行效果有所不同，在开发时要多注意。
* `goto`语句在开发中要谨慎使用。
* **运算符优先级简单记**：! > 算数运算符 > 关系运算符 > 逻辑运算符 > 赋值运算符 > ,

* 逻辑运算符与`iso646.h`的非美式键盘的适配。
* **`switch`的表达式限制**：`switch`后的表达式结果必须是整数类型（`int`、`char`、枚举等），不能是浮点型或字符串，这一点可以补充，避免使用时踩坑。
* **逻辑运算符的短路特性**：`&&`左侧为假时右侧不执行，`||`左侧为真时右侧不执行，这一特性在实际代码中很常用（比如`if (p != NULL && p->value > 0)`避免空指针访问），可以简单提及。