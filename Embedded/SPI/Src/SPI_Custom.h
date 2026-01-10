#ifndef _SPI_CUSTOM_H
#define _SPI_CUSTOM_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define SPI1_BASE 0x02008000
#define SPI2_BASE 0x0200C000
#define SPI3_BASE 0x02010000
#define SPI4_BASE 0x02014000

typedef struct{
    volatile uint32_t ECSPIx_RXDATA;        // 0x00 接收数据寄存器
    volatile uint32_t ECSPIx_TXDATA;        // 0x04 发送数据
    volatile uint32_t ECSPIx_CONREG;        // 0x08 控制寄存器
    volatile uint32_t ECSPIx_CONFIGREG;     // 0x0C 配置寄存器
    volatile uint32_t ECSPIx_INTREG;        // 0x10 中断寄存器
    volatile uint32_t ECSPIx_DMAREG;        // 0x14 DMA 控制寄存器
    volatile uint32_t ECSPIx_STATREG;       // 0x18 状态寄   存器
    volatile uint32_t ECSPIx_PERIODREG;     // 0x1C 时钟分频寄存器
    volatile uint32_t ECSPIx_TESTREG;       // 0x20 测试寄存器
    volatile uint32_t ECSPIx_MSGDATA;       // 0x24 消息数据寄存器
}SPI_Register;

#define SPI1 ((SPI_Register *)SPI1_BASE)
#define SPI2 ((SPI_Register *)SPI2_BASE)
#define SPI3 ((SPI_Register *)SPI3_BASE)
#define SPI4 ((SPI_Register *)SPI4_BASE)

/**
 * @brief 初始化 SPI1 接口
 * @param spi_base SPI 基地址
 * @return 0 成功，其他值失败
 */
int SPI1_Init(SPI_Register* spi_base);

/**
 * @brief 初始化 SPI 接口
 * @param spi_base SPI 基地址
 * @return 0 成功，其他值失败
 */
int SPI_Init(SPI_Register* spi_base);

/**
 * @brief 通过 SPI 读写数据
 * @param [in] SPI_Register* spi_base SPI 基地址
 * @param [in] uint8_t* tx_data 发送数据缓冲区
 * @param [in] uint32_t time_out 超时时间
 * @return uint8_t 接收数据
 */
uint8_t SPI_ReadWriteData(SPI_Register* spi_base, uint8_t tx_data, uint32_t time_out);

/**
 * @brief 软件片选拉高
 * @param void
 * @return void
 */
void SPI_SS_High(void);

/**
 * @brief 软件片选拉低
 * @param void
 * @return void
 */
void SPI_SS_Low(void);

unsigned char SPI_TESTRW(SPI_Register *spi_num);

#endif // _SPI_CUSTOM_H