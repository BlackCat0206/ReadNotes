/**
 * @brief ICM20608G SPI 读写测试函数
 * @author Chime Lee
 * @date 2026-01-09
 * @version 1.0
 * @details
 * 本文件实现了 ICM20608G 传感器通过 SPI 接口进行读写操作的功能。
 */
#include "SPI_Custom.h"
#include "my_printf.h"
#include "ICM20608G_Custom.h"

static ICM20608G_GYRO_ACC_adc ICM20608_DATA;

/**
 * @brief ICM20608G 寄存器写入数据
 * @param uint8_t reg_addr 寄存器地址
 * @param uint8_t data 要写入的数据
 * @return void
 * @details 
 * 1. 地址的最高位为0，表示写操作。
 * 2. 片选信号由软件控制。
 * 3. 发送地址和数据。
 * 4. 取消片选信号。
 */
void ICM20608G_WriteRegister(uint8_t reg_addr, uint8_t data)
{
    reg_addr &= ~(0x80);  // 清除最高位，表示写操作
    SPI_SS_Low();  // 片选拉低
    SPI_ReadWriteData(SPI3, reg_addr, 1000);  // 发送寄存器地址
    SPI_ReadWriteData(SPI3, data, 1000);      // 发送数据
    SPI_SS_High(); // 片选拉高
    return;
}

/**
 * @brief ICM20608G 寄存器读取数据
 * @param uint8_t reg_addr 寄存器地址
 * @return uint8_t 读取到的数据
 * @details
 * 1. 地址的最高位为1，表示读操作。
 * 2. 片选信号由软件控制。
 * 3. 发送地址，接收数据。
 * 4. 取消片选信号。
 */
uint8_t ICM20608G_ReadRegister(uint8_t reg_addr)
{
    uint8_t received_data;
    reg_addr |= 0x80;  // 设置最高位，表示读操作
    SPI_SS_Low();  // 片选拉低
    SPI_ReadWriteData(SPI3, reg_addr, 1000);  // 发送寄存器地址
    received_data = SPI_ReadWriteData(SPI3, 0xFF, 1000);  // 发送空数据以接收数据
    SPI_SS_High(); // 片选拉高
    return received_data;
}

/**
 * @brief ICM20608G 读取多字节数据
 * @param uint8_t start_addr 起始寄存器地址
 * @param uint8_t* buffer 数据缓冲区
 * @param uint32_t length 读取长度
 * @return void
 * @details
 * 1. 地址的最高位为1，表示读操作。
 * 2. 片选信号由软件控制。
 * 3. 发送起始地址，连续接收数据。
 * 4. 取消片选信号。
 */
void ICM20608G_ReadMultipleRegisters(uint8_t start_addr, uint8_t* buffer, uint32_t length)
{
    start_addr |= 0x80;  // 设置最高位，表示读操作
    SPI_SS_Low();  // 片选拉低
    SPI_ReadWriteData(SPI3, start_addr, 1000);  // 发送起始寄存器地址
    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = SPI_ReadWriteData(SPI3, 0xFF, 1000);  // 发送空数据以接收数据
    }
    SPI_SS_High(); // 片选拉高
    return;
}

/**
 * @brief ICM20608G [辅助函数] 打印指定长度的数据
 * @param uint8_t* data 数据缓冲区
 * @param uint32_t length 数据长度
 * @return void
 * @details
 */
void ICM20608G_PrintData(uint8_t* data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        printf("Read_Custom: %d 0x%02x\n\r", i, data[i]);
    }
    return;
}

/**
 * @brief ICM20608G 读取加速度原始数据
 * @param void
 * @return void
 * @details
 * 1. 读取加速度寄存器数据。
 * 2. 打印读取到的数据。
 */
void ICM20608G_ReadAccelData(void)
{
    uint8_t buffer[6];
    ICM20608G_ReadMultipleRegisters(ICM20608G_ACC_XOUT_H, buffer, 6);
    ICM20608_DATA.acc_x_adc = (int16_t)((buffer[0] << 8) | buffer[1]);
    ICM20608_DATA.acc_y_adc = (int16_t)((buffer[2] << 8) | buffer[3]);
    ICM20608_DATA.acc_z_adc = (int16_t)((buffer[4] << 8) | buffer[5]);
    printf("Accel Data: \n\r");
    ICM20608G_PrintData(buffer, 6);
    return;
}

/**
 * @brief ICM20608G 读取陀螺仪原始数据
 * @param void
 * @return void
 * @details
 * 1. 读取陀螺仪寄存器数据。
 * 2. 打印读取到的数据。
 */
void ICM20608G_ReadGyroData(void)
{
    uint8_t buffer[6];
    ICM20608G_ReadMultipleRegisters(ICM20608G_GYRO_XOUT_H, buffer, 6);
    ICM20608_DATA.gyro_x_adc = (int16_t)((buffer[0] << 8) | buffer[1]);
    ICM20608_DATA.gyro_y_adc = (int16_t)((buffer[2] << 8) | buffer[3]);
    ICM20608_DATA.gyro_z_adc = (int16_t)((buffer[4] << 8) | buffer[5]);
    printf("Gyro Data: \n\r");
    ICM20608G_PrintData(buffer, 6);
    return;
}

/**
 * @brief ICM20608G 读取温度原始数据
 * @param void
 * @return void
 * @details
 * 1. 读取温度寄存器数据。
 * 2. 打印读取到的数据。
 */
void ICM20608G_ReadTempData(void)
{
    uint8_t buffer[2];
    ICM20608G_ReadMultipleRegisters(ICM20608G_TEMP_OUT_H, buffer, 2);
    ICM20608_DATA.temp_adc = (int16_t)((buffer[0] << 8) | buffer[1]);
    printf("Temp Data: \n\r");
    ICM20608G_PrintData(buffer, 2);
    return;
}

/**
 * @brief ICM20608G 初始化函数
 * @param void
 * @return uint8_t 初始化结果，0表示成功
 * @details
 * 1. 初始化SPI接口。
 * 2. 复位ICM20608G传感器。
 * 3. 读取传感器ID进行验证。
 * 4. 配置传感器工作模式。
 */
uint8_t ICM20608G_Init(void)
{
    if (0 != SPI_Init(SPI3)) 
    {
        printf("SPI3 Init Failed!\n\r");
        return 1;
    }

    printf("ICM20608G Resetting...\n\r");
    ICM20608G_WriteRegister(ICM20608G_PWR_MGMT_1, 0x01);  // 传感器时钟设置

    uint8_t who_am_i = ICM20608G_ReadRegister(ICM20608G_WHO_AM_I);
    printf("ICM20608G WHO_AM_I: 0x%02X\n\r", who_am_i);
    if (who_am_i != 0xAF) 
    {
        printf("ICM20608G WHO_AM_I Error: 0x%02X\n\r", who_am_i);
        return 2;
    }

    // 配置传感器工作模式（本次均使用默认模式）
    ICM20608G_WriteRegister(ICM20608G_SMPLRT_DIV, 0x00);      // 采样率分频1K
    ICM20608G_WriteRegister(ICM20608G_CONFIG, 0x00);          // 禁止FIFO
    ICM20608G_WriteRegister(ICM20608G_GYRO_CONFIG, 0x00);     // 使用默认量程和低通滤波器
    ICM20608G_WriteRegister(ICM20608G_ACC_CONFIG, 0x00);      // 使用默认量程和低通滤波器
    ICM20608G_WriteRegister(ICM20608G_ACC_CONFIG2, 0x00);     // 使用默认低通滤波器
    ICM20608G_WriteRegister(ICM20608G_LP_MODE_CFG, 0x00);     // 关闭低功耗模式
    ICM20608G_WriteRegister(ICM20608G_FIFO_EN, 0x00);         // 禁止传感器FIFO
    ICM20608G_WriteRegister(ICM20608G_PWR_MGMT_2, 0x00);      // 启用所有传感器
    printf("ICM20608G Initialized Successfully!\n\r");
    return 0;
}