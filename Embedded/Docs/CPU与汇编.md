# CPU与汇编

---- Chime Lee

## 一、概述

CPU与汇编相关知识。

## 二、CPU

* CPU的9种运行模式：User为非特权模式，其余8种都是特权模式。

* 9种模式可以通过“软件中断或异常”自动，从**User**（用户模式）切换到**Supervisor(SVC)**（超级管理员模式）
* 大多数程序运行在User模式，是不能访问系统所有资源的。
* **Linux系统调用（*system call*）**：用户空间请求内核服务接口。其通过**软件中断**的方式，陷入内核的，CPU**自动切换到SVC模式**。
* **软件中断**：系统调用陷入内核模式。
* **硬件中断**：控制器相对于CPU的中断。

**寄存器简介：**

* R0~R7：通用寄存器
* sp（*statck pointer*）：栈顶指针，保存各自栈的**栈顶地址**。
* lr（*link register*）：链接寄存器，保存**函数的返回地址**。
* pc（*programe conut*）：程序计数器，保存当**前将要执行的那条指令的地址**。
* cpsr（*current programe status register*）：当前程序状态寄存器，**完成状态查询**和**功能控制**。
* spsr（*stored programe status register*）：备份程序状态寄存器。
  * 当特定中断发生时，SPSR寄存器用来保存当前当前程序状态寄存器（CPSR）的值，当异常退出以后，可以用SPSR中保存的值来恢复SPSR。
  * 注意：寄存器 
    * 第6位 F （快速中断）
      * cpsie i (change processor status to enable)
      * cpsid i (change processor status to disable)
    * 第7位 I (一般中断)
      * cpsie i
      * cpsid i

## 三、汇编示例

```bash
	
# .text 伪指令，每一行代码将被编程成地址，表示当前的地址。.text 表示程序代码区开始了。
.text
# .global 表示指定了一个叫_start的标号，是全局标号。（既其他汇编文件也可以使用此标号——就是一行的标记类似goto语句的lable）
.global  _start, _vector_table
# start: 标号，表示代码位置的命令
_start:  				

	#将地址值0x80200000赋值给栈指针了。 0x80200000~0x80000000 = 2 * 2^10 * 2^10 = 2 * 1 MB = 2MB
	# ldr load register
	ldr  sp,=0x80200000

	# branch and link 跳转并且链接，意思是跳转至clean_bss，并将下一条的代码的地址放在lr寄存器中
	bl clean_bss
	
	bl main

_vector_table:
	ldr 	pc, =Reset_Handler			 /* Reset				   */
	

Reset_Handler:
	/* Reset SCTlr Settings */
	mrc 	p15, 0, r0, c1, c0, 0	  /* read SCTRL, Read CP15 System Control register		*/
	bic 	r0,  r0, #(0x1 << 13)	  /* Clear V bit 13 to use normal exception vectors  	*/
	bic 	r0,  r0, #(0x1 << 12)	  /* Clear I bit 12 to disable I Cache					*/
	bic 	r0,  r0, #(0x1 <<  2)	  /* Clear C bit  2 to disable D Cache					*/
	bic 	r0,  r0, #(0x1 << 2)	  /* Clear A bit  1 to disable strict alignment 		*/
	bic 	r0,  r0, #(0x1 << 11)	  /* Clear Z bit 11 to disable branch prediction		*/
	bic 	r0,  r0, #0x1			  /* Clear M bit  0 to disable MMU						*/
	mcr 	p15, 0, r0, c1, c0, 0	  /* write SCTRL, Write to CP15 System Control register	*/
	bl clean_bss
	
	bl system_init
/*	cpsie	i					 /* Unmask interrupts			  */
	bl main

halt:
	b  halt

halt:
	b  halt 

clean_bss:
	/* ���BSS�� */
	ldr r1, =__bss_start
	ldr r2, =__bss_end
	mov r3, #0
clean:
	str r3, [r1] # store to register 将r3的值存到r1指向的内存地址中
	# 每次加4个字节
	add r1, r1, #4 
	# 比较r1和r2的值
	cmp r1, r2
	# branch if not equal 如果不相等则继续清零
	bne clean
	
	# 返回到lr寄存器中保存的地址
	mov pc, lr

```

## 链接描述脚本(*.lds)

```bash
SECTIONS {
    # 设置程序的起始地址
    . = 0x80100000;

    # 对齐到4字节边界
    . = ALIGN(4);

    # 定义.text段，存放代码
    .text      :
    {
      *(.text)
    }

    # 定义.rodata段，存放只读数据
    . = ALIGN(4);
    .rodata : { *(.rodata) }

    # 定义.data段，存放已初始化的数据
    . = ALIGN(4);
    .data : { *(.data) }

    # 定义.bss段，存放未初始化的数据
    . = ALIGN(4);
    __bss_start = .;
    .bss : { *(.bss) *(.COMMON) }
    __bss_end = .;
}
```

* 功能：将各个文件链接到一起。

## 段和分区的关系

段：二进制文件中描述

分区：程序加载到内存中的描述