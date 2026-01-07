#ifndef AP3216_H
#define AP3216_H

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
uint8_t Ap3216c_Init(void);

/**
 * @brief 读取AP3216C传感器的IR、PS和ALS数据
 * @param [out] uint16_t* ir: 指向存储IR数据的变量指针
 * @param [out] uint16_t* ps: 指向存储PS数据的变量指针
 * @param [out] uint16_t* als: 指向存储ALS数据的变量指针
 * @return void
 */
void Ap3216c_ReadData(uint16_t* ir, uint16_t* ps, uint16_t* als);

#endif // AP3216_H