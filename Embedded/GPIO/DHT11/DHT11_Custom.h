#ifndef _DHT11_H__
#define _DHT11_H_

/**
 * @brief DHT11 传感器读取温湿度数据
 * @param [in] void
 * @return void
 * @note: 通过调用DHT11_ReadByte函数读取5个字节的数据
 */
void DHT11_ReadData(void);

#endif /* _DHT11_H__ */