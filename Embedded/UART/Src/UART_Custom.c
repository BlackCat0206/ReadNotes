/***
 * @file UART.c
 * @brief UART communication implementation.
 * @author Chime Lee
 * @version 1.0
 * @date 2026-01-02
 * @note:
 * 波特率计算公式：BaudRate = RefFreq / (16 * (UBMR + 1) / (UBIR + 1))
 * 其中，RefFreq为参考时钟频率，UBMR为波特率
 * 调制寄存器，UBIR为波特率增量寄存器。
 * 例如，若参考时钟频率为80MHz，目标波特率为115200，则可通过调整UBMR和UBIR的值来实现。
 * 设UBIR = 71，则UBMR计算如下：
 * UBMR = (RefFreq / (16 * BaudRate)) * (UBIR + 1) - 1
 *      = (80000000 / (16 * 115200)) * (71 + 1) - 1
 *      = 3124
 * 因此，设置UBIR为71，UBMR为3124即可实现115200的波特率。
 */

#include "UART_Custom.h"

// 多路复用"发送数据"引脚指针：20E_0084h
volatile int* IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA_PTR = (unsigned int*) 0x020E0084;
// 多路复用"接受数据"引脚多路复用二次选择指针：20E_0624h
volatile int* IOMUXC_UART1_RX_DATA_SELECT_INPUT_PTR = (unsigned int*) 0x020E0624;
// 多路复用"接受数据"引脚指针：20E_0088h
volatile int* IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA_PTR = (unsigned int*) 0x020E0088;

// 电器参数配置"发送数据"引脚指针：20E_0310h
volatile int* IOMUXC_SW_PAD_CTL_PAD_UART1_TX_DATA_PTR = (unsigned int*) 0x020E0310;
// 电器参数配置"接受数据"引脚指针：20E_0314h
volatile int* IOMUXC_SW_PAD_CTL_PAD_UART1_RX_DATA_PTR = (unsigned int*) 0x020E0314;

// 因使用控制寄存器较多，故定义结构体，方便后续操作
typedef struct{
    volatile unsigned int UART1_URXD;      // 接收数据寄存器 Base address + 0h offset
    unsigned char RESERVED0[60];           // 保留空间
    volatile unsigned int UART1_UTXD;      // 发送数据寄存器 Base address + 40h offset
    unsigned char RESERVED1[60];           // 保留空间
    volatile unsigned int UART1_UCR1;      // 控制寄存器1 Base address + 80h offset
    volatile unsigned int UART1_UCR2;      // 控制寄存器2 Base address + 84h offset
    volatile unsigned int UART1_UCR3;      // 控制寄存器3 Base address + 88h offset
    volatile unsigned int UART1_UCR4;      // 控制寄存器4 Base address + 8Ch offset
    volatile unsigned int UART1_UFCR;      // FIFO控制寄存器 Base address + 90h offset
    volatile unsigned int UART1_USR1;      // 状态寄存器1 Base address + 94h offset
    volatile unsigned int UART1_USR2;      // 状态寄存器2 Base address + 98h offset
    volatile unsigned int UART1_UESC;      // 错误状态寄存器 Base address + 9Ch offset
    volatile unsigned int UART1_UTIM;      // 超时寄存器 Base address + A0h offset
    volatile unsigned int UART1_UBIR;      // 波特率增量寄存器 Base address + A4h offset
    volatile unsigned int UART1_UBMR;      // 波特率调制寄存器 Base address + A8h offset
    volatile unsigned int UART1_UBRC;      // 波特率除数寄存器 Base address + ACh offset
    volatile unsigned int UART1_ONEMS;     // 1毫秒计数寄存器 Base address + B0h offset
    volatile unsigned int UART1_UTS;       // 测试寄存器 Base address + B4h offset
    volatile unsigned int UART1_UMCR;      // 调制控制寄存器 Base address + B8h offset
    // 其他寄存器省略
}uart_registers;

#define UART1_BASE_ADDR ((uart_registers*)0x02020000 ) // UART1基地址

void UART_Init(void) {
    // 初始化UART配置
    // 1.多路复用引脚配置
    *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA_PTR = 0x00000000; // 配置为UART1_TX
    *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA_PTR = 0x00000000; // 配置为UART1_RX
    *IOMUXC_UART1_RX_DATA_SELECT_INPUT_PTR   = 0x00000003;   // 选择为UART1_RX_DATA

    // 2.电器参数配置（默认）

    // 3.参数配置（波特率，帧格式）
    
    // 波特率：115200，假设时钟频率为80MHz
    UART1_BASE_ADDR->UART1_UFCR &= ~(7 << 7); // 设置置0 RFDIV为0
    UART1_BASE_ADDR->UART1_UFCR |= (5 << 7);  // 设置数据位为8位

    UART1_BASE_ADDR->UART1_UBIR = 71;
    UART1_BASE_ADDR->UART1_UBMR = 3124;         // 设置波特率为115200

    // 帧格式：
    UART1_BASE_ADDR->UART1_UCR2 &= ~(1);        // 设置复位开关0位 ：关闭。
    UART1_BASE_ADDR->UART1_UCR2 |= (1<<1)|(1<<2)|(1<<5)|(1<<14); // 设置发送使能，接受使能，Word长度8位，无奇偶校验，忽略RTX输入信号（DCE）

    // 当前芯片UART工作在多路复用模式下，UART_UCR3的第2位，必须始终置1。原文：In this chip, UARTs are used in MUXED mode, so that this bit should always be set
    UART1_BASE_ADDR->UART1_UCR3 |= (1 << 2);

    // 时钟和使能配置（根据Clock Tree，本次使用默认分频即可）
    // 时钟使用默认开启状态，无需额外配置
    // 使能UART模块
    UART1_BASE_ADDR->UART1_UCR1 |= (1 << 0); // 使能UART模块

    // 中断配置（本次不使用中断）
}

char* UART_SendData(const char* data, unsigned int length) {
    // 通过UART发送数据
    while (0 == (UART1_BASE_ADDR->UART1_USR2 & (1 << 3)))
    {
        // 等待发送缓冲区为空
    }
    
    for (unsigned int i = 0; i < length; i++) {
        UART1_BASE_ADDR->UART1_UTXD = data[i];
        // 等待发送完成
        while (0 == (UART1_BASE_ADDR->UART1_USR2 & (1 << 3)))
        {
            // 等待发送缓冲区为空
        }
    }
    
}

char* UART_ReceiveData(char* buffer, unsigned int length) {
    // 通过UART接收数据(通过状态寄存器USR2的第0位判断是否有数据接收)
    while (0 == (UART1_BASE_ADDR->UART1_USR2 & (1 << 0)))
    {
        // 等待数据接收
        ;
    }
    return UART1_BASE_ADDR->UART1_URXD&0xFF;
    
}


int main(int argc, char* argv[]) {
    UART_Init();
    const char* message = "Hello, UART!";
    UART_SendData(message, 12);

    // char buffer[50];
    // UART_ReceiveData(buffer, 50);

    return 0;
}