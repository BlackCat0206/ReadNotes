# GPT

---- Chime Lee

## 一、概述

GPT （*General Purpose Timer*）：通用目的计时器。

**功能：**

GPT有**32位递**增计数器。可以将**外部引脚上的事件**捕获到**GPT寄存器**中，这些事件可以被设置为**上升沿**或**下降沿**，甚至**双边沿触发**。当定时器达到**设定值**时，可以让定时器的**输出引脚**输出指定**电平**，或是**产生中断**。

**分频器：**

GPT有一个12位预分频器，它可以对多个时钟源的时钟进行分频。

**运行模式：**

低功耗（*lower power*）或调式（*debug*）模式下运行。

**产生中断：**

捕获（*capture*），比较（*compare*）和反转（*rollover*）事件中产生中断。

**计数模式：**

重新启动（*restart*）或自由运行（*free-run*）。

**使用时钟：**

ipg_clk：外设时钟

ipg_clk_32k：低频参考时钟

ipg_clk_highfreq：高频参考时钟

ipg_clk_s：访问寄存器时钟。

时钟计算：

66MHz = 66/us

**寄存器：**

GPT_CR(**GPT Control Register**)：GPT控制寄存器

* EN（0位）：使能
* ENMOD （1位）：GPT使能模式：
  * 0：GPT计数在其关闭时会保留
  * 1：GPT计数在其关闭时会重置
* DBGEN：（2位）：Debug模式
* WAITEN  (3位)：wait mode时，GPT是否使能
* DOZEEN（4位）：Doze mode时，GPT是否使能
* STOPEN（5位）：stop mode时，GPT是否使能
* CLKSRC（6~8位）：选择时钟源。
* FRR（9位）：计数模式选择 自由运行，重新启动。
* EN_24M（10）：使能24MHz时输入。
* SWR（15）：软件重置使能位，在重置时置1，完成后会自动清0
* IM1~IM2（16~19位）：捕获模式设置。
* OM1~OM3（20~28位）：控制输出比较通道运行模式。
  * 000 没输出
  * 001 输出引脚反转，高->低，低->高
  * 010：变低电平
  * 011：变高电平
  * 100：产生有效的地脉冲。
* FO1~FO3（29~31）：强制输出比较通道。

GPTx_RP (**GPT Prescaler Register**)：GPT 预分频寄存器

* PRESCALER（0~11）：选定的时钟信号 / PERSCALER + 1后，用于驱动器计数器。

* PRESCALER24M（12~15）：选定时钟信号源之前，24M时钟 / PRESCALLER + 1，若未被选择，这个位域不会生效。

GPTx_SR（**GPT Status Register**）：GPT状态寄存器

* OF1~OF3（0 ~ 2）：在那个通道发生了**输出比较事件**。
* IF1~IF2（3~4）：在那个通道发生了**输入捕获事件**。
* ROV （5）：反转是否发生。

GPTx_IR（**GPT Interrupt Register**）：GPT 中断寄存器

* OF1IE~OF3IE（0~2）：控制对应通道的**输出比较中断使能**。
* IF2IE~IF2IE(3~4)：控制**输入比较捕获中断使能**。
* ROVIE（5）：控制翻转中断使能。

GPTx_OCRn （**GPT Output Compare Register n**）：GPT 输出比较寄存器n

* 存储着输出比较通道n上何时**触发输出比较事件的值**。

* 重启模式 （restart mode）：任何写入都会重置GPT计数器。

* IP总线写入需要等待一个周期后才能访问CPT_OCR1，IP总线读操作可立刻执行。

GPTx_ICR1（**GPT Input Capture Register n**）：GPT 输入捕获寄存器n

* 只读寄存器。
* 存储着输入捕获通道n上最后一次**捕获事件**期间计数器中的数据。

GPTx_CNT (**GPT Counter Register**)：GPT 计数寄存器

* 只读寄存器。
* 只能在不影响GPT计数的进程中读。

## 二、设计思路

GPT_Init：

* 复位
* 等待复位完成
* 配置：GPT关闭后重置计数、等待模式使能、停止模式使能、时钟源选择外设时钟、计数模式在重新计数模式。
* 时钟：分频配置（RP）
* 配置输出比较计数值
* 中断使能