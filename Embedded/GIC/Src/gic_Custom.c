/***
 * @brief gic_Custom.c
 * @author Chime Lee
 * @version v1.0
 * @date 2026-01-12
 * @details Customized GIC driver implementation
 * @note：
 * Linux下的GIC驱动代码参考自Linux内核源码，经过简化和修改以适应本项目需求。
 */

#include "gic_Custom.h"
#include "my_printf.h"

static irqCustom_table_t irqCustom_table[160];  // 自定义中断处理函数表

/**
 * @brief GIC CPU获取基地址
 * @param void
 * @return GIC_Type* GIC基地址指针
 * @details
 * 使用协处理器命令：MRC p15, 4, <Rt>, c15, c0, 0 获取GIC基地址。
 */
GIC_Type * gicCustom_GetGICCPUBase(void){
    GIC_Type *dst;

    // 执行协处理器命令, 使用汇编指令"=r"赋值给dst
    __asm volatile ("mrc p15, 4, %0, c15, c0, 0" : "=r" (dst)); 

    return dst;
}

/**
 * @brief GIC初始化
 * @param void
 * @return void
 * @details
 * 获取GIC基地址后，进行以下初始化操作：
 * 1. 获取GIC最大支持中断ID。
 * 2. 禁用所有PPI, SGI和SPI中断。
 * 3. 设置优先级掩码级别：小于该值的中断将被GIC CPU转发给处理器。
 * 4. 设置无子优先级，所有优先级级别允许抢占
 * 5. 使能分发器将挂起的中断转发到CPU接口，启用group0分发。
 * 6. 使能CPU接口向连接的处理器发出中断信号，启用group0信号。
 */
void gicCustom_Init(void){
    GIC_Type* p_gic = gicCustom_GetGICCPUBase();

    // 获取GIC最大支持中断ID
    uint32_t irq_num = (p_gic->D_TYPER & 0x1F) + 1;

    // 禁用所有PPI, SGI和SPI中断
    for(uint32_t i = 0; i < irq_num; i++){
        p_gic->D_ICENABLER[i] = 0xFFFFFFFFUL;
    }

    // 设置优先级掩码级别：小于该值的中断将被GIC CPU转发给处理器
    p_gic->C_PMR = (0xFFUL << (8 - 5)) & 0xFFUL;    // 248

    // 设置无子优先级，所有优先级级别允许抢占
    p_gic->C_BPR = 7 - 5;

    // 使能分发器将挂起的中断转发到CPU接口，启用group0分发
    p_gic->D_CTLR = 1UL;

    // 使能CPU接口向连接的处理器发出中断信号，启用group0信号
    p_gic->C_CTLR = 1UL;
}

/**
 * @brief 中断默认处理函数
 * @param uint32_t nr 中断号
 * @param void *param 用户参数
 * @return void
 */
void gicCustom_DefaultHandler(uint32_t nr, void *param){
    printf("Unhandled IRQ: %d\n", nr);
}

/**
 * @brief 注册自定义中断处理函数
 * @param IRQn_Type irq 中断号
 * @param irqCustom_handler_t handler 中断处理函数指针
 * @param void *param 用户参数
 * @return void
 */
void gicCustom_RegisterIRQHandler(IRQn_Type irq, irqCustom_handler_t handler, void *param){
    if(irq < 160){
        irqCustom_table[irq].handler = handler;
        irqCustom_table[irq].param = param;
    }
}

/**
 * @brief 初始化自定义中断处理函数表
 * @param void
 * @return void
 */
void gicCustom_InitIRQTable(void){
    for(uint32_t i = 0; i < 160; i++){
        irqCustom_table[i].handler = gicCustom_DefaultHandler;
        irqCustom_table[i].param = NULL;
    }
}

/**
 * @brief 自定义中断处理函数
 * @param void
 * @return void
 * @details
 * 1. 获取GIC基地址。
 * 2. 读取当前活跃中断的ID。
 * 3. 调用注册的中断处理函数。
 * 4. 将中断号写入中断结束寄存器，通知GIC中断处理完成。
 */
void gicCustom_IRQHandler(void)
{
    int nr;

    GIC_Type* p_gic = gicCustom_GetGICCPUBase();

    // 读取当前活跃中断的ID
    nr = p_gic->C_IAR;
    printf("Custom IRQ Handler: IRQ %d triggered\n", nr);

    // 调用注册的中断处理函数
    if(nr < 160){
        irqCustom_table[nr].handler(nr, irqCustom_table[nr].param);
    }

    // 将中断号写入中断结束寄存器，通知GIC中断处理完成
    p_gic->C_EOIR = nr;
}

/**
 * @brief 中断使能函数
 * @param IRQn_Type nr 中断号
 * @return void
 */
void gicCustom_EnableIRQ(IRQn_Type nr){
    GIC_Type *gic = gicCustom_GetGICCPUBase();

    // 打开分配器的对应中断使能位
    gic->D_ISENABLER[nr >> 5] = (uint32_t)(1UL << (nr & 0x1FUL));
}

/**
 * @brief 中断禁用函数
 * @param IRQn_Type nr 中断号
 * @return void
 */
void gicCustom_DisableIRQ(IRQn_Type nr){
    GIC_Type *gic = gicCustom_GetGICCPUBase();

    // 关闭分配器的对应中断使能位
    gic->D_ICENABLER[nr >> 5] = (uint32_t)(1UL << (nr & 0x1FUL));
}