/***
 * @brief OLED 自定义裸机驱动头文件
 * @author Chime Lee
 * @date 2026-01-09
 * @version 1.0
 * @details
 * 
 * @note
 * SPI发送给OLED的信息可以是命令，也可以是数据。（DC引脚高电平：数据,低电平：命令）
 */ 

#include "font.h"
#include "SPI_Custom.h"
#include "oled_Custom.h"

#define OLED_DEBUG 0

#if OLED_DEBUG
#endif

typedef enum{
    OLED_CMD = 0,
    OLED_DATA = 1
}OLED_OPERATION;

volatile static uint32_t* IOMUXC_SW_MUX_CTL_PAD_CSI_HSYNC = (volatile uint32_t*)0x020E01E0;  // GPIO4_20 多路复用器寄存器

volatile static uint32_t* GPIO4_DR;  // GPIO4 数据寄存器
volatile static uint32_t* GPIO4_DIR;   // GPIO4 方向寄存器

void GPIO4_20_High(void) {
    *(volatile uint32_t*)GPIO4_DR |= (1 << 20);  // 设置高电平
}

void GPIO4_20_Low(void) {
    *(volatile uint32_t*)GPIO4_DR &= ~(1 << 20);  // 设置低电平
}

/**
 * @brief OLED 发送命令/数据
 * @param uint8_t cmd_data 命令或数据
 * @param uint8_t is_data 标志，1表示数据，0表示命令
 * @return void
 */
static void OLED_SendCmdData(uint8_t cmd_data, uint8_t is_data) {
    if (OLED_DATA == is_data) {
        GPIO4_20_High();  // 数据
    } else {
        GPIO4_20_Low();   // 命令
    }

	// spi_writeread(ESCPI1_BASE, cmd_data);  // 发送数据

    SPI_ReadWriteData(SPI1, cmd_data, 1000);  // 发送数据

	return;
}

/***
 * @brief OLED 初始化函数
 * @param void
 * @return 0 成功，其他值失败
 */
int OLED_Init(void) {
	SPI1_Init(SPI1);      // 初始化 SPI1 接口
	GPIO4_DR = (volatile uint32_t*)0x020A8000;
	GPIO4_DIR = (volatile uint32_t*)0x020A8004; 
	OLED_SendCmdData(0xae,OLED_CMD);//关闭显示

	OLED_SendCmdData(0x00,OLED_CMD);//设置 lower column address
	OLED_SendCmdData(0x10,OLED_CMD);//设置 higher column address

	OLED_SendCmdData(0x40,OLED_CMD);//设置 display start line

	OLED_SendCmdData(0xB0,OLED_CMD);//设置page address

	OLED_SendCmdData(0x81,OLED_CMD);// contract control
	OLED_SendCmdData(0x66,OLED_CMD);//128

	OLED_SendCmdData(0xa1,OLED_CMD);//设置 segment remap

	OLED_SendCmdData(0xa6,OLED_CMD);//normal /reverse

	OLED_SendCmdData(0xa8,OLED_CMD);//multiple ratio
	OLED_SendCmdData(0x3f,OLED_CMD);//duty = 1/64

	OLED_SendCmdData(0xc8,OLED_CMD);//com scan direction

	OLED_SendCmdData(0xd3,OLED_CMD);//set displat offset
	OLED_SendCmdData(0x00,OLED_CMD);//

	OLED_SendCmdData(0xd5,OLED_CMD);//set osc division
	OLED_SendCmdData(0x80,OLED_CMD);//

	OLED_SendCmdData(0xd9,OLED_CMD);//ser pre-charge period
	OLED_SendCmdData(0x1f,OLED_CMD);//

	OLED_SendCmdData(0xda,OLED_CMD);//set com pins
	OLED_SendCmdData(0x12,OLED_CMD);//

	OLED_SendCmdData(0xdb,OLED_CMD);//set vcomh
	OLED_SendCmdData(0x30,OLED_CMD);//

	OLED_SendCmdData(0x8d,OLED_CMD);//set charge pump disable 
	OLED_SendCmdData(0x14,OLED_CMD);//

	OLED_SendCmdData(0xaf,OLED_CMD);//set dispkay on
    return 0;
}

/**
 * @brief OLED 确认页地址和列地址函数
 * @param int page 页地址 0~7
 * @param int column 列数据
 * @note
 * 列数据要发送两次，低四位和高四位 高四位的格式是 0x10 + 高四位数据
 * @return void
 */
void OLED_SetPageAndColumn(int page, int column) {
	OLED_SendCmdData(0xb0 + page, OLED_CMD);               // 设置页地址
	OLED_SendCmdData((column & 0x0F), OLED_CMD);    // 设置列地址低四位
	OLED_SendCmdData((0x10 | ((column >> 4) & 0x0F)), OLED_CMD); // 设置列地址高四位
	return;
}

/***
 * @brief OLED 清屏函数
 * @param void
 * @return void
 */
void OLED_ClearScreen(void) {
	for (int page = 0; page < 8; page++) {
		OLED_SetPageAndColumn(page, 0);  // 设置页地址和列地址
		for (int col = 0; col < 128; col++) {
			OLED_SendCmdData(0x00, OLED_DATA);  // 清除数据
		}
	}
	return;
}

/**
 * @brief OLED 显示字符函数
 * @param int page 页地址 0~7
 * @param int column 列地址 0~127
 * @param char ch 要显示的字符
 * @note
 * 1. 使用 16x8 点阵显示字符
 * 2. 每个字符占用 16 行，8 列
 * @return void
 */
void OLED_ShowChar(int page, int column, unsigned char ch)
{
	int i = 0;
	// 获取对应的字模
	const unsigned char* font = oled_asc2_8x16[ch - ' '];

	// 显示上半部分
	OLED_SetPageAndColumn(page, column);
	for (i = 0; i < 8; i++) {
		OLED_SendCmdData(font[i], OLED_DATA);
	}

	// 显示下半部分
	OLED_SetPageAndColumn(page + 1, column);
	for (i = 8; i < 16; i++) {
		OLED_SendCmdData(font[i], OLED_DATA);
	}
	return;
}

/**
 * @brief OLED 显示字符串函数
 * @param int page 页地址 0~7
 * @param int column 列地址 0~127
 * @param char* str 要显示的字符串
 * @return void
 */
void OLED_ShowString(int page, int column, const char* str)
{
	int j = 0;
	while (str[j])
	{
		OLED_ShowChar(page, column, str[j]);
		column += 8;
		if (column >= 128) // 换行处理
		{
			column = 0;
			page += 2; // 每个字符占用两页
			if (page >= 8) // 超出屏幕范围，停止显示
			{
				break;
			}
		}
		j++;
	}
}

/***
 * @brief OLED 显示汉字函数
 * @param int page 页地址 0~7
 * @param int column 列地址 0~127
 * @param uint8_t ch 汉字点阵数据下标
 * @return void
 */
void OLED_ShowChinese(int page, int column, const uint8_t ch)
{
	int i = 0;
	// 显示上半部分
	OLED_SetPageAndColumn(page, column);
	for (i = 0; i < 16; i++) {
		OLED_SendCmdData(hz_1616[ch][i * 2], OLED_DATA);
	}

	// 显示下半部分
	OLED_SetPageAndColumn(page + 1, column);
	for (i = 0; i < 16; i++) {
		OLED_SendCmdData(hz_1616[ch][i * 2 + 1], OLED_DATA);
	}
	return;
}

/**
 * @brief 测试 OLED 显示函数
 * @param void
 */
void OLED_Test(void) {
	OLED_ClearScreen();
	OLED_ShowString(0, 0, "Hello, OLED!");
	OLED_ShowString(2, 0, "Li Jinsheng");
	OLED_ShowString(4, 0, "SPI OLED TEST");

	for (int i = 0; i < 3; i++) {
		OLED_ShowChinese(6, i * 32, i);  // 显示汉字 "百问网"
	}
	return;
}