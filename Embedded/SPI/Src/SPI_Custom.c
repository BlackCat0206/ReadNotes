/***
 * @brief SPI 裸机驱动程序
 * @author Chime Lee
 * @date 2026-01-08
 * @note
 * 本驱动使用与IMX6ULL系列芯片兼容的裸机SPI驱动程序。
 * @details
 * 
 */
#include "SPI_Custom.h"
#include "my_printf.h"

volatile static uint32_t* IOMUXC_SW_MUX_CTL_PAD_UART2_TX_DATA = (volatile uint32_t*)0x020E0094;  // SS0 多路复用器寄存器
volatile static uint32_t* IOMUXC_SW_PAD_CTL_PAD_UART2_TX_DATA = (volatile uint32_t*)0x020E0320;  // SS0 PAD 控制寄存器
volatile static uint32_t* IOMUXC_ECSPI3_SS0_B_SELECT_INPUT    = (volatile uint32_t*)0x020E0560;  // SS0 选择输入寄存器

volatile static uint32_t* IOMUXC_SW_MUX_CTL_PAD_UART2_RX_DATA = (volatile uint32_t*)0x020E0098;  // SCLK 多路复用器寄存器
volatile static uint32_t* IOMUXC_SW_PAD_CTL_PAD_UART2_RX_DATA = (volatile uint32_t*)0x020E0324;  // SCLK PAD 控制寄存器
volatile static uint32_t* IOMUXC_ECSPI3_SCLK_SELECT_INPUT    =  (volatile uint32_t*)0x020E0554;  // SCLK 选择输入寄存器

volatile static uint32_t* IOMUXC_SW_MUX_CTL_PAD_UART2_CTS_B    = (volatile uint32_t*)0x020E009C;  // MOSI 选择输入寄存器
volatile static uint32_t* IOMUXC_SW_PAD_CTL_PAD_UART2_CTS_B    = (volatile uint32_t*)0x020E0328;  // MOSI PAD 控制寄存器
volatile static uint32_t* IOMUXC_ECSPI3_MOSI_SELECT_INPUT      = (volatile uint32_t*)0x020E055C;  // MOSI 选择输入寄存器

volatile static uint32_t* IOMUXC_SW_MUX_CTL_PAD_UART2_RTS_B    = (volatile uint32_t*)0x020E00A0;  // MISO 选择输入寄存器
volatile static uint32_t* IOMUXC_SW_PAD_CTL_PAD_UART2_RTS_B    = (volatile uint32_t*)0x020E032C;  // MISO PAD 控制寄存器
volatile static uint32_t* IOMUXC_ECSPI3_MISO_SELECT_INPUT      = (volatile uint32_t*)0x020E0558;  // MISO 选择输入寄存器

volatile static uint32_t* GPIO_DIR = (volatile uint32_t*)0x0209C004;  // GPIO 方向寄存器
volatile static uint32_t* GPIO_DR = (volatile uint32_t*)0x0209C000;   // GPIO 数据寄存器
/**
 * @brief SS0 多路复用器寄存器设置
 * @param void
 * @returnK
 */
void Configure_SS0_MUX(void) {
    *IOMUXC_SW_MUX_CTL_PAD_UART2_TX_DATA = 0x00000005;  // 设置为 GPIO1_IO20
    volatile uint32_t* GPIO_DIR = (volatile uint32_t*)0x0209C004;  // GPIO 方向寄存器
    *(volatile uint32_t*)GPIO_DIR |= (1 << 20);  // 设置为输出
    volatile uint32_t* GPIO_DR = (volatile uint32_t*)0x0209C000;   // GPIO 数据寄存器
    *(volatile uint32_t*)GPIO_DR |= (1 << 20);  // 设置高电平
    *IOMUXC_ECSPI3_SS0_B_SELECT_INPUT = 0x00000000;     // 设置为 ECSPI3 SS0
}

/**
 * @brief 初始化 SPI 接口
 * @param spi_base SPI 基地址
 * @return 0 成功，其他值失败
 */
int SPI_Init(SPI_Register* spi_base) 
{
    // 一、初始化
    // 1.1 多路复用电器参数配置
    // 设置多路复用器寄存器
    *IOMUXC_SW_MUX_CTL_PAD_UART2_TX_DATA = 0x00000005;  // 设置为 GPIO1_IO20
    *IOMUXC_SW_MUX_CTL_PAD_UART2_RX_DATA = 0x00000008;  // 设置为 ECSPI3 SCLK
    *IOMUXC_SW_MUX_CTL_PAD_UART2_CTS_B   = 0x00000008;  // 设置为 ECSPI3 MOSI
    *IOMUXC_SW_MUX_CTL_PAD_UART2_RTS_B   = 0x00000008;  // 设置为 ECSPI3 MISO

    // 配置选择输入寄存器
    *IOMUXC_ECSPI3_SS0_B_SELECT_INPUT = 0x00000000;  // 设置为 ECSPI3 SS0
    *IOMUXC_ECSPI3_SCLK_SELECT_INPUT = 0x00000000;   // 设置为 ECSPI3 SCLK
    *IOMUXC_ECSPI3_MOSI_SELECT_INPUT = 0x00000000;   // 设置为 ECSPI3 MOSI
    *IOMUXC_ECSPI3_MISO_SELECT_INPUT = 0x00000000;   // 设置为 ECSPI3 MISO

    // 参数配置
    // 配置 SS0 引脚为 GPIO 输出，并设置高电平
    *GPIO_DIR |= (1 << 20);  // 设置为输出
    *GPIO_DR |= (1 << 20);  // 设置高电平   
    
    // 关闭 SPI 以进行配置，清空控制寄存器
    spi_base->ECSPIx_CONREG = 0x00000000;
    /**
     *  [31:20]：设置burst length。本次一次传8位数据，设置为7。
     *  [19:18]: 通道选择，SS0 选择通道0，设置为0。
     *  [17:16]: SPI Ready Control，本次不使用，设置为0。
     *  [15:8]: 时钟分频，后续设置。
     *  [7:4]: 通道模式，选择主机模式，设置为1
     *  [3]: 开始模式：1TXFIFO有数据立刻开始传输，设置为1。
     *  ...
     *  [0]: 启动 SPI，1-启动
     */
    spi_base->ECSPIx_CONREG = (7 << 20) | (1 << 4) | (1 << 3) | (1 << 0);

    /**
     * SCLK空闲状态（低电平），极性（0），相位（0），数据线空闲高电平
     */
    spi_base->ECSPIx_CONFIGREG = 0x00000000;

    // 时钟和使能配置
    // 输入时钟：60MHz, 目标时钟：4MHz, 分频系数 = 15
    // 清空控制寄存器的分屏位
    spi_base->ECSPIx_CONREG &= ~((0xF << 12) | (0xF << 8));
    // 设置分频系数
    spi_base->ECSPIx_CONREG |= ((15 - 1) << 12);  // 设置为15分频

    // 中断和 DMA 配置（本例中不使用中断和 DMA）
    return 0;  // 成功
}

/**
 * @brief 通过 SPI 读写数据
 * @param [in] SPI_Register* spi_base SPI 基地址
 * @param [in] uint8_t* tx_data 发送数据缓冲区
 * @param [in] uint32_t time_out 超时时间
 * @return uint8_t 接收数据
 */
uint8_t SPI_ReadWriteData(SPI_Register* spi_base, uint8_t tx_data, uint32_t time_out)
{
    // 硬件片选拉低
    spi_base->ECSPIx_CONREG &= ~(3 << 18);  // 选择通道0，SS0拉低
    spi_base->ECSPIx_CONREG |= (0 << 18);   // 选择通道0，SS0拉低

    while (0 == (spi_base->ECSPIx_STATREG & (1 << 0))); // 等待 TX FIFO 非满
    spi_base->ECSPIx_TXDATA = tx_data;  // 发送数据

    while (0 == ((spi_base->ECSPIx_STATREG >> 3) & 0x1)){
        if (time_out-- == 0) {
            // 超时处理
            return 0;  // 返回0表示超时
        }
    } // 等待 RX FIFO 非空
    return (uint8_t)(spi_base->ECSPIx_RXDATA & 0xFF);  // 读取接收数据
}

/**
 * @brief 软件片选拉高
 * @param void
 * @return void
 */
void SPI_SS_High(void) {
    *(volatile uint32_t*)GPIO_DR |= (1 << 20);  // 设置高电平
}

/**
 * @brief 软件片选拉低
 * @param void
 * @return void
 */
void SPI_SS_Low(void) {
    *(volatile uint32_t*)GPIO_DR &= ~(1 << 20);  // 设置低电平
}

unsigned char SPI_TESTRW(SPI_Register *spi_num)
{
	/*
		*enable loopback test
		*transmitter and receiver sections internally connected for loopback
	*/
	unsigned char uc_cnt=0;
	unsigned char uc_buf_write[20]={0};
	unsigned char uc_buf_read[20]={0};
	//设置进入loop模式，进行测试
	spi_num->ECSPIx_TESTREG = (1<<31);
	printf("spi进入回环测试模式\n\r");	 
	//造数
	for(uc_cnt=0;uc_cnt<20;uc_cnt++)
	{
		uc_buf_write[uc_cnt] = 0x20+uc_cnt;
	}
	//进行读写测试
	for(uc_cnt=0;uc_cnt<20;uc_cnt++)
	{
		printf("write_cnt %d\t",uc_cnt);	
		uc_buf_read[uc_cnt]=SPI_ReadWriteData(spi_num,uc_buf_write[uc_cnt], 1000);
		printf("write %d\t",uc_buf_write[uc_cnt]);	
		printf("read %d\n\r",uc_buf_read[uc_cnt]);	
	}
	//进行数据对比
	for(uc_cnt=0;uc_cnt<20;uc_cnt++)
	{
		if(uc_buf_read[uc_cnt]!=uc_buf_write[uc_cnt])
		{/*表示回环测试失败，存在问题*/
			printf("！！！spi回环测试失败\n\r");			
			return -1;
		}
	}
	printf("@@@spi回环测试成功\n\r");
	printf("spi退出回环测试模式\n\r");
		//exit loopback mode
	spi_num->ECSPIx_TESTREG = 0;
	return 0;
}