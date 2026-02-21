# IO 库

***---- Li Jinsheng***

## 一、概述

本篇为《C++ Primer》（第 5 版）第 8 章读书笔记，主要内容为 C++ 标准库中的 IO 库相关基本概述、特性与使用方式。IO 库提供了处理输入输出的统一接口，涵盖控制台 IO、文件 IO、字符串 IO 等场景，是 C++ 程序与外部交互的核心基础。

## 二、内容

IO 库的核心思想：**抽象与统一**。将不同的输入输出源（控制台、文件、字符串）抽象为统一的流（stream）接口，屏蔽底层实现差异，让开发者以一致的方式处理各类 IO 操作。

### 2.1 IO 流的基本概念与分类

IO 流是数据传输的抽象，按数据流向可分为输入流（istream）、输出流（ostream）；按处理对象可分为：

- 控制台 IO：`cin`（标准输入）、`cout`（标准输出）、`cerr`（标准错误，无缓冲）、`clog`（标准日志，有缓冲）；
- 文件 IO：`ifstream`（文件输入）、`ofstream`（文件输出）、`fstream`（文件输入输出）；
- 字符串 IO：`istringstream`（字符串输入）、`ostringstream`（字符串输出）、`stringstream`（字符串输入输出）。

所有 IO 流类均继承自基类`ios_base`，核心特性（如状态、格式控制）在基类中定义，派生类实现具体的 IO 逻辑。

### 2.2 IO 状态（流的条件状态）

每个 IO 流对象都维护一个**条件状态**（`iostate`），用于标识流的运行状态，核心状态标志：

- `goodbit`：流状态正常，无错误；
- `badbit`：系统级错误（如读写失败），流不可再用；
- `failbit`：可恢复错误（如输入类型不匹配），流可重置后复用；
- `eofbit`：到达输入流末尾（end-of-file）。

#### 2.2.1 状态查询与修改

```cpp
#include <iostream>
using namespace std;

int main() {
    int ival;
    cin >> ival;
    // 查询状态
    if (cin.good()) { // 流状态正常
        cout << "输入成功：" << ival << endl;
    } else if (cin.fail()) { // 输入类型不匹配
        cout << "输入类型错误" << endl;
        cin.clear(); // 重置所有错误状态
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略缓冲区错误数据
    } else if (cin.bad()) { // 严重错误
        cout << "IO系统错误" << endl;
    } else if (cin.eof()) { // 到达输入末尾
        cout << "输入流结束" << endl;
    }
    return 0;
}
```

- `clear()`：重置流的状态标志，可指定重置后的状态（如`cin.clear(ios::failbit)`）；
- `setstate()`：设置指定的错误状态（不清除原有状态）；
- `rdstate()`：返回当前的状态标志（`iostate`类型）。

#### 2.2.2 流状态的使用原则

- 执行 IO 操作前，应检查流状态是否正常；
- 错误发生后，需重置状态并清理缓冲区，否则后续 IO 操作会直接失败；
- 条件判断（如`while (cin >> ival)`）会隐式检查`failbit`和`badbit`，若为真则循环终止。

### 2.3 文件 IO

文件 IO 通过`fstream`系列类实现，核心操作包括文件打开、读写、关闭。

#### 2.3.1 文件打开与关闭

```cpp
#include <fstream>
#include <string>
using namespace std;

int main() {
    // 1. 构造函数打开文件（常用方式）
    ofstream outfile("data.txt"); // 默认以输出模式打开，文件不存在则创建，存在则截断
    if (!outfile) { // 检查文件是否成功打开
        cerr << "文件打开失败" << endl;
        return -1;
    }
    outfile << "Hello, File IO!" << endl;
    outfile.close(); // 显式关闭文件，析构函数也会自动关闭

    // 2. 先创建对象，后用open()打开
    ifstream infile;
    infile.open("data.txt");
    if (infile) {
        string line;
        getline(infile, line); // 读取一行
        cout << "读取文件内容：" << line << endl;
        infile.close();
    }

    // 3. 指定打开模式
    fstream ioFile("data.txt", ios::in | ios::out | ios::app); // 输入+输出+追加模式
    if (ioFile) {
        ioFile << "追加内容" << endl;
        ioFile.close();
    }
    return 0;
}
```

#### 2.3.2 文件打开模式

| 模式标志      | 含义                         | 适用流类型           |
| :------------ | :--------------------------- | :------------------- |
| `ios::in`     | 以读模式打开                 | `ifstream`/`fstream` |
| `ios::out`    | 以写模式打开                 | `ofstream`/`fstream` |
| `ios::app`    | 追加模式，写入数据到文件末尾 | `ofstream`/`fstream` |
| `ios::trunc`  | 截断文件（清空原有内容）     | `ofstream`/`fstream` |
| `ios::binary` | 二进制模式（默认是文本模式） | 所有文件流           |
| `ios::ate`    | 打开后定位到文件末尾         | 所有文件流           |

- 注意：`ofstream`默认使用`ios::out | ios::trunc`，即打开文件时清空原有内容；若需保留内容，需显式指定`ios::app`。

#### 2.3.3 文件流的特有操作

- `open(const string& filename, ios_base::openmode mode = ios_base::out)`：打开指定文件；
- `close()`：关闭文件，释放文件资源；
- `is_open()`：检查文件是否成功打开且未关闭。

### 2.4 字符串 IO（内存流）

字符串 IO 通过`sstream`系列类实现，将字符串作为 IO 流的操作对象，适用于内存中数据的格式化处理。

```cpp
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

int main() {
    // 1. ostringstream：写入字符串
    ostringstream oss;
    oss << "姓名：" << "张三" << "，年龄：" << 20 << "，成绩：" << 95.5 << endl;
    string result = oss.str(); // 获取拼接后的字符串
    cout << "ostringstream结果：" << result;

    // 2. istringstream：读取字符串（按格式解析）
    string data = "100 200.5 hello";
    istringstream iss(data);
    int ival;
    double dval;
    string sval;
    iss >> ival >> dval >> sval; // 按空格分割解析
    cout << "解析结果：" << ival << " " << dval << " " << sval << endl;

    // 3. stringstream：读写字符串
    stringstream ss;
    ss << "初始值：" << 123;
    string temp = ss.str();
    cout << temp << endl;
    ss.str(""); // 清空字符串缓冲区
    ss << "新值：" << 456;
    cout << ss.str() << endl;
    return 0;
}
```

#### 2.4.1 字符串流的特有操作

- `str()`：返回当前流中的字符串副本；
- `str(const string& s)`：将字符串`s`设置为流的缓冲区内容，覆盖原有数据；
- 字符串流无 “打开 / 关闭” 操作，生命周期与对象绑定，析构时自动释放内存。

#### 2.4.2 应用场景

- 数据格式化拼接（替代繁琐的字符串拼接操作）；
- 解析带格式的字符串（如日志、网络数据）；
- 临时存储 IO 操作的中间结果，避免频繁的文件 / 控制台 IO。

### 2.5 IO 格式化

IO 库支持对输入输出的格式进行精细控制，包括数值进制、浮点数精度、对齐方式、填充字符等。

#### 2.5.1 格式控制符（无参操纵符）

```cpp
#include <iostream>
#include <iomanip> // 包含setprecision、setw等带参操纵符
using namespace std;

int main() {
    // 1. 进制控制
    int num = 10;
    cout << "十进制：" << dec << num << endl; // 十进制（默认）
    cout << "八进制：" << oct << num << endl; // 八进制
    cout << "十六进制：" << hex << num << endl; // 十六进制
    cout << "十六进制（大写）：" << uppercase << hex << num << endl;

    // 2. 布尔值格式
    bool flag = true;
    cout << boolalpha << "布尔值：" << flag << endl; // 输出true/false
    cout << noboolalpha << "布尔值：" << flag << endl; // 输出1/0

    // 3. 浮点数精度
    double pi = 3.1415926;
    cout << "默认精度（6位）：" << pi << endl;
    cout << "精度设为8位：" << setprecision(8) << pi << endl;
    cout << "固定小数位（2位）：" << fixed << setprecision(2) << pi << endl;

    // 4. 宽度与填充
    cout << setw(10) << "宽度10：" << endl; // 只作用于下一个输出
    cout << setfill('*') << setw(10) << 123 << endl; // 填充字符+宽度
    return 0;
}
```

#### 2.5.2 常用格式操纵符

| 操纵符                    | 作用                             | 备注                              |
| :------------------------ | :------------------------------- | :-------------------------------- |
| `dec`/`oct`/`hex`         | 设置整数进制为十 / 八 / 十六进制 | 作用于后续所有整数输出            |
| `boolalpha`/`noboolalpha` | 布尔值输出为 true/false/ 1/0     | 全局生效                          |
| `fixed`/`scientific`      | 浮点数固定小数位 / 科学计数法    | 全局生效                          |
| `setprecision(n)`         | 设置浮点数精度（n 位）           | 需包含`<iomanip>`，全局生效       |
| `setw(n)`                 | 设置下一个输出的宽度为 n         | 仅作用于下一个输出，需`<iomanip>` |
| `setfill(c)`              | 设置填充字符为 c                 | 全局生效，需`<iomanip>`           |
| `left`/`right`            | 输出内容左对齐 / 右对齐          | 全局生效                          |

#### 2.5.3 格式状态的保存与恢复

```cpp
#include <iostream>
#include <iomanip>
#include <ios> // 包含fmtflags
using namespace std;

int main() {
    // 保存当前格式状态
    ios::fmtflags old_flag = cout.flags();

    cout << hex << uppercase << 100 << endl; // 输出64（十六进制大写）

    // 恢复原有格式状态
    cout.flags(old_flag);
    cout << 100 << endl; // 恢复默认，输出100
    return 0;
}
```

### 2.6 未格式化的 IO 操作

除了`>>`、`<<`、`getline`等格式化操作，IO 库还提供直接操作字符 / 字节的未格式化 IO，适用于底层数据处理。

#### 2.6.1 单字符操作

```cpp
#include <iostream>
using namespace std;

int main() {
    char c;
    // 1. cin.get(c)：读取一个字符（包括空白符），成功返回cin
    while (cin.get(c)) { 
        cout.put(c); // cout.put(c)：输出一个字符
    }

    // 2. cin.get()：返回读取的字符（int类型，EOF为-1）
    int ch;
    while ((ch = cin.get()) != EOF) {
        cout.put(ch);
    }

    // 3. 忽略字符：cin.ignore(n, delim)，忽略n个字符或直到遇到delim
    cin.ignore(10, '\n'); // 忽略前10个字符或直到换行符
    return 0;
}
```

#### 2.6.2 多字符操作

```cpp
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char buf[1024];
    // 1. 读取指定长度的字符到缓冲区
    cin.get(buf, 1024); // 读取最多1023个字符（留1位存'\0'），遇到换行符停止
    cout << "读取的内容：" << buf << endl;

    // 2. 读取并丢弃分隔符（解决get后换行符残留问题）
    cin.get(buf, 1024);
    cin.get(); // 读取换行符
    cin.get(buf, 1024);
    cout << "第二次读取：" << buf << endl;

    // 3. read/write：读写字节（无格式化，不处理'\0'）
    const char* data = "未格式化字节数据";
    cout.write(data, strlen(data)); // 输出指定长度的字节
    return 0;
}
```

### 2.7 流的绑定与同步

- `tie()`：将输出流绑定到输入流，保证输入前刷新输出缓冲区（如`cin`默认绑定`cout`）；
- `sync_with_stdio()`：控制 C++ 流与 C 标准 IO（`stdio.h`）的同步，关闭同步可提升 IO 效率。

```cpp
#include <iostream>
using namespace std;

int main() {
    // 关闭cin与cout的绑定
    cin.tie(nullptr);
    // 关闭C++流与C标准IO的同步（提升效率）
    ios::sync_with_stdio(false);

    // 大量IO操作时效率更高
    for (int i = 0; i < 100000; ++i) {
        cout << i << endl;
    }
    return 0;
}
```

## 三、小结

C++ IO 库通过抽象的流接口统一了不同场景的输入输出操作，核心包括：

1. 流的条件状态是 IO 操作的核心，需时刻检查并处理错误；
2. 文件 IO 支持控制台之外的持久化数据读写，需关注打开模式和文件状态；
3. 字符串 IO 适用于内存中数据的格式化解析与拼接，是高效的内存数据处理方式；
4. 格式化操纵符可精细控制输出样式，未格式化操作满足底层字节处理需求；
5. 合理使用流的绑定、同步等特性，可平衡 IO 的易用性与效率。

掌握 IO 库的核心特性，能让程序灵活处理各类输入输出场景，同时保证代码的健壮性和效率。