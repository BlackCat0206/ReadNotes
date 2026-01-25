# CMake

---- Chime Lee

## 一、概述

记录常用`CMake`命令。

## 二、内容

### 2.1 指定CMake最低版本

```cmake
cmake_minimum_required(VERSION 3.10)
```

### 2.2 指定项目名称和版本

```cmake
project(xxx VERSION 1.0 LANGUAGES CXX)
```

### 2.3 指定语言标准

```cmake
set(CMKAE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

### 2.4 头文件连接

```cmake
target_include_directories(目标名
    PRIVATE   # 仅当前目标的源文件可见（内部头文件）
    PUBLIC    # 当前目标 + 依赖该目标的其他目标可见（接口头文件）
    INTERFACE # 仅依赖该目标的其他目标可见（纯接口）
    头文件目录路径
)

#示例：
target_include_directories(math_utils
    PRIVATE ${CMAKE_SOURCE_DIR}/src/math/private_include  # 私有头文件
    PUBLIC  ${CMAKE_SOURCE_DIR}/src/math/public_include   # 接口头文件
)
```

### 2.5 编译可执行文件

```cmake
add_executable(xxx str/xxx.cpp)
```

### 2.6 生成 `compile_commands.json`

```cmake
# 生成 compile_commands.json（也可通过 VSCode 配置，这里是冗余保障）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

### 2.7 链接依赖库

```cmake
# 链接 pthread 库
target_link_libraries(thread_app pthread)
```

### 2.8 设置编译参数

```cmake
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
```

