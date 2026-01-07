#ifndef _I2C_CUSTOM_H
#define _I2C_CUSTOM_H

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

/**
 * @brief I2C寄存器结构体
 */
typedef struct {
    volatile uint16_t I2Cx_IADR;    // I2C地址寄存器
    uint8_t reservedIADR[2];        // 保留字节
    volatile uint16_t I2Cx_IFDR;    // I2C频率分频寄存器
    uint8_t reservedIFDR[2];        // 保留字节
    volatile uint16_t I2Cx_I2CR;    // I2C控制寄存器
    uint8_t reservedICCR[2];        // 保留字节
    volatile uint16_t I2Cx_I2SR;    // I2C状态寄存器
    uint8_t reservedISR[2];         // 保留字节
    volatile uint16_t I2Cx_I2DR;    // I2C数据寄存器
}I2C_Register;

/**
 * @brief I2C传输结构体
 */
typedef struct{
    uint8_t  slaverAdders;           // 从机地址
    uint32_t operationCode;          // 操作码
    uint32_t slaverRegisterAddrs;    // 从机寄存器地址
    uint8_t  slaverRegisterAddrsLen; // 寄存器地址长度
    volatile uint32_t uLength;       // 数据长度
    uint8_t* volatile pbuf;          // 数据
}I2C_Transfer;

/**
 * @brief I2C状态枚举
 */
typedef enum{
    I2C_OK = 0,
    I2C_ADDRNAK,
    I2C_DATA_NAK,
    I2C_ARBITRATION_LOST,
    I2C_BUS_ERROR,
    I2C_TIMEOUT,
    I2C_BUS_BUSY,
    I2C_MODULE_NOT_ENABLED
}I2C_Status;

/**
 * @brief I2C操作符
 */
typedef enum{
    I2C_WRITE = 0,
    I2C_READ  = 1,
}I2C_Operation;

#define I2C1_BASE_ADDR 0x021A0000u  // I2C1寄存器基地址
#define I2C2_BASE_ADDR 0x021A4000u  // I2C2寄存器基地址
#define I2C3_BASE_ADDR 0x021A8000u  // I2C3寄存器基地址
#define I2C4_BASE_ADDR 0x021F8000u  // I2C4寄存器基地址

#define I2C1 ((I2C_Register *)I2C1_BASE_ADDR)   // I2C1寄存器指针
#define I2C2 ((I2C_Register *)I2C2_BASE_ADDR)   // I2C2寄存器指针
#define I2C3 ((I2C_Register *)I2C3_BASE_ADDR)   // I2C3寄存器指针
#define I2C4 ((I2C_Register *)I2C4_BASE_ADDR)   // I2C4寄存器指针

/**
 * @brief I2C 初始化函数
 * @param [in] void
 * @return void
 * @note:
 * 配置时钟分频，使能I2C模块
 */
void I2C_Init(void);

/**
 * @brief I2C 写入单字节函数
 * @param [in] uint8_t addr: 从机地址
 * @param [in] uint8_t reg: 寄存器地址
 * @param [in] uint8_t data: 要写入的数据
 * @param [in] I2C_Register* I2C_BASE: 指向I2C寄存器结构体的指针
 * @return uint8_t: 返回状态，0表示成功，非0表示失败
 */
uint8_t I2C_WriteOneByte(uint8_t addr,uint8_t reg, uint8_t data,I2C_Register *I2C_BASE);

/**
 * @brief I2C 读取单字节函数
 * @param [in] uint8_t addr: 从机地址
 * @param [in] uint8_t reg: 寄存器地址
 * @param [in] I2C_Register* I2C_BASE: 指向I2C寄存器结构体的指针
 * @return uint8_t: 返回读取的数据
 */
uint8_t I2C_ReadOneByte(uint8_t addr,uint8_t reg,I2C_Register *I2C_BASE);

#endif // _I2C_CUSTOM_H