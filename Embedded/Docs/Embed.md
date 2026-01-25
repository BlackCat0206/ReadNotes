# Embed

---- Chime Lee

## 一、概述

**黄金法则：**

* 初始化：
  * 多路复用（**多个信号数据流**通过**一套物理链路传输**）确认信号和引脚的关系。和电器参数配置（电路选择 上拉电阻，下拉电阻）。
    * **IOMUTX Control**模块的：多路复用寄存器将引脚与信号连接在一起。
    * **IOMUTX Control**模块的：增强电路能力
  * 控制器参数配置：如果有从机，也要对从机进行参数配置。
    * 例如：方向寄存器，。
  * 时钟和使能：配置个模块工作使用，使能对应的开关寄存器。
    * CCM （Clock Controller Module）：为所有控制器提供时钟的模块
    * CCM_CCGR (CCM Clock Gating Register)：独立控制每个模块的时钟开关，
  * 中断的配置：中断源、中断触发的条件，中断处理函数。
    * ICR (Interrput Configuation Register)：配置中断模式
    * IMR (Interrput Mask Register)：配置中断屏蔽
    * ISR (Interrput Status Register)：读取对应位是否发生中断
    * EDGE_SEL（edge select register）：可以重写ICR，1 上升下降沿均触发。
* 读（接收）、写（发送）：正常流程。
* 处理中断：异常流程。

MCU：Micro Controler Unit 微控制单元

SOC：System on chip 

## 二、

### 2.1交叉编译（*cross complie*）

* 在一个平台上编译另一个平台的可执行代码

* 在X86下使用ARM编译指令编译其对应可执行程序。
* 重复发挥X86运行速度快的特性。

### 2.2 控制器

* 控制外部设备的电路。
* 软件要操作的寄存器。

### 2.3 寄存器（*register*）

* 软件操作硬件的唯一接口。

* CPU与专用控制器之间进行配置，命令下达和数据交换的唯一接口。

### 2.4 GPIO

General Purpose Input Output

### 2.5 中断有三个：

* 控制器
* GIC (Golbal Interrupt Controller)：对中断进行排序，屏蔽或者
* CPU的F位和I位：屏蔽或者使能中断处理。