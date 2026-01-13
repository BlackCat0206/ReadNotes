#ifndef GIC_CUSTOM_H
#define GIC_CUSTOM_H

#include "common.h"

typedef void (*irqCustom_handler_t)(uint32_t nr, void* param);

typedef struct{
    irqCustom_handler_t handler;
    void *param;
}irqCustom_table_t;

/**
 * @brief GIC初始化
 * @param void
 * @return void
 * @details
 * 获取GIC基地址后，进行以下初始化操作：
 * 1. 获取GIC最大支持中断ID。
 * 2. 禁用所有PPI, SGI和SPI中断。
 */
void gicCustom_Init(void);

/**
 * @brief 中断使能函数
 * @param IRQn_Type nr 中断号
 * @return void
 */
void gicCustom_EnableIRQ(IRQn_Type nr);

/**
 * @brief 中断禁用函数
 * @param IRQn_Type nr 中断号
 * @return void
 */
void gicCustom_DisableIRQ(IRQn_Type nr);

/**
 * @brief 注册自定义中断处理函数
 * @param IRQn_Type irq 中断号
 * @param irqCustom_handler_t handler 中断处理函数指针
 * @param void *param 用户参数
 * @return void
 */
void gicCustom_RegisterIRQHandler(IRQn_Type irq, irqCustom_handler_t handler, void *param);

/**
 * @brief 初始化自定义中断处理函数表
 * @param void
 * @return void
 */
void gicCustom_InitIRQTable(void);

void gic_init(void);

#endif /* GIC_CUSTOM_H */