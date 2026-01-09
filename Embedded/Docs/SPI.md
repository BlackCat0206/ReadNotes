# SPI

---- Chime Lee

## 一、概述

SPI （Serial Peripheral Interface）串行外设接口协议

**信号：8个**

**MISO(Master Input Slaver Output)：**主机输入，从机输出

**MOSI(Master Output Slaver Input)：**主机输出，从机输入

**RDY(Ready)：**

**SCLK(Serial Clock)：**串行时钟总线

**CS(chip select) / SS（slaver select）0 ~ 3：**片选信号线（低电平片选），**可以用GPIO扩展片选**。

一般用四个：

* MISO，MOSI，SCLK，CS/SS

**通信类型：**

全双工同步串行通信总线

传输数据长度不限制。

**引入概念：** 重要

**SPI模式由下属概念，组合四种组合，决定时钟的空闲状态和在那个边沿采样。**

* **时钟极性（polarity）：SCLK空闲状态**
  * 0：时钟空闲为低电平
  * 1：时钟空闲为高电平

* **时钟向位（Phase）：数据在时钟的那个边沿被采样**
  * 0：数据在时钟的第一个边沿被采样。
  * 1：数据在时钟的第二个边沿被采样。
  * **决定数据输出（改变）时机。数据总在反方向采样的边沿准备好。**

| polarity | phrase | 采样边沿     |
| -------- | ------ | ------------ |
| 0        | 0      | 第一个上升沿 |
| 0        | 1      | 第一个下降沿 |
| 1        | 0      | 第一个下降沿 |
| 1        | 1      | 第一个上升沿 |

**寄存器：**

**ECSPIx_RXDATA**：接收数据寄存器 32位

**ECSPIx_TXDATA**：发送数据寄存器 32位，由控制寄存器的“BURST_LENGTH”决定有效长度。

**ECSPIx_CONREG**：控制寄存器

* [31:20] BURST_LEN GTH  R/W Burst Length，突发传输长度。SPI 移位寄存器中保存有要传输的数据，它是 32 位的。我们只想传输一个字节即 8 位时，怎么办？要传输 n 位数据时，可以设置此位域为(n-1)，0x000：传输 32 位里的 1 位(LSB，最低位)；0x001：传输 32 位里的 2 位(LSB)；

**ECSPIx_CONFIGER**：配置寄存器

* [23:20] SCLK_CTL  R/W 使用某个 SPI 通道时，非激活状态下 SCLK 的电平，由此位控制。
  * 0：低电平；
  * 1：高电平
* [19:16] DATA_CTL  使用某个 SPI 通道时，非激活状态下数据线的电平，由此位控制。
  * 0：低电平；
  * 1：高电平
* [15:12] SS_POL 使用某个 SPI 通道时，对应的片选信号的极性，由此位控制。
  * 0：低电平有效；
  * 1：高电平有效

**ECSPIx_STATREG**：状态寄存器（传输状态，发送/接收寄存器的状态）

**ECSPIx_TESTREG**：测试寄存器（回环测试）

## 二、实验内容

**实验传感器：ICM-20608-G**

6轴姿态传感器

支持400kHz快速模式I2C，8MHz的SPI

**涉及传感器：**

* Sample rate divider：设置采样频率

* Configuartion：工作模式

* Gyroscope configuration：配置加速传感器，量程，自测等

* Accelerometer configuration：配置加速传感器，量程，自测等

* Accelerometer configuration2：配置加速传感器，量程，自测等

* Lower power mode configuration：于设置 ICM-20608-G 是否处于低功耗模式以及唤醒频率

* Fifo enable：该寄存器用于设置数据存储 FIFO 是否进行使用，为 1 表示使用，为 0 表示禁止 FIFO
*  Power management1：该寄存器用于设置 ICM-20608-G 时钟来源、电源模式、以及是否清楚全部寄存器到默认状态。
* Power management2：该寄存器用于设置是否使能相关传感器、包括加速度和角速度传感器。
*  Who am i：该寄存器存储设备 ID，ICM-20608-G 的默认值为 **0xAF**。
*  Accelerometer measurements：该寄存器存储角速度的测量结果，一共6 个地址，每两个地址为一组 X/Y/Z 轴角速度的高低字节，具体内容如下图所示。
*  Temperature measurements：该寄存器用于存储温度测量结果，由高低两个字节构成。
*  Gyroscope measurement：该寄存器存储加速度的测量结果，一共 6 个地址，每两个地址为一组 X/Y/Z 轴加速度的高低字节，具体内容如下图所示。

**数据传输：**高位->低位，上升读取，下降沿传输。

* **先传输MSB位，最后传输LSB位**。
* **数据在SCLK上升沿被锁存**。
* **数据在SCLK下降沿被传输**。
* **最大支持8MHz的时钟频率**。
* **读写在16个或更多的时钟周期内完成**。
* **支持单字节或多个字节读写操作**。

**读写操作：**

* 发送寄存器地址与操作：MSB ：操作符（1读，0写）+ 发送寄存器地址 LSB
* 读：看MISO
* 写：看MOSI

## 三、实验步骤

### 3.1 初始化SPI

* 将SS0复用位GPIO，通过拉高拉低实现片选。
* SPI控制寄存器：设置传输数据burst size，主从模式，开始传输模式，使能。
* SPI配置寄存器：SCLK空闲状态（低电平），极性（0），相位（0），数据线空闲高电平。
* 时钟分频：控制寄存器分频15，最终频率4MHz

### 3.2 配置ICM20608G采样频率

寄存器地址：0x19

采样频率计算公式：AMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)。INTERNAL_SAMPLE_RATE = 1kHz。

配置值0





