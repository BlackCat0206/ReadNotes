/**
 * @file AP3216.h
 * @brief AP3216C 光学传感器驱动头文件
 * @author Chime Lee
 * @date 2026-01-06
 * @version 1.0
 * @note:
 * I2C通信实现AP3216C传感器的初始化及数据读取。 
 * @details:
 * 1. 定义AP3216C寄存器地址和操作码
 * 2. 声明初始化函数和数据读取函数
 * 3. 提供函数原型供外部调用
 */
#include "I2C_Custom.h"
#include "AP3216.h"
#include "my_printf.h"


typedef enum {
    AP3216C_I2C_ADDRESS = 0x1E,      // AP3216C I2C设备地址
    AP3216C_REG_SYSTEM_CONFIG = 0x00,// 系统配置寄存器
    AP3216C_REG_INT_STATUS = 0x01,   // 中断状态寄存器
    AP3216C_REG_IR_DATA_L = 0x0A,    // 红外数据低字节寄存器
    AP3216C_REG_IR_DATA_H = 0x0B,    // 红外数据高字节寄存器
    AP3216C_REG_PS_DATA_L = 0x0C,    // 接近数据低字节寄存器
    AP3216C_REG_PS_DATA_H = 0x0D,    // 接近数据高字节寄存器
    AP3216C_REG_ALS_DATA_L = 0x0E,   // 环境光数据低字节寄存器
    AP3216C_REG_ALS_DATA_H = 0x0F     // 环境光数据高字节寄存器
} AP3216C_Registers;

typedef enum {
    AP3216C_CMD_INIT = 0x04,         // 初始化/复位命令
    AP3216C_CMD_START_ALL = 0x03     // 启动所有传感器命令
} AP3216C_Commands;

static void delay(volatile unsigned int d)
{
    while(d--);
}

/**
 * @brief 读取AP3216C传感器状态
 * @param [in] void
 * @return uint8_t: 返回状态，0表示正常，非0表示异常
 * @details:
 * 1. 读取系统配置寄存器
 * 2. 判断返回值是否有效
 */
uint8_t Ap3216c_ReadStatus(void)
{
    uint8_t data = I2C_ReadOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_SYSTEM_CONFIG, I2C1);
    if (data != 0 && data != 0xFF)
    {
        return 0; // AP3216C正常
    }
    else
    {
        return 1; // AP3216C异常
    }
}

/**
 * @brief 初始化AP3216C传感器
 * @param [in] void
 * @return uint8_t: 返回状态，0表示成功，非0表示失败
 * @details:
 * 1. 初始化I2C接口
 * 2. 复位AP3216C传感器
 * 3. 等待至少10ms
 * 4. 启动传感器测量
 */
uint8_t Ap3216c_Init(void)
{
    I2C_Init(); // 初始化I2C接口
    printf("I2C_Init success!\n\r");
    // 复位AP3216C传感器
    uint8_t ret = I2C_WriteOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_SYSTEM_CONFIG, AP3216C_CMD_INIT, I2C1);
    if (ret != I2C_OK)
    {
        printf("AP3216C Reset fail!\n\r");
        return ret; // 复位失败，返回错误码
    }
    delay(10000); // 等待至少10ms

    // 启动传感器测量
    ret = I2C_WriteOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_SYSTEM_CONFIG, AP3216C_CMD_START_ALL, I2C1);
    if (ret != I2C_OK)
    {
        printf("AP3216C Start fail!\n\r");
    }
    return Ap3216c_ReadStatus(); // 返回启动结果
}

/**
 * @brief 读取AP3216C传感器的红外(IR)数据
 * @param [out] uint16_t* ir: 指向存储IR数据的变量指针
 * @return void
 * @details:
 * 1. 读取IR数据的低字节和高字节
 * 2. 处理无效数据情况(低字节最高位为1)
 * 3. 组合成16位数据
 * @note:
 * IR数据格式: 高字节在前，低字节在后，低字节的高6位无效
 */
void Ap3216c_ReadIR(uint16_t* ir)
{
    uint8_t irLow = I2C_ReadOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_IR_DATA_L, I2C1);
    uint8_t irHigh = I2C_ReadOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_IR_DATA_H, I2C1);

    if (1 == (irLow & 0x80))
    {
        *ir = 0; // 无效数据
    }
    else
    {
        *ir = ((uint16_t)irHigh << 2) | (irLow & 0x03); // 组合成16位数据
    }
}

/**
 * @brief 读取AP3216C传感器的环境光(ALS)数据
 * @param [out] uint16_t* als: 指向存储ALS数据的变量指针
 * @return void
 */
void Ap3216c_ReadALS(uint16_t* als)
{
    uint8_t alsLow = I2C_ReadOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_ALS_DATA_L, I2C1);
    uint8_t alsHigh = I2C_ReadOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_ALS_DATA_H, I2C1);

    *als = ((uint16_t)alsHigh << 8) | alsLow; // 组合成16位数据
}

/**
 * @brief 读取AP3216C传感器的接近(PS)数据
 * @param [out] uint16_t* ps: 指向存储PS数据的变量指针
 * @return void
 * @details:
 * 1. 读取PS数据的低字节和高字节
 * 2. 处理无效数据情况(低字节第6位为1)
 * 3. 组合成16位数据
 * @note:
 * PS数据格式: 高字节在前，低字节在后，低字节的[3:0]有效，高字节的[5:0]有效。
 */
void Ap3216c_ReadPS(uint16_t* ps)
{
    uint8_t psLow = I2C_ReadOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_PS_DATA_L, I2C1);
    uint8_t psHigh = I2C_ReadOneByte(AP3216C_I2C_ADDRESS, AP3216C_REG_PS_DATA_H, I2C1);

    if (1 == (psLow & (1 << 6)))
    {
        *ps = 0; // 无效数据
    }
    else
    {
        *ps = (((uint16_t)(psHigh & 0x3F)) << 4) | (psLow & 0x0F); // 组合成16位数据
    }
}

/**
 * @brief 读取AP3216C传感器的IR、PS和ALS数据
 * @param [out] uint16_t* ir: 指向存储IR数据的变量指针
 * @param [out] uint16_t* ps: 指向存储PS数据的变量指针
 * @param [out] uint16_t* als: 指向存储ALS数据的变量指针
 * @return void
 */
void Ap3216c_ReadData(uint16_t* ir, uint16_t* ps, uint16_t* als)
{
    Ap3216c_ReadIR(ir);
    Ap3216c_ReadPS(ps);
    Ap3216c_ReadALS(als);
    return;
}