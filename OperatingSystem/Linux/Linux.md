# Linux

---- Lee

## 一、概述



## 二、内容

### 2.1 常用命令：

#### 2.1.1 设置其他用户密码

```shell
sudo passwd root
```

#### 2.1.2 切换用户

* su：switch user

```shell
# 不完全切换 （家目录不变）
su user

# 完成切换 （家目录改变）
su -user
```

#### 2.1.3 命令使用说明

```shell
man ls
```

#### 2.1.4 命令详细说明

```shell
info ls
```

#### 2.1.5 上一条命令的返回值的状态码

```shell
echo $?
```

#### 2.1.6 查看历史指向的命令

```shel
history
```

#### 2.1.7 树状展示目录

```shell
#树状展示目录下两级内容
tree -L 2
```

#### 2.1.8 显示系统中运行的进程状态

```bash
ps
top
```

#### 2.1.9 检查文件系统的磁盘空间占用情况

```shell
df
lee@lee-virtual-machine:~$ df
文件系统          1K-块    已用    可用 已用% 挂载点
udev            1937008       0 1937008    0% /dev
tmpfs            396900    1804  395096    1% /run
/dev/sda5      19947120 9132980 9775548   49% /
tmpfs           1984484       0 1984484    0% /dev/shm
tmpfs              5120       4    5116    1% /run/lock
tmpfs           1984484       0 1984484    0% /sys/fs/cgroup
/dev/loop0          128     128       0  100% /snap/bare/5
/dev/loop3       224256  224256       0  100% /snap/gnome-3-34-1804/72
/dev/loop2        66688   66688       0  100% /snap/gtk-common-themes/1515
/dev/loop1        56832   56832       0  100% /snap/core18/2128
/dev/loop4        93952   93952       0  100% /snap/gtk-common-themes/1535
/dev/loop5        52224   52224       0  100% /snap/snap-store/547
/dev/loop6        33152   33152       0  100% /snap/snapd/12704
/dev/sda1        523248       4  523244    1% /boot/efi
tmpfs            396896      16  396880    1% /run/user/1000
/dev/sr0        2999936 2999936       0  100% /media/lee/Ubuntu 20.04.3 LTS amd64

```

#### 2.1.10 查看`CPU`逻辑个数

```bash
cat /proc/cpuinfo | grep "processor" | wc -l 
```

#### 2.1.11 网络配置

* ip：全能工具

```bash
#查看网卡信息（IP、MAC、状态）：
ip addr show  # 等价于 ip a，查看所有网卡的详细信息
ip addr show eth0  # 只查看 eth0 网卡

#配置 IP 地址（临时生效，重启失效）：
ip addr add 192.168.1.100/24 dev eth0  # 给 eth0 加 IP
ip addr del 192.168.1.100/24 dev eth0  # 删除 IP

#启停网卡：
ip link set eth0 up  # 启动网卡
ip link set eth0 down  # 关闭网卡

#查看路由表（替代 route -n）：
ip route show  # 等价于 ip r，查看内核路由表

#添加 / 删除静态路由：
ip route add 10.0.0.0/8 via 192.168.1.1 dev eth0  # 访问10.0.0.0网段走192.168.1.1网关
ip route del 10.0.0.0/8  # 删除路由
```

* route：管理路由

* netstat: 网络连接与端口状态查询工具

* ```bash
  netstat -tulnp  # 最常用组合参数
  ```

  * `-t`：只显示 TCP 连接；
  * `-u`：只显示 UDP 连接；
  * `-l`：只显示监听状态的端口；
  * `-n`：用 IP / 端口号显示，不解析域名 / 服务名；
  * `-p`：显示连接对应的进程 PID 和名称（需 root 权限）

#### 2.1.12 chomd

```bash
-rwxrwxrwx-
用户(u)，组权限(g)，其他用户权限(o)
    
#指定改变权限：全部加可执行权限
chmod a+x test
#用户加可执行权限，用户组加读权限，其他用户加写权限
chmod u+x,g+r,o+w test
```

#### 2.1.13 `less,more`

* Linux中查看文件内容时，`more`和`less`都支持用空格翻页。
* `more`：只能向前浏览
* `less`：只能向后浏览
  * /字符串：向前搜索
  * ?字符串：向后搜索


#### 2.1.14 设置浏览器缓存时间：

```bash
expires 30d;
```

#### 2.1.15 `shell`脚本加密

```bash
shc -v -f test.sh
#test.sh.x 加密后的二进制文件
#test.sh.x.c 生成二进制文件所依赖的文件
```



#### 

### 2.2 `VIM`命令

#### 2.2.1 字符替换：

```bash
# 1,$ 表示从第一行到文件行尾
# s表示替换
# /g表示在全局文件中进行替换，省略时仅对每一行第一个匹配串进行替换
:1,$s/old/new/g
```



### 2.3 Linux相关知识：

#### 2.3.1 链接

链接的本质是文件的“**快捷方式**”，基于文件系统的底层机制实现，核心分为“**硬链接**”和“**软链接（符号链接）**”。

一、Linux中，文件的**实际数据**和**文件名**是分开存储的。

1.inode（索引节点）：相当于“身份证”，存储文件的大小、权限、数据存储等信息，一个文件至少对应1各inode。

2.文件名：inode的“别名”，本身不存储数据，作用是方便用户查找文件。

链接的本质：**给一个inode或一个文件，创建新的文件名**。

**二、两种链接的核心区别：**

| 特性           | **硬链接（Hard Link）**                  | **软链接（Symbolic Link）**            |
| -------------- | ---------------------------------------- | -------------------------------------- |
| **本质**       | 给同一个 inode 新增一个文件名            | 新建一个独立文件，内容是目标文件的路径 |
| **inode 号**   | 和原文件**相同**                         | 和原文件**不同**                       |
| **跨文件系统** | 不支持（inode 是文件系统内唯一的）       | 支持（本质是路径引用）                 |
| **原文件删除** | 硬链接仍可访问数据（inode 引用计数 > 0） | 软链接失效（变成 “死链接”）            |
| **链接对象**   | 只能链接**普通文件**，不能链接目录       | 可以链接**文件或目录**                 |
| **命令创建**   | `ln 原文件 硬链接文件`                   | `ln -s 原文件 软链接文件`              |

**三、通俗举例：**

硬链接：给文件取“小名”

```bash
ln report.txt note.txt
```

- 现在 `report.txt` 和 `note.txt` 共用 **inode 1001**，相当于同一个人有两个名字。
- 删掉 `report.txt`，`note.txt` 依然能打开，因为数据还在（inode 引用计数从 2 变成 1）。
- 改 `note.txt` 的内容，`report.txt` 也会变，因为改的是同一个 inode 对应的数据。

软链接：贴标签

```bash
ln -s report.txt shortcut.txt
```

- `shortcut.txt` 是一个**新文件**（inode 号：2002），内容只有一行：`report.txt`。
- 打开 `shortcut.txt` 时，系统会根据路径找到 `report.txt` 并打开它。
- 删掉 `report.txt`，`shortcut.txt` 就变成 “死链接”（找不到目标），打开会报错 `No such file or directory`。
- 软链接可以跨文件夹，比如 `ln -s /home/user/report.txt /tmp/report-link`。

**四、小结：**

* 硬链接：给inode取一个文件名，与原文件共享inode，inode增加引用计数，源文件删除新文件可以正常访问。不能跨文件系统，只能链接文件，不能链接目录。
* 软链接：创建一个新的文件，其内容是目标文件的路径。有独立的inode，原文件删除不能访问，链接变为死链接。可以跨文件系统，可以链接文件或目录。

#### 2.3.2 OSI模型

OSI模型（*Open Systems Interconnection Model*, 开放式系统互联模型）由国际组织ISO制定一套**网络通信7层分层标准**。

| 层级 | 层级名称   | 核心功能                                              | 经典协议/设备            |
| ---- | ---------- | ----------------------------------------------------- | ------------------------ |
| 7    | 应用层     | 负责为应用程序**提供网络服务**                        | HTTP，HTTPS              |
| 6    | 表示层     | 负责对**数据进行格式转换、加密、解密、压缩**          | JPEG、ASCII、SSL         |
| 5    | 会话层     | 建立、管理和终止设备间的通信                          | RPC                      |
| 4    | 传输层     | 负责端到端的**数据传输**，**差错校验**。              | TCP、UDP                 |
| 3    | 网络层     | 负责**网络路径选择**、**逻辑地址（IP地址）分配**      | IP、ARP、路由器          |
| 2    | 数据链路层 | 负责**相邻节点的帧传输**、**物理地址（MAC地址）寻址** | 交换机、Wifi、Ethernet   |
| 1    | 物理层     | 负责**物理介质**上二进制数据的传输                    | 网线、光纤、网卡、集线器 |

#### 2.3.3 find

* 根据修改时间查找文件：-mtine -/+ num

```bash
# 查找修改时间在两天前文件
find /opt/log -type f -mtime +2

# 查找修改时间在两天内的文件
find /opt/log -type f -mtine -2
```

#### 2.3.4 awk

#### 2.3.5 sed

`sed`（Stream Editor）是 Linux/Unix 下的流式文本编辑器，核心用于**文本的查找、替换、删除、新增、提取**等操作，处理时逐行读取文本并按规则修改，是 Shell 脚本中处理文本的核心工具。以下是`sed`最常用的方法，附示例和核心说明：

##### 一、基础语法

```bash
sed [选项] '编辑命令' 文件名  # 处理文件
echo "文本" | sed [选项] '编辑命令'  # 处理管道输入
```

##### 核心选项：

| 选项      | 作用                                                         |            |
| --------- | ------------------------------------------------------------ | ---------- |
| `-n`      | 静默模式，仅输出被处理的行（默认输出所有行）                 |            |
| `-i`      | 直接修改原文件（⚠️ 谨慎使用，建议先备份），`-i.bak`会先备份为`文件名.bak`再修改 |            |
| `-e`      | 执行多个编辑命令（多规则）                                   |            |
| `-f`      | 从脚本文件读取编辑命令                                       |            |
| `-r`/`-E` | 启用扩展正则表达式（无需转义`()`/`                           | `/`+` 等） |

##### 编辑命令格式（核心）：

```plaintext
[地址范围] 操作命令 [参数]
```

- 地址范围：指定处理哪些行（如`5`= 第 5 行，`1,10`=1-10 行，`/pattern/`= 匹配正则的行）；
- 操作命令：`s`（替换）、`d`（删除）、`p`（打印）、`a`（追加）、`i`（插入）、`c`（替换整行）等。

##### 二、最常用场景（附示例）

##### 1. 文本替换（核心：`s`命令）

`sed`最核心的用法，语法：`s/匹配模式/替换内容/修饰符`

| 修饰符 | 作用                                   |
| ------ | -------------------------------------- |
| `g`    | 全局替换（默认仅替换每行第一个匹配项） |
| `i`    | 忽略大小写（GNU sed 特有）             |
| `p`    | 打印替换后的行（需配合`-n`）           |
| `数字` | 替换每行第 N 个匹配项                  |

##### 示例：

```bash
# 1. 替换每行第一个"old"为"new"（仅输出，不修改原文件）
sed 's/old/new/' test.txt

# 2. 全局替换（所有"old"→"new"）
sed 's/old/new/g' test.txt

# 3. 忽略大小写替换
sed 's/old/new/gi' test.txt

# 4. 替换每行第2个"old"为"new"
sed 's/old/new/2' test.txt

# 5. 直接修改原文件（加备份）
sed -i.bak 's/old/new/g' test.txt  # 原文件备份为test.txt.bak，原文件被修改

# 6. 正则替换（如替换所有数字为空）
sed 's/[0-9]//g' test.txt

# 7. 扩展正则（匹配"foo"或"bar"，需-E）
sed -E 's/foo|bar/xxx/g' test.txt

# 8. 保留匹配内容（用&引用匹配项，如给所有数字加括号）
echo "123 abc 456" | sed 's/[0-9]\+/(&)/g'  # 输出：(123) abc (456)
```

##### 2. 行删除（`d`命令）

删除指定行或匹配规则的行，**默认仅输出删除后的结果，加`-i`才修改原文件**。

```bash
# 1. 删除第5行
sed '5d' test.txt

# 2. 删除1-10行
sed '1,10d' test.txt

# 3. 删除最后一行
sed '$d' test.txt

# 4. 删除匹配"error"的行
sed '/error/d' test.txt

# 5. 删除不匹配"success"的行（!取反）
sed '/success/!d' test.txt

# 6. 删除空行
sed '/^$/d' test.txt

# 7. 删除以#开头的注释行（保留空行）
sed '/^#/d' test.txt
```

##### 3. 行打印（`p`命令，需配合`-n`）

仅输出指定行或匹配行，避免默认的全量输出。

```bash
# 1. 打印第3行
sed -n '3p' test.txt

# 2. 打印1-5行
sed -n '1,5p' test.txt

# 3. 打印匹配"warning"的行
sed -n '/warning/p' test.txt

# 4. 打印匹配行的行号（=输出行号，p输出内容）
sed -n '/error/=; /error/p' test.txt

# 5. 打印最后一行
sed -n '$p' test.txt
```

##### 4. 文本新增 / 插入（`a`/`i`/`c`命令）

- `a`：在指定行**后**追加文本；
- `i`：在指定行**前**插入文本；
- `c`：替换指定行为新文本。

```bash
# 1. 在第5行后追加"hello world"
sed '5a hello world' test.txt

# 2. 在匹配"error"的行前插入"======"
sed '/error/i ======' test.txt

# 3. 将第3行替换为"new line"
sed '3c new line' test.txt

# 4. 在最后一行后追加内容
sed '$a end of file' test.txt
```

##### 5. 提取指定列 / 字段（配合正则）

`sed`可提取文本中的特定字段，适合简单的字段分割（复杂场景推荐`awk`）。

```bash
# 示例文本：test.txt内容为"name:zhangsan,age:20,gender:male"
# 提取姓名（匹配name:后到,前的内容）
sed -n 's/.*name:\([^,]*\).*/\1/p' test.txt  # 输出：zhangsan

# 提取年龄
sed -n 's/.*age:\([0-9]\+\).*/\1/p' test.txt  # 输出：20
```

##### 6. 多命令组合（`-e`或分号`;`）

执行多个编辑规则，适合复杂文本处理。

```bash
# 示例：删除空行 + 全局替换old→new + 打印匹配success的行
sed -e '/^$/d' -e 's/old/new/g' -n '/success/p' test.txt

# 等价写法（用分号分隔）
sed -n '/^$/d; s/old/new/g; /success/p' test.txt
```

##### 三、高频实用技巧

1. ##### **批量替换文件内容（递归）**：

```bash
# 递归替换当前目录下所有txt文件中的"foo"为"bar"（加备份）
sed -i.bak 's/foo/bar/g' $(find . -name "*.txt")
```

1. ##### **注释指定行**：

```bash
# 给第10行加#注释
sed '10s/^/#/' test.txt

# 给匹配"debug"的行加注释
sed '/debug/s/^/#/' test.txt
```

1. ##### **取消注释**：

```bash
# 取消以#开头且包含"debug"的行的注释
sed '/^#.*debug/s/^#//' test.txt
```

1. ##### **处理换行符（合并行）**：

```bash
# 将所有行合并为一行（用空格分隔）
sed ':a;N;$!ba;s/\n/ /g' test.txt
```

##### 四、注意事项

1. **正则转义**：默认`sed`使用基础正则，`()`/`|`/`+`等需转义（`\(`/`\|`/`\+`），加`-E`可使用扩展正则，无需转义；
2. **`-i`参数**：不同系统（如 macOS）的`sed`中`-i`需加空参数（`sed -i '' 's/old/new/g' test.txt`），GNU sed（Linux）无需；
3. **特殊字符**：替换内容包含`/`时，可改用`#`/`@`等分隔符，如`sed 's#/usr/bin#/usr/local/bin#g' test.txt`；
4. **备份文件**：使用`-i.bak`修改原文件前，会自动生成备份，避免误操作导致数据丢失。

#### 2.3.6 `tar`压缩于解压

##### 一、先搞懂 `tar` 核心参数

解压常用核心参数（组合使用）：

| 参数 | 作用                                                         |
| ---- | ------------------------------------------------------------ |
| `-x` | 解压（extract），必选                                        |
| `-v` | 显示解压过程（verbose），可选（推荐加，能看到解压的文件）    |
| `-f` | 指定要解压的文件名（file），**必须放在参数最后**，紧跟文件名 |
| `-z` | 处理 `.gz` 格式（gzip 压缩）                                 |
| `-j` | 处理 `.bz2` 格式（bzip2 压缩）                               |
| `-J` | 处理 `.xz` 格式（xz 压缩）                                   |
| `-C` | 指定解压到的目标目录（大写 C）                               |

### 二、按压缩包格式分类解压

#### 1. 基础 `.tar` 包（仅归档，未压缩）

格式：`tar -xvf 文件名.tar`

```bash
# 解压到当前目录
tar -xvf test.tar

# 解压到指定目录（比如 /opt/test）
tar -xvf test.tar -C /opt/test
```

#### 2. `.tar.gz` / `.tgz` 包（gzip 压缩）

格式：`tar -zxvf 文件名.tar.gz`（`.tgz` 同理）

```bash
# 解压到当前目录
tar -zxvf test.tar.gz
tar -zxvf test.tgz  # .tgz 是 .tar.gz 的简写

# 解压到指定目录
tar -zxvf test.tar.gz -C /home/user/docs
```

#### 3. `.tar.bz2` / `.tbz2` 包（bzip2 压缩）

格式：`tar -jxvf 文件名.tar.bz2`（`.tbz2` 同理）

```bash
# 解压到当前目录
tar -jxvf test.tar.bz2
tar -jxvf test.tbz2  # .tbz2 是 .tar.bz2 的简写

# 静默解压（不显示过程，去掉 -v 即可）
tar -jxf test.tar.bz2
```

#### 4. `.tar.xz` 包（xz 压缩，近年更常用）

格式：`tar -Jxvf 文件名.tar.xz`（注意 `-J` 是大写）

```bash
# 解压到当前目录
tar -Jxvf test.tar.xz

# 解压到指定目录
tar -Jxvf test.tar.xz -C /tmp
```

### 三、进阶技巧

#### 1. 只解压压缩包中的某个文件 / 目录

格式：`tar -xvf 压缩包名 要解压的文件路径`

示例（从 `test.tar.gz` 中只解压 `docs/readme.txt`）：

```bash
tar -zxvf test.tar.gz docs/readme.txt
```

#### 2. 先查看压缩包内容，再决定解压

解压前先看里面有什么文件，避免解压错：

```bash
# 查看 .tar.gz 内容
tar -ztvf test.tar.gz

# 查看 .tar.bz2 内容
tar -jtvf test.tar.bz2

# 查看 .tar.xz 内容
tar -Jtvf test.tar.xz

# 查看普通 .tar 内容
tar -tvf test.tar
```

（参数 `-t` 表示 “列出内容”，`-v` 显示详情）

#### 3. 兼容模式：不用记 `-z/-j/-J`（推荐新手）

新版 `tar` 支持自动识别压缩格式，直接用 `-xvf` 即可，无需区分压缩类型：

```bash
# 万能解压（自动识别 .tar/.tar.gz/.tar.bz2/.tar.xz）
tar -xvf 任意压缩包名
```

### 四、常见问题

1. **解压报错：`gzip: stdin: not in gzip format`**

   原因：用了 `-z` 参数解压非 `.gz` 格式的包（比如 `.tar.bz2`）。

   解决：去掉 `-z`，改用对应参数（如 `-j`），或直接用 `tar -xvf` 自动识别。

2. **权限不足：`Permission denied`**

   解决：加 `sudo`（如 `sudo tar -xvf test.tar.gz`），或确认目标目录有写入权限。

3. **解压后文件乱码**

   原因：压缩包在 Windows 下创建，编码不兼容。

   解决：解压时指定编码（需安装 `iconv`），示例：

   ```bash
   tar -xvf test.tar.gz --to-command='iconv -f gbk -t utf-8 > $TAR_FILENAME'
   ```
