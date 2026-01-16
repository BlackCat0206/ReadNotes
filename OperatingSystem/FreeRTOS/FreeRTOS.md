# FreeRTOS

---- Chime Lee

## 一、概述

**与裸机的区别：**

`FreeRTOS`在裸机的基础上提供了：

* 任务：线程的处理
* 队列：线程间通信方法
* 事件：线程间同步机制
* 定时处理任务等`API`函数，

方便我们处理并行的裸机程序。

**执行模式**：`FreeRTOS`通过**时间片轮转**和**优先级抢占**的调度机制。

**编译**：

```bash
#编译.imx文件
mkimage -n ./tools/imximage.cfg.cfgtmp -T imximage -e 0x80002000 -d sdk20-app.bin led.imx
```

## 二、任务函数解析

**创建任务的核心函数：**

```cpp
BaseType_t xTaskCreate(
    TaskFunction_t pvTaskCode,        // 任务函数指针
    const char * const pcName,        // 任务名称字符串
    configSTACK_DEPTH_TYPE usStackDepth, // 堆栈大小（字）1字 = 4字节
    void *pvParameters,              // 传递给任务的参数
    UBaseType_t uxPriority,          // 任务优先级：0为最低优先级
    TaskHandle_t *pxCreatedTask      // 任务句柄指针（输出）
);

// 任务函数
void vTaskFunction(void *pvParameters);
```

**任务调度**：

* 调用都器会选择**最高优先级的就绪任务**，这是**抢占式调度**。
* 如果多个**任务同优先级**，使用**时间片轮转调度**，这是**时间片轮转**。

```c
// FreeRTOS的调度启动函数，整个RTOS系统的核心入口
vTaskStartScheduler();
```

**任务删除**：

* 释放任务控制块内存，栈内存
* 从任务就绪列表，阻塞列表等中删除。

```c
// 当前任务删除自己
vTaskDelete(NuLL);
// 删除指定句柄的任务
vTaskDelete(taskHandle);
```

**任务挂起**：

* **所有的`FromeISR`函数，均需要在中断函数中使用**。如果不使用这类函数，程序会卡死。
* 在`FreeRTOS`**中断函数处理函数**中，**不能休眠、不能阻塞、不能进程间切换**。

```c
vTaskSuspend;
vTaskResume;

BaseType_t xTaskResumeFromISR(TaskHandler_t xTaskResume);

// 任务阻塞函数
vTaskDelay;

// 任务调用 vTaskDelay（）
// 任务从Running->Block状态
// 调度器选择其他就绪任务运行
// 延时结束后，任务从Blcok->Ready状态
// 调度器可能在下次调度时恢复该任务
```

**任务同步机制：信号量（*Semaphore*）**

* 计数信号量：

```c
xSemaphoreCreateCounting();
```

* 二值信号量：

```c
xSemaphoreCreateBinary();	// 二值信号量创建
```

* 释放信号量：

```c
xSemaphoreGive(xSemaphore);	// 释放信号量，信号+1
```

* 等待信号量：

```c
xSemaphoreTake(xSemaphore, portMax_DELAY);	// 等待信号量
```

**任务同步机制：互斥量（*Mutex*）**

* 互斥锁创建：

```c
xSemaphoreCreateMutex();
```

* 递归互斥锁创建：

```c
xSemaphoreCreateRecursiveMutex();
```

* 使用实例：

```c
void task1(void *pvParameters) {
    xSemaphoreTake(xMutex, portMAX_DELAY);  // 获取锁
    task1(pvParameters);像这种必须用递归锁，否则会死锁。
    xSemaphoreGive(xMutex);                 // 释放锁
}
```

**任务的状态：运行态、就绪态、阻塞态、挂起态、删除态**

```c
typedef enum
{
	eRunning = 0,	/* A task is querying the state of itself, so must be running. */
	eReady,		/* The task being queried is in a read or pending ready list. */
	eBlocked,	/* The task being queried is in the Blocked state. */
	eSuspended,	/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
  	eDeleted,	/* The task being queried has been deleted, but its 
                                             TCB has not yet been freed. */

	eInvalid		/* Used as an 'invalid state' value. */
} eTaskState;

```

* 就绪态：就绪态->运行态，
* 运行态：运行态->阻塞态、运行态->挂起态、运行态->删除态
* 阻塞态：阻塞态->就绪态
* 挂起态：挂起态->就绪态
* 删除态不能到其他状态，只有运行态结束，才能到删除态

## 三、队列

`FreeRTOS`队列是**任务间**、**任务与中断**间进行数据传递的**通信机制**，提供**线程安全**、**阻塞式**的数据缓冲区。

```c
QueueHandle_t xQueue = xQueueCreate(
    10,         // 队列能存储的最大项目数
    sizeof(int) // 每个项目的大小（字节）
);

xQueueSend(xQueue, &data, portMAX_DELAY);  // 发送数据
xQueueReceive(xQueue, &receivedData, portMAX_DELAY);  // 接收数据

// 覆盖发送（队列满时覆盖最旧数据）
BaseType_t xQueueOverwrite(...);
// 中断安全版本
BaseType_t xQueueSendFromISR(...);
BaseType_t xQueueReceiveFromISR(...);
可以传递任何数据类型
```

## 四、`freeRTOSconfig.h`

项目级别文件，用于**裁剪、硬件参数适配和内存**等参数配置。

**核心参数配置**：

* configUSE_PREEMPTION ：是否应用抢占式调度，
  *  1：使用抢占式调度。高优先级任务，立即抢占低优先级任务。                                    
  *  0：任务会一直运行，直到它主动调用 taskYIELD() 之类的函数放。
* configUSE_TIME_SLICING
  * 1：开启。如果两个任务优先级相同，它们将共享 CPU 时间，每个任务运行一个时间片（configTICK_RATE_HZ 定义的时间）后切换。
  *  0：关闭。同优先级任务不会自动切换，必须等运行中的任务阻塞（如调用 vTaskDelay）或主动放弃 CPU。
* **configMINIMAL_STACK_SIZE** （高频面试问题）
  * 空闲任务和定时器这两个任务需要的栈的大小。
  * 如何计算：考虑任务回调函数的形参和局部变量的占用空间，任务一旦被打断，那么`R0~15`也要压到栈中。**所以最少16个，安全一点，配置128单位字word。（32位系统下，就是4个字节）**。
* **configUSE_16_BIT_TICKS**：定义 TickType_t 的大小的
  * TickType_t 就是32位的。
  * TickType_t 就是16 位的。最大的数值范围就不同了。
* configIDLE_SHOULD_YIELD ：决定空闲任务（IDLE任务）是否会主动让出CPU给其他同等优先级的任务
  * 0：空闲任务 不让路
  * 1：空闲任务，让路
* configUSE_TASK_NOTIFICATIONS ：轻量级的任务通知，用于任务间的通讯。

**功能配置**：

* configUSE_MUTEX
  * 1：启用
  * 2：禁用，节省少量的代码空间和内存。
* configUSE_RECURSIVE_MUTEXES：一个递归函数要用的锁，要使用递归锁。
  * 原理：**如果获取锁的是当前持有它的任务：计数器+1，函数立即成功返回**。如果获取锁的是其他任务：任务阻塞（与普通互斥锁行为一致）。
  * 作用：节省少量的代码空间和内存。
* configUSE_COUNTING_SEMAPHORES：是否使用计数信号量
* configQUEUE_REGISTRY_SIZE：队列注册表大小
* configUSE_QUEUE_SETS：如何让一个任务同时等待多个不同的事件
* configUSE_NEWLIB_REENTRANT： 绝大多数情况下，如果你在 FreeRTOS 中使用 Newlib 并且有多个任务调用标准库函数，你必须将其设置为 1。
* configNUM_THREAD_LOCAL_STORAGE_POINTERS：**任务本地存储指针数量**。
* configUSE_APPLICATION_TASK_TAG：为任务取别名，通过各种方式快速识别出那个任务。

**系统辅助配置**：

* configAPPLICATION_ALLOCATED_HEAP：
  *  0：由 FreeRTOS 自动分配堆内存。编译器会在 .bss 段中自动预留一块大小为 configTOTAL_HEAP_SIZE 的空间作为堆。这是最常见、最简单的用法。        
  * 1：由应用程序开发者手动分配堆内存。你需要自己定义一个数组（通常是全局的）作为 FreeRTOS 的堆，并通过 configTOTAL_HEAP_SIZE 告诉内核这个数组有多大。
* configUSE_IDLE_HOOK：是否需要空闲时的回调函数
  * 1：启用空闲任务钩子函数。你需要自己实现一个名为 
    `vApplicationIdleHook() `的函数。当系统进入空闲状态时，FreeRTOS 会自动在空闲任务的上下文中调用这个函数。
  * 0：禁用空闲任务钩子函数。这是默认值，因为大多数应用不需要它。禁用后，内核不会尝试调用这个不存在的函数，可以节省极少的执行开销。

* configUSE_TICK_HOOK：它允许你在每个RTOS系统时钟节拍（Tick）的中断中，插入一段你自己的代码来执行
  * 1：启用
  * 0：禁用
* configCHECK_FOR_STACK_OVERFLOW：是否检查堆栈溢出。

* configUSE_TRACE_FACILITY： 是否使用跟踪工具。用于调试。
* configUSE_STATS_FORMATTING_FUNCTIONS：系统统计信息格式化函数是否启用。

**定时器**：

* configUSE_TIMERS：软件定时器功能是否启用
  * 设为 `1`：启用软件定时器功能，FreeRTOS 会创建一个**定时器服务任务（Timer Service Task）**，专门管理所有软件定时器的创建、启动、停止和回调执行；
* configTIMER_QUEUE_LENGTH：软件定时器命令队列长度的重要参数 
* configTIMER_TASK_PRIORITY：定时器服务任务的优先级
  * 建议：configMAX_PRIORITIES - 1
* configTIMER_TASK_STACK_DEPTH：定时器服务任务的栈大小

## 五、堆内存管理方式

每个`heap_x.c`文件都实现了以下两个函数，这是它们对外的统一接口。

```c
void* pvPortMalloc(size_t xWantedSize);	// 分配内存
void  vPortFree(void* pv);				// 释放内存
```

**五种内存管理方案**：

| 方案       | 特点                                            | 碎片化 | 确定性 | 内存使用 | 使用场景         |
| ---------- | ----------------------------------------------- | ------ | ------ | -------- | ---------------- |
| `heap_1.c` | 只分配不释放                                    | 无     | 是     | 静态     | 简单的嵌入式项目 |
| `heap_2.c` | 可分配释放，使用最**佳匹配算法**                | 中等   | 否     | 较好     | 中小型项目       |
| `heap_3.c` | 封装标准库`malloc/free`，<br />增加线程安全保护 | 严重   | 否     | 一般     | PC/Unix移植      |
| `heap_4.c` | 可分配释放，释放时合并相邻空闲块（First Fit）   | 少     | 否     | 优秀     | 最常用           |
| `heap_5.c` | heap_4.c + 多不连续内存区域                     | 少     | 否     | 优先     | 复杂内存布局。   |

## 六、中断处理

路径：devices/MCIMX6Y2/system_MCIMX6Y2.hsystem_MCIMX6Y2.h

**中断指针类型**：

```c
typedef void (*system_irq_handler_t) (uint32_t giccIar, void *param); 
```

**中断指针结构体**：

```c
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler; /**< IRQ handler for specific IRQ */
    void *userParam;                 /**< User param for handler callback */
} sys_irq_handle_t; 
```

**配置中断号对应中断处理函数**：

```c
/*
* @brief 作用：给指定的中断号安装中断处理程序。
* @param [in] IRQn_Type irq ，中断号
* @param [in] handler ，接收中断处理函数 
* @param [in] 给中断处理函数传入的实参。
*/
void SystemInstallIrqHandler (IRQn_Type irq, system_irq_handler_t handler, void *userParam); 

// .c文件中具体实现：
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];//二级中断向量表。

void SystemInstallIrqHandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam) {
      irqTable[irq].irqHandler = handler;
      irqTable[irq].userParam = userParam;
}
```

**中断处理过程**：

类似于裸机

**GIC相关配置**：

* __IOM uint32_t D_ISENABLER[16]; /*!< 偏移: 0x1100 - 0x13C (读写) 中断使能设置寄存器 */
*  __IOM uint32_t D_ICENABLER[16]; /*!< 偏移: 0x1180 - 0x1BC (读写) 中断使能清除寄存器 */
* __IOM uint32_t C_PMR;  /*!< 偏移地址: 0x2004 (可读写) 中断优先级掩码寄存器 */
* __IM  uint32_t C_IAR;              /*!< 偏移地址: 0x200C (只读) 中断应答寄存器 */
* __OM  uint32_t C_EOIR;         /*!< 偏移地址: 0x2010 (只写) 中断结束寄存器 */

```c
typedef struct {        uint32_t RESERVED0[1024];                 /* 保留区域0 */
  __IOM uint32_t D_CTLR;                 /*!< 偏移: 0x1000 (读写) 分发器控制寄存器 */
  __IM  uint32_t D_TYPER;                /*!< 偏移: 0x1004 (只读) 中断控制器类型寄存器 */
  __IM  uint32_t D_IIDR;                 /*!< 偏移: 0x1008 (只读) 分发器实现者标识寄存器 */
        uint32_t RESERVED1[29];                  /* 保留区域1 */
  __IOM uint32_t D_IGROUPR[16];          /*!< 偏移: 0x1080 - 0x0BC (读写) 中断分组寄存器 */
        uint32_t RESERVED2[16];                  /* 保留区域2 */
  __IOM uint32_t D_ISENABLER[16];        /*!< 偏移: 0x1100 - 0x13C (读写) 中断使能设置寄存器 */
        uint32_t RESERVED3[16];                  /* 保留区域3 */
  __IOM uint32_t D_ICENABLER[16];        /*!< 偏移: 0x1180 - 0x1BC (读写) 中断使能清除寄存器 */
        uint32_t RESERVED4[16];                  /* 保留区域4 */
  __IOM uint32_t D_ISPENDR[16];          /*!< 偏移: 0x1200 - 0x23C (读写) 中断挂起设置寄存器 */
        uint32_t RESERVED5[16];                  /* 保留区域5 */
  __IOM uint32_t D_ICPENDR[16];          /*!< 偏移: 0x1280 - 0x2BC (读写) 中断挂起清除寄存器 */
        uint32_t RESERVED6[16];                  /* 保留区域6 */
  __IOM uint32_t D_ISACTIVER[16];        /*!< 偏移: 0x1300 - 0x33C (读写) 中断激活设置寄存器 */
        uint32_t RESERVED7[16];                  /* 保留区域7 */
  __IOM uint32_t D_ICACTIVER[16];        /*!< 偏移: 0x1380 - 0x3BC (读写) 中断激活清除寄存器 */ 
 ____IOM uint8_t  D_IPRIORITYR[512];      /*!< 偏移地址: 0x1400 - 0x5FC (可读写) 中断优先级寄存器数组 */
        uint32_t RESERVED9[128];                 /* 保留区域9 */
__IOM uint8_t  D_ITARGETSR[512];       /*!< 偏移地址: 0x1800 - 0x9FC (可读写) 中断目标寄存器数组 */
        uint32_t RESERVED10[128];                /* 保留区域10 */
__IOM uint32_t D_ICFGR[32];            /*!< 偏移地址: 0x1C00 - 0xC7C (可读写) 中断配置寄存器数组 */
        uint32_t RESERVED11[32];                 /* 保留区域11 */
__IM  uint32_t D_PPISR;                /*!< 偏移地址: 0x1D00 (只读) 私有外设中断状态寄存器 */
__IM  uint32_t D_SPISR[15];            /*!< 偏移地址: 0x1D04 - 0xD3C (只读) 共享外设中断状态寄存器数组 */
        uint32_t RESERVED12[112];                /* 保留区域12 */
__OM  uint32_t D_SGIR;                 /*!< 偏移地址: 0x1F00 (只写) 软件生成中断寄存器 */
        uint32_t RESERVED13[3];                  /* 保留区域13 */
__IOM uint8_t  D_CPENDSGIR[16];        /*!< 偏移地址: 0x1F10 - 0xF1C (可读写) SGI挂起清除寄存器数组 */
__IOM uint8_t  D_SPENDSGIR[16];        /*!< 偏移地址: 0x1F20 - 0xF2C (可读写) SGI挂起设置寄存器数组 */
        uint32_t RESERVED14[40];                 /* 保留区域14 */
__IM  uint32_t D_PIDR4;                /*!< 偏移地址: 0x1FD0 (只读) 外设ID4寄存器 */
__IM  uint32_t D_PIDR5;                /*!< 偏移地址: 0x1FD4 (只读) 外设ID5寄存器 */
__IM  uint32_t D_PIDR6;                /*!< 偏移地址: 0x1FD8 (只读) 外设ID6寄存器 */
__IM  uint32_t D_PIDR7;                /*!< 偏移地址: 0x1FDC (只读) 外设ID7寄存器 */
__IM  uint32_t D_PIDR0;                /*!< 偏移地址: 0x1FE0 (只读) 外设ID0寄存器 */
__IM  uint32_t D_PIDR1;                /*!< 偏移地址: 0x1FE4 (只读) 外设ID1寄存器 */
__IM  uint32_t D_PIDR2;                /*!< 偏移地址: 0x1FE8 (只读) 外设ID2寄存器 */
__IM  uint32_t D_PIDR3;                /*!< 偏移地址: 0x1FEC (只读) 外设ID3寄存器 */
__IM  uint32_t D_CIDR0;                /*!< 偏移地址: 0x1FF0 (只读) 组件ID0寄存器 */
__IM  uint32_t D_CIDR1;                /*!< 偏移地址: 0x1FF4 (只读) 组件ID1寄存器 */
__IM  uint32_t D_CIDR2;                /*!< 偏移地址: 0x1FF8 (只读) 组件ID2寄存器 */
__IM  uint32_t D_CIDR3;                /*!< 偏移地址: 0x1FFC (只读) 组件ID3寄存器 */
__IOM uint32_t C_CTLR;                 /*!< 偏移地址: 0x2000 (可读写) CPU接口控制寄存器 */
__IOM uint32_t C_PMR;                  /*!< 偏移地址: 0x2004 (可读写) 中断优先级掩码寄存器 */
__IOM uint32_t C_BPR;                  /*!< 偏移地址: 0x2008 (可读写) 二进制点寄存器 */
__IM  uint32_t C_IAR;                  /*!< 偏移地址: 0x200C (只读) 中断应答寄存器 */
__OM  uint32_t C_EOIR;                 /*!< 偏移地址: 0x2010 (只写) 中断结束寄存器 */
__IM  uint32_t C_RPR;                  /*!< 偏移地址: 0x2014 (只读) 运行优先级寄存器 */
__IM  uint32_t C_HPPIR;                /*!< 偏移地址: 0x2018 (只读) 最高优先级挂起中断寄存器 */
__IOM uint32_t C_ABPR;                 /*!< 偏移地址: 0x201C (可读写) 别名二进制点寄存器 */
__IM  uint32_t C_AIAR;                 /*!< 偏移地址: 0x2020 (只读) 别名中断应答寄存器 */
__OM  uint32_t C_AEOIR;                /*!< 偏移地址: 0x2024 (只写) 别名中断结束寄存器 */
__IM  uint32_t C_AHPPIR;               /*!< 偏移地址: 0x2028 (只读) 别名最高优先级挂起中断寄存器 */
        uint32_t RESERVED15[41];                 /* 保留区域15 */
__IOM uint32_t C_APR0;                 /*!< 偏移地址: 0x20D0 (可读写) 活动优先级寄存器 */
        uint32_t RESERVED16[3];                  /* 保留区域16 */
__IOM uint32_t C_NSAPR0;               /*!< 偏移地址: 0x20E0 (可读写) 非安全活动优先级寄存器 */
        uint32_t RESERVED17[6];                  /* 保留区域17 */
__IM  uint32_t C_IIDR;                 /*!< 偏移地址: 0x20FC (只读) CPU接口标识寄存器 */
        uint32_t RESERVED18[960];                /* 保留区域18 */
__OM  uint32_t C_DIR;                  /*!< 偏移地址: 0x3000 (只写) 停用中断寄存器 */
} GIC_Type;

```

## X、问题

### n.1 二值信号量和互斥体的区别

| 特性             | 二值信号量（Semaphore） | 互斥量（Mutex）        |
| ---------------- | ----------------------- | ---------------------- |
| **设计目的**     | 任务同步、事件通知      | 保护共享资源、防止竞争 |
| **优先级继承**   | 不支持                  | 支持（防止优先级反转） |
| **持有者该概念** | 无持有者                | 有持有者               |
| **释放要求**     | 任何任务都可释放        | 必须持有者释放         |
| **递归获取**     | 不支持                  | 支持                   |
| **初始化状态**   | 0                       | 1                      |
| **内存占用**     | 80字节                  | 80字节+继承开销        |

### n.2 什么是优先级反转：

三个任务（高优先级：Task_H，中优先级：Task_M，低优先级：Task_L）+ 一个共享资源R的场景：

* **低优先级先持有资源**：`TaskL`先执行，获取保护资源R的互斥锁，开始访问。
* **高优先级任务被唤醒抢占失败**：`TaskH`被外部事件（中断）唤醒，由于优先级高，**抢占CPU执行**。访问R时发现`R`的互斥锁被`TaskL`持有，因此进入**阻塞态**。
* **中优先级抢占低优先级任务**：`TaskM`此时被唤醒，其优先级高于`TaskL`，低于`TaskH`。由于`TaskH`处于阻塞态，**`TaskM`会抢占`TaskL`的CPU使用权**，执行自己的逻辑。
* **优先级反转现象出现**：优先级最高的`TaskH`无法执行，反而优先级中等的`TaskM`持续运行；低优先级`TaskL`被抢占，无法快速释放资源R，**导致`TaskH`阻塞时间被无限制延长**，破坏了实时性。

**解决方法：互斥锁优先级继承机制**

FreeRTOS中的**互斥锁（Mutex）**内置了**优先级继承（Priority Inheritance）**。

**优先级继承核心逻辑**：

当低优先级任务**`TaskL`持有互斥锁**，高优先级任务**`TaskH`因等待锁进入阻塞**：

* `FreeRTOS`会**临时将`TaskL`的优先级提升至`TaskH`的优先级**。
* 互斥锁释放后，`TaskL`的优先级**自动恢复到原来的低优先级**。

### n.3 什么是`systick`

例如：`168MHz`的主频 每秒跳动`168000000`。也就是当需要产生1000个系统的**中断（系统嘀嗒）**，**主频每16800就要产生一个中断（168000000 / 1000）**。

### n.4 `FreeRTOS`中断处理函数，编写的注意事项

* 永不阻塞阻塞、休眠、进程切换。
* 仅用`FromISR`：只使用中断安全的`FreeRTOS API`。
* 静态分配：避免任何形式的内存分配。
* 原子操作：共享数据访问要原子化。
* 清除标志：必须清除中断标志。
* 避免浮点：除非硬件支持。
* 简单处理：复杂工作交给任务。
* 可重入性：只调用可重入函数。
* 测试验证：验证最坏情况执行实现。

### n.5 `FreeRTOS`中断具体场景禁止事项表

| 类别     | 具体禁止              | 后果           | 安全方案                |
| -------- | --------------------- | -------------- | ----------------------- |
| 时间     | vTaskDelay(), sleep() | 系统死锁       | 使用任务通知、信号量    |
| 内存     | malloc()、free()      | 堆碎片化，死锁 | 静态预分配内存池        |
| I/O操作  | printf()，scanf()     | 阻塞、不可重入 | 队列传递到任务处理      |
| 复杂计算 | 浮点计算              | 中断响应延时   | 记录数据，任务中处理    |
| 同步语句 | 非`FromISR`版本API    | 未定义         | 仅使用`FromISR`         |
| 硬件访问 | 慢速外设操作          | 时序破坏       | 使用DMA或快速寄存器访问 |

