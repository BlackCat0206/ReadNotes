/***
 * @file DHT11.c
 * @brief DHT11 GPIO driver
 * @author Chime Lee
 * @date 2026-01-03
 * @version 1.0
 */

#include "DHT11_Custom.h"
#include "timer.h"
#include "my_printf.h"

// 多路复用引脚配置 引脚名：CSI_VSYNC
volatile unsigned int* SW_MUX_CTL_PAD_CSI_VSYNC_SW_MUX_Control_Ptr;
volatile unsigned int* SW_PAD_CTL_PAD_CSI_VSYNC_SW_PAD_Control_Ptr;
volatile unsigned int* CSI_VSYNC_SELECT_INPUT_DAISY_Ptr;
// 时钟使能寄存器指针
volatile unsigned int* CCM_CCGR3_Ptr;

// GPIO4 数据寄存器 和 方向寄存器 指针
volatile unsigned int* GPIO4_GDIR_Ptr;
volatile unsigned int* GPIO4_DR_Ptr;

/**
 * @brief GPIO4_IO19 (CSI_VSYNC) 初始化函数
 * 配置该引脚为 GPIO 输入模式，用于连接 DHT11 传感器的数据线
 * @param [in] void
 * @return void
 */
void GPIO4_IO19_Init(void)
{
    // 一、初始化各寄存器地址
    // 1. GPIO4_IO19 多路复用寄存器：20E_01DC
    SW_MUX_CTL_PAD_CSI_VSYNC_SW_MUX_Control_Ptr = (volatile unsigned int*)0x20E01DC;
    // 2. GPIO4_IO19 电器参数配置寄存器：20E_0468
    SW_PAD_CTL_PAD_CSI_VSYNC_SW_PAD_Control_Ptr = (volatile unsigned int*)0x20E0468;
    // 3. 二次选择寄存器：20E_052C
    CSI_VSYNC_SELECT_INPUT_DAISY_Ptr = (volatile unsigned int*)0x20E052C;

    // 配置多路复用寄存器为 GPIO4_IO19 功能
    unsigned int mux_reg = *SW_MUX_CTL_PAD_CSI_VSYNC_SW_MUX_Control_Ptr;
    mux_reg &= (~0xF); // 清除 MUX_MODE 位
    mux_reg |= 0x5;  // 设置 MUX_MODE 为 5 (GPIO4_IO19)
    *SW_MUX_CTL_PAD_CSI_VSYNC_SW_MUX_Control_Ptr = mux_reg;

    // 配置电器参数寄存器 (默认值，本次不修改内容)

    // 配置二次选择寄存器为默认值 (默认值，本次不修改内容)

    // 二、时钟使能 (时钟寄存器地址为 20C_4074)
    CCM_CCGR3_Ptr = (volatile unsigned int*)0x20C4074;
    unsigned int ccgr3_reg = *CCM_CCGR3_Ptr;
    ccgr3_reg |= (0x3 << 12); // 使能 GPIO4 时钟
    *CCM_CCGR3_Ptr = ccgr3_reg;

    // 三、参数配置 (具体读写操作在其他函数中完成)
    // 数据寄存器 地址：20A_8000
    GPIO4_DR_Ptr = (volatile unsigned int*)0x20A8000;
    // 方向寄存器 地址：20A_8004
    GPIO4_GDIR_Ptr = (volatile unsigned int*)0x20A8004;

    // 四、中断配置（本次不使用中断功能）
    
    return;
}

/**
 * @brief GPIO4_IO19 设置为输入模式
 * @param [in] void
 * @return void
 * @note: 寄存器第19位为0时为输入模式
 */
void GPIO4_IO19_SetPinInputMode(void)
{
    unsigned int gdir_reg = *GPIO4_GDIR_Ptr;
    gdir_reg &= ~(1 << 19); // 设置第19位为0，配置为输入模式
    *GPIO4_GDIR_Ptr = gdir_reg;
}

/**
 * @brief GPIO4_IO19 设置为输出模式
 * @param [in] void
 * @return void
 * @note: 寄存器第19位为1时为输出模式
 */
void GPIO4_IO19_SetPinOutputMode(void)
{
    unsigned int gdir_reg = *GPIO4_GDIR_Ptr;
    gdir_reg |= (1 << 19); // 设置第19位为1，配置为输出模式
    *GPIO4_GDIR_Ptr = gdir_reg;
}

/**
 * @brief 读取 GPIO4_IO19 引脚电平状态
 * @param [in] void
 * @return int 引脚电平状态，0表示低电平，1表示高电平
 * @note: 读取数据寄存器的第19位
 */
static int GPIO4_IO19_ReadPinLevel(void)
{
    unsigned int dr_reg = *GPIO4_DR_Ptr;
    return ((dr_reg >> 19) & 0x1) ? 1 : 0; // 返回第19位的状态
}

/**
 * @brief 设置 GPIO4_IO19 引脚电平
 * @param [in] int level 引脚电平，0表示低电平，1表示高电平
 * @return void
 * @note: 设置数据寄存器的第19位为1
 */
void GPIO4_IO19_SetPinLevel(int level)
{
    unsigned int dr_reg = *GPIO4_DR_Ptr;
    if (1 == level)
    {
        dr_reg |= (1 << 19); // 设置第19位为1，输出高电平
    }
    else
    {
        dr_reg &= ~(1 << 19); // 清除第19位为0，输出低电平
    }
    *GPIO4_DR_Ptr = dr_reg;
}

/**
 * @brief DHT11 传感器初始化
 * @param [in] void
 * @return void
 * @note: 设置为输出模式，并输出高电平，等待传感器稳定延时2000000us
 */
static void DHT11_Init(void)
{
    GPIO4_IO19_SetPinOutputMode(); // 设置为输出模式
    GPIO4_IO19_SetPinLevel(1);     // 输出高电平

    // 延时2000000us，等待传感器稳定
    gpt2_chan1_delay_us(2000000);

    return;
}

/**
 * @brief DHT11 传感器开始信号发送
 * @param [in] void
 * @return void
 * @note: 设置为输出模式，拉低引脚至少18ms。
 */
static void DHT11_StartSignal(void)
{
    GPIO4_IO19_SetPinLevel(0); // 拉低引脚
    gpt2_chan1_delay_us(20000); // 延时20ms
}

/**
 * @brief DHT11 传感器等待响应数据
 * @param [in] int value 等待数据的电平值，0表示等待低电平，1表示等待高电平
 * @param [in] unsigned int timeout_us 超时时间，单位微秒
 * @return int 0表示成功等待到指定电平，-1表示超时
 * @note: 用于等待传感器响应信号，与DHT11_ReadBit函数配合使用
 */
static int DHT11_WaitForLevel(int value, unsigned int timeout_us)
{
    while (timeout_us--)
    {
        if (GPIO4_IO19_ReadPinLevel() == value)
        {
            return 0; // 成功等待到指定电平
        }
        gpt2_chan1_delay_us(1); // 延时1us
    }
    return -1; // 超时
}

/**
 * @brief DHT11 数据读取，读取一个字节
 * @param [in] void
 * @return int 读取到的字节数据，读取失败返回-1
 * @note: 通过调用DHT11_WaitForLevel函数等待数据位的电平变化
 * 读取数据方式：一直等到高电平出现，延后40us后读取电平，若为高电平（70us）则该位为1，否则为(26~28)0
 */
static int DHT11_ReadByte(void)
{
    int i = 0;
    int data = 0;

    for (i = 0; i < 8; ++i)
    {
        if (-1 == DHT11_WaitForLevel(1, 1000))
        {
            printf("DHT11_ReadByte: Wait for high level timeout\r\n");
            return -1; // 等待高电平超时
        }
        
        gpt2_chan1_delay_us(40); // 延时40us后读取电平
        data <<= 1; // 左移一位
        if (GPIO4_IO19_ReadPinLevel() == 1)
        {
            data |= 1; // 该位为1
        }

        if (-1 == DHT11_WaitForLevel(0, 1000))
        {
            printf("DHT11_ReadByte: Wait for low level timeout\r\n");
            return -1; // 等待低电平超时
        }
    }
    return data;
}

/**
 * @brief DHT11 传感器读取温湿度数据
 * @param [in] void
 * @return void
 * @note: 通过调用DHT11_ReadByte函数读取5个字节的数据
 */
void DHT11_ReadData(void)
{
    // GPIO4_IO19初始化
    GPIO4_IO19_Init();
    // DHT11初始化
    DHT11_Init();
    // 发送开始信号
    DHT11_StartSignal();
    // 升为高电平并延续20-40us，等待DHT11响应
    GPIO4_IO19_SetPinLevel(1);
    gpt2_chan1_delay_us(40);

    // 设置GPIO4_IO19为输入模式，准备读取数据
    GPIO4_IO19_SetPinInputMode();

    // 数据存储数组
    unsigned char data[5] = {0};    // 存储5个字节数据
    unsigned char checksum = 0;     // 校验和

    // 等待DHT11响应信号，先等低电平80us，再等高电平80us
    if (0 == GPIO4_IO19_ReadPinLevel())
    {
        while (GPIO4_IO19_ReadPinLevel() == 0); // 等待低电平结束
        while (GPIO4_IO19_ReadPinLevel() == 1); // 等待高电平结束
        data[0] = DHT11_ReadByte(); // 读取湿度整数部分
        data[1] = DHT11_ReadByte(); // 读取湿度小数
        data[2] = DHT11_ReadByte(); // 读取温度整数部分
        data[3] = DHT11_ReadByte(); // 读取温度小数
        data[4] = DHT11_ReadByte(); // 读取校验和
        // 计算校验和
        checksum = data[0] + data[1] + data[2] + data[3];
        if (checksum != data[4])
        {
            printf("DHT11_ReadData: Checksum error\r\n");
        }
        else
        {
            printf("DHT11_ReadData: Humidity = %d.%d %% Temperature = %d.%d C\r\n",
                   data[0], data[1], data[2], data[3]);
        }
    }
}