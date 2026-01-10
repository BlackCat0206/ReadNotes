#ifndef _OLED_CUSTOM_H_
#define _OLED_CUSTOM_H_

/***
 * @brief OLED 初始化函数
 * @param void
 * @return 0 成功，其他值失败
 */
int OLED_Init(void);

/***
 * @brief OLED 清屏函数
 * @param void
 * @return void
 */
void OLED_ClearScreen(void);

/**
 * @brief 测试 OLED 显示函数
 * @param void
 */
void OLED_Test(void);

#endif //_OLED_CUSTOM_H_