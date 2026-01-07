/**
 * @file I2C_Custom.c
 * @brief I2C通信实验，自定义I2C接口实现。用于AP3216C传感器的I2C通信。
 * @author Chime Lee
 * @date 2026-01-06
 * @version 1.0
 * @note:
 * 1.引脚多路复用时要注意引脚的强制输入配置，否则可能无法正常工作（无法接收应答信号， 无法进行总线仲裁等功能）
 * 2.引脚的电器参数配置要符合I2C的要求，开漏输出，上拉电阻等
 */
#include "I2C_Custom.h"
#include "my_printf.h"
#include "type.h"

static volatile unsigned int* IOMUXC_SW_MUX_CTL_PAD_UART4_TX_DATA;      // UART4_TX多路复用寄存器
static volatile unsigned int* IOMUXC_SW_PAD_CTL_PAD_UART4_TX_DATA;      // UART4_TX引脚控制寄存器
static volatile unsigned int* IOMUXC_SW_MUX_CTL_PAD_UART4_RX_DATA;      // UART4_RX多路复用寄存器
static volatile unsigned int* IOMUXC_SW_PAD_CTL_PAD_UART4_RX_DATA;      // UART4_RX引脚控制寄存器

static volatile unsigned int* IOMUXC_I2C1_SCL_SELECT_INPUT;             // I2C1_SCL输入选择寄存器
static volatile unsigned int* IOMUXC_I2C1_SDA_SELECT_INPUT;             // I2C1_SDA输入选择寄存器


/**
 * @brief I2C 初始化函数
 * @param [in] void
 * @return void
 * @note:
 * 配置时钟分频，使能I2C模块
 */
void I2C_Init(void)
{

    // 一、初始化
    IOMUXC_SW_MUX_CTL_PAD_UART4_TX_DATA = (volatile unsigned int*)0x020E00B4u; // UART4_TX多路复用寄存器地址
    IOMUXC_SW_MUX_CTL_PAD_UART4_RX_DATA = (volatile unsigned int*)0x020E00B8u; // UART4_RX多路复用寄存器地址
    IOMUXC_SW_PAD_CTL_PAD_UART4_TX_DATA = (volatile unsigned int*)0x020E0340u; // UART4_TX引脚控制寄存器地址
    IOMUXC_SW_PAD_CTL_PAD_UART4_RX_DATA = (volatile unsigned int*)0x020E0344u; // UART4_RX引脚控制寄存器地址
    IOMUXC_I2C1_SCL_SELECT_INPUT = (volatile unsigned int*)0x020E05A4u;        // I2C1_SCL输入选择寄存器地址
    IOMUXC_I2C1_SDA_SELECT_INPUT = (volatile unsigned int*)0x020E05A8u;        // I2C1_SDA输入选择寄存器地址
    // 1.1 多路复用和电器参数配置
    *IOMUXC_SW_MUX_CTL_PAD_UART4_TX_DATA = 0x12u;   // 配置UART4_TX为I2C1_SCL功能
    *IOMUXC_SW_MUX_CTL_PAD_UART4_RX_DATA = 0x12u;   // 配置UART4_RX为I2C1_SDA功能
    *IOMUXC_SW_PAD_CTL_PAD_UART4_TX_DATA = 0x70B0u; // 配置I2C1_SCL引脚属性
    *IOMUXC_SW_PAD_CTL_PAD_UART4_RX_DATA = 0x70B0u; // 配置I2C1_SDA引脚属性
    *IOMUXC_I2C1_SCL_SELECT_INPUT        = 0x1u;    // 配置I2C1_SCL输入选择
    *IOMUXC_I2C1_SDA_SELECT_INPUT        = 0x2u;    // 配置I2C1_SDA输入选择
    // 1.2 配置I2C1寄存器
    I2C1->I2Cx_I2CR &= ~(1 << 7);  // 关闭I2C1模块
    // 配置时钟:I2C的时钟源来源于IPG_CLK_ROOT=49.5Mhz
    // 配置I2C的时钟为100K选择分频时钟：49500000 / 100000 = 495
    I2C1->I2Cx_IFDR = 0x37u;       // 设置分频寄存器
    
    I2C1->I2Cx_I2CR |= (1 << 7);   // 打开I2C1模块

    printf("I2C1 Addr: 0x%08x\n", (unsigned int)I2C1);
     // 检查I2C模块是否使能
    printf("I2C1 initialized.\n");

    return;
}

/**
 * @brief I2C状态检测函数（是否发生仲裁失败或未应答）
 * @param [in] I2C_Register* I2Cx: 指向I2C寄存器结构体的指针
 * @param [in] uint16_t status: I2C状态寄存器值
 * @return uint8_t: 返回状态，0表示成功，非0表示失败 
 */
uint8_t I2C_Check(I2C_Register* I2C_BASE, uint16_t status)
{
    // 判断是否发生仲裁失败
    if (1 == ((status >> 4) & (1 << 0)))
    {
        printf("I2C arbitration lost.\n");
        I2C_BASE->I2Cx_I2SR &= ~(1 << 4); // 清除仲裁失败标志位
        I2C_BASE->I2Cx_I2CR &= ~(1 << 7); // 复位I2C模块，先关闭
        I2C_BASE->I2Cx_I2CR |= (1 << 7);  // 再打开
        return I2C_ARBITRATION_LOST;
    }
    else if (status & (1 << 0)) // 判断是否未应答
    {
        printf("Stauts is :%d\n", status);
        printf("I2C NAK received.\n");
        return I2C_ADDRNAK; // 返回未应答状态
    }
    return I2C_OK;
}

/**
 * @brief I2C 发送起始信号函数
 * @param [in] I2C_Register* I2Cx: 指向I2C寄存器结构体的指针
 * @param [in] uint8_t slaveAddr: 从机地址
 * @param [in] uint32_t opcode: 操作码（读/写）
 * @return uint8_t: 返回状态，0表示成功，非0表示失败
 * @details:
 * 1.检查I2C总线是否忙
 * 2.设置控制寄存器，进入主模式并设置为发送模式
 * 3.设置数据寄存器，发送从机地址和读写位
 */
uint8_t I2C_Start(I2C_Register* I2C_BASE, uint8_t slaveAddr, uint32_t opcode)
{
    // 判断I2C总线是否忙
    if (1 == ((I2C_BASE->I2Cx_I2SR >> 5) & (1 << 0)))
    {
        printf("I2C bus busy!\n");
        return I2C_BUS_BUSY; // 总线忙，返回错误
    }

    // 设置控制寄存器，进入主模式并设置为发送模式
    I2C_BASE->I2Cx_I2CR |= (1 << 5) | (1 << 4);

    // 设置数据寄存器，发送从机地址和读写位
    I2C_BASE->I2Cx_I2DR = ((uint32_t)slaveAddr << 1) | ((I2C_READ == opcode) ? 1 : 0);
    return 0;
}

/**
 * @brief I2C 发送restart信号函数
 * @param [in] I2C_Register* I2Cx: 指向I2C寄存器结构体的指针
 * @param [in] uint8_t slaveAddr: 从机地址
 * @param [in] uint32_t opcode: 操作码（读/写）
 * @return uint8_t: 返回状态，0表示成功，非0表示失败
 * @details:
 * 1.检查I2C总线是否忙
 * 2.设置控制寄存器，发送重复开始信号并设置为发送模式
 * 3.设置数据寄存器，发送从机地址和读写位
 */
uint8_t I2C_Restart(I2C_Register* I2C_BASE, uint8_t slaveAddr, uint32_t opcode)
{
    // 若总线忙且非主模式，返回错误
    if ((1 == ((I2C_BASE->I2Cx_I2CR >> 5) & (1 << 0))) && (0 == ((I2C_BASE->I2Cx_I2CR >> 5) & (1 << 0))))
    {
        return I2C_BUS_BUSY; // 总线忙，返回错误
    }

    // 设置控制寄存器，转换位发送模式，发送repeat start信号
    I2C_BASE->I2Cx_I2CR |= (1 << 4) | (1 << 2);

    // 设置数据寄存器，发送从机地址和读写位
    I2C_BASE->I2Cx_I2DR = ((uint32_t)slaveAddr << 1) | (I2C_READ == opcode ? 1 : 0);
    return 0;
}

/**
 * @brief I2C 停止函数
 * @param [in] I2C_Register* I2Cx: 指向I2C寄存器结构体的指针
 * @return uint8_t: 返回状态，0表示成功，非0表示失败
 * @details:
 * 1.设置控制寄存器，发送STOP信号
 * 2.等待STOP信号发送完成
 */
uint8_t I2C_Stop(I2C_Register* I2C_BASE)
{
    uint16_t timeout = 0xFFFF;

    // 设置控制寄存器，不发送响应信号，转换为接收模式，发送STOP信号
    I2C_BASE->I2Cx_I2CR &= ~((1 << 5) | (1 << 4) | (1 << 3));

    // 判断是否进入从机模式，发送STOP信号
    while (1 == ((I2C_BASE->I2Cx_I2SR >> 5) & (1 << 0)))
    {
        timeout--;
        if (0 == timeout)
        {
            return I2C_TIMEOUT; // 超时，返回错误
        }
    }
    return I2C_OK;
}

/**
 * @brief I2C 写数据函数
 * @param [in] I2C_Register* I2Cx: 指向I2C寄存器结构体的指针
 * @param [in] const uint8_t* pbuf: 指向数据缓冲区的指针
 * @param [in] uint32_t len: 数据长度
 * @return void
 * @details:
 * 1.等待数据寄存器就绪
 * 2.中断标志位清零
 * 3.转换到发送模式
 * 4.循环发送数据
 *   4.1 等待传输完成
 *   4.2 中断标志位清零
 * 4.3 检查是否出错
 * 5.发送停止信号
 */
void I2C_Write(I2C_Register* I2C_BASE, const uint8_t* puf, uint32_t len)
{
    // 等待数据发送完毕
    while (1 != ((I2C_BASE->I2Cx_I2SR >> 7) & (1 << 0))) {
        printf("Waiting for I2C write ready...\n");
    }

    // 中断标志位清零
    I2C_BASE->I2Cx_I2SR &= ~(1 << 1);
    // 转换到发送模式
    I2C_BASE->I2Cx_I2CR |= (1 << 4);

    while (len--)
    {
        I2C_BASE->I2Cx_I2DR = *puf++; // 发送数据

        // 等待数据发送完毕
        while (1 != ((I2C_BASE->I2Cx_I2SR >> 1) & (1 << 0))) {
            printf("Waiting for I2C write ready...\n\r");
        }

        // 中断标志位清零
        I2C_BASE->I2Cx_I2SR &= ~(1 << 1);

        // 检查是否出错
        if (I2C_OK != I2C_Check(I2C_BASE, I2C_BASE->I2Cx_I2SR))
        {
            break; // 出错，跳出循环
        }
    }
    // 中断标志位清零
    I2C_BASE->I2Cx_I2SR &= ~(1 << 1);
    I2C_Stop(I2C_BASE); // 发送停止信号
    return;
}

/***
 * @brief I2C 读数据函数
 * @param [in] I2C_Register* I2Cx: 指向I2C寄存器结构体的指针
 * @param [in] uint8_t* pbuf: 指向数据缓冲区的指针
 * @param [in] uint32_t len: 数据长度
 * @details:
 * 1.等待数据寄存器就绪
 * 2.中断标志位清零
 * 3.转换到接收模式, 发送应答
 * 4.如果只接收一个字节，发送NACK信号
 * 5.假读数据寄存器
 * 6.循环读取数据
 *   6.1 等待传输完成
 *   6.2 中断标志位清零
 *   6.3 如果是倒数第二个字节，发送NACK信号
 *   6.4 读取数据寄存器数据到缓冲区
 */
void I2C_Read(I2C_Register* I2C_BASE, uint8_t* pbuf, uint32_t len)
{
    volatile uint8_t dummy = 0;
    dummy++; // 防止编译器警告

    // 1.等待数据寄存器就绪
    while (1 != ((I2C_BASE->I2Cx_I2SR >> 7) & (1 << 0))) {}

    // 2.中断标志位清零
    I2C_BASE->I2Cx_I2SR &= ~(1 << 1);
    // 3.转换到接收模式, 发送应答
    I2C_BASE->I2Cx_I2CR &= ~((1 << 4) | (1 << 3));

    // 4.如果只接收一个字节，发送NACK信号
    if (1 == len)
    {
        I2C_BASE->I2Cx_I2CR |= (1 << 3);
    }

    // 5.假读数据寄存器
    dummy = I2C_BASE->I2Cx_I2DR;

    while (len--)
    {
        // 6.1 等待传输完成
        while (1 != ((I2C_BASE->I2Cx_I2SR >> 1) & (1 << 0))) {}

        // 6.2 中断标志位清零
        I2C_BASE->I2Cx_I2SR &= ~(1 << 1);

        // 6.3 如果是最后一个字节，发送停止信号
        if (0 == len)
        {
            I2C_Stop(I2C_BASE); // 发送停止信号
        }

        // 6.4 如果是倒数第二个字节，发送NACK信号
        if (1 == len)
        {
            I2C_BASE->I2Cx_I2CR |= (1 << 3);
        }

        // 6.5 读取数据寄存器数据到缓冲区
        *pbuf++ = I2C_BASE->I2Cx_I2DR;
    }
    return;
}

/**
 * @brief I2C传输函数
 * @param [in] I2C_Register* I2Cx: 指向I2C寄存器结构体的指针
 * @param [in] I2C_Transfer* transfer: 指向I2C传输结构体的指针
 * @return uint8_t: 返回传输状态，0表示成功，非0表示失败
 * @details: 
 * 1.清除标识位（仲裁位 IAL 与 中断标志位 IIF）
 * 2.等待传输完成
 * 3.对于读操作：
 *   3.1 向从机地址发送写入信号，传输完成中断会被置为1
 *   3.2 发送寄存器地址（先重置中断状态位， 判断寄存器地址产长度是否不为0，不为0进行偏移，从高位逐个传输）
 *   3.3 发送重复开始信号和从机地址读操作，传输完成中断会被置为1，检测是否有仲裁和应答。
 *   3.4 读取返回数据
 * 4.对于写操作：
 *   4.1 向从机地址发送写入信号，传输完成中断会被置为1
 *   4.2 发送寄存器地址（先重置中断状态位， 判断寄存器地址产长度是否不为0，不为0进行偏移，从高位逐个传输）
 *   4.3 发送数据
 */
uint8_t I2C_TransferData(I2C_Register* I2C_BASE, I2C_Transfer* transfer)
{
    // 变量定义
    I2C_Operation ulOpcode = transfer->operationCode;
    uint8_t ulRet = 0;

    // 1.清除标志位
    I2C_BASE->I2Cx_I2SR &= ~((1 << 1) | (1 << 4));

    // 2.等待传输完成
    while (1 != ((I2C_BASE->I2Cx_I2SR >> 7) & 0x1)) {
        printf("Waiting for I2C ready...\n\r");
    }

    // 3.如果是读操作，先写入寄存器地址
    if ((transfer->slaverRegisterAddrsLen > 0) && (ulOpcode == I2C_READ))
    {
        ulOpcode = I2C_WRITE;
    }

    // 4.发送起始信号和从机地址
    uint8_t ret = I2C_Start(I2C_BASE, transfer->slaverAdders, ulOpcode);
    if (ret != 0)
    {
        printf("I2C start error: %d\n", ret);
        return ret;
    }

    while (1 != ((I2C_BASE->I2Cx_I2SR >> 1) & (1 << 0))) {}

    ret = I2C_Check(I2C_BASE, I2C_BASE->I2Cx_I2SR);
    if (I2C_OK != ret)
    {
        printf("I2C start check error: %d\n", ret);
        I2C_Stop(I2C_BASE); // 发送停止信号
        return ret;
    }

    // 5.发送寄存器地址（如果有）
    if (transfer->slaverRegisterAddrsLen != 0)
    {
        do
        {
            // 重置中断位
            I2C_BASE->I2Cx_I2SR &= ~(1 << 1);

            // 因为数据寄存器的长度[7:0]只能存放8位，且I2C从高位向地位传输数据, 故需要根据长度右移
            transfer->slaverRegisterAddrsLen--;
            I2C_BASE->I2Cx_I2DR = (transfer->slaverRegisterAddrs >> (8 * transfer->slaverRegisterAddrsLen));
            
            // 等待传输完成
            while (1 != ((I2C_BASE->I2Cx_I2SR >> 1) & (1 << 0))) {}

            // 检查是否出错
            ulRet = I2C_Check(I2C_BASE, I2C_BASE->I2Cx_I2SR);
            if (I2C_OK != ulRet)
            {
                // 发送停止信号
                printf("I2C register address error: %d\n", ulRet);
                I2C_Stop(I2C_BASE);
                return ulRet;
            }
        }
        while(transfer->slaverRegisterAddrsLen > 0);

        // 如果是读操作，发送重复开始信号和从机地址
        if (I2C_READ == transfer->operationCode)
        {
            I2C_BASE->I2Cx_I2SR &= ~(1 << 1); // 清除中断状态位
            // 发送重复开始信号和从机地址 
            ret = I2C_Restart(I2C_BASE, transfer->slaverAdders, I2C_READ);
            if (ret != 0)
            {
                printf("I2C restart error: %d\n", ret);
                I2C_Stop(I2C_BASE); // 发送停止信号
                return ret;
            }

            while (1 != ((I2C_BASE->I2Cx_I2SR >> 1) & (1 << 0))) {}

            // 检查是否出错
            ulRet = I2C_Check(I2C_BASE, I2C_BASE->I2Cx_I2SR);
            if (I2C_OK != ulRet)
            {
                I2C_Stop(I2C_BASE); // 发送停止信号
                printf("I2C restart error: %d\n", ulRet);
                return ulRet;
            }
        }
    }

    // 读操作：写入数据
    if ((I2C_WRITE == transfer->operationCode) && (transfer->uLength > 0))
    {
        // 写入数据
        I2C_Write(I2C_BASE, transfer->pbuf, transfer->uLength);
    }

    // 写操作：读取数据
    if ((I2C_READ == transfer->operationCode) && (transfer->uLength > 0))
    {
        // 读取数据
        I2C_Read(I2C_BASE, transfer->pbuf, transfer->uLength);
    }
    return I2C_OK;
}

/**
 * @brief I2C 写入单字节函数
 * @param [in] uint8_t addr: 从机地址
 * @param [in] uint8_t reg: 寄存器地址
 * @param [in] uint8_t data: 要写入的数据
 * @param [in] I2C_Register* I2C_BASE: 指向I2C寄存器结构体的指针
 * @return uint8_t: 返回状态，0表示成功，非0表示失败
 */
uint8_t I2C_WriteOneByte(uint8_t addr,uint8_t reg, uint8_t data,I2C_Register *I2C_BASE)
{
    I2C_Transfer I2C_Transfer;
    uint8_t buf = data;

    I2C_Transfer.slaverAdders = addr;
    I2C_Transfer.operationCode = I2C_WRITE;
    I2C_Transfer.slaverRegisterAddrs = reg;
    I2C_Transfer.slaverRegisterAddrsLen = 1;
    I2C_Transfer.pbuf = &buf;
    I2C_Transfer.uLength = 1;

    uint8_t ret = I2C_TransferData(I2C_BASE, &I2C_Transfer);
    return ret;
}

/**
 * @brief I2C 读取单字节函数
 * @param [in] uint8_t addr: 从机地址
 * @param [in] uint8_t reg: 寄存器地址
 * @param [in] I2C_Register* I2C_BASE: 指向I2C寄存器结构体的指针
 * @return uint8_t: 返回读取的数据
 */
uint8_t I2C_ReadOneByte(uint8_t addr,uint8_t reg,I2C_Register *I2C_BASE)
{
    I2C_Transfer I2C_Transfer;
    uint8_t buf;

    I2C_Transfer.slaverAdders = addr;
    I2C_Transfer.operationCode = I2C_READ;
    I2C_Transfer.slaverRegisterAddrs = reg;
    I2C_Transfer.slaverRegisterAddrsLen = 1;
    I2C_Transfer.pbuf = &buf;
    I2C_Transfer.uLength = 1;

    uint8_t ret = I2C_TransferData(I2C_BASE, &I2C_Transfer);
    if (ret != I2C_OK)
    {
        return 0; // 读取失败，返回0
    }
    return buf;
}