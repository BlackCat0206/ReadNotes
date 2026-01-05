/***
 * @file printf_Custom.c
 * @brief Custom printf implementation using UART.
 * @author Chime Lee
 * @date 2026-01-03
 * @version 1.0
 */


#include "printf_Custom.h"
#include "UART_Custom.h"

typedef char *  va_list;
// 将任意类型n的大小（sizeof(n)），向上取整到sizeof(int)的整数倍
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )  // (sizeof(n) + sizeof(int) - 1)向上取整，& ~(sizeof(int) - 1)对齐到sizeof(int)的整数倍
#define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )                    // 初始化ap指向第一个可变参数
#define va_arg(ap,t)    ( *(t *)( ap=ap + _INTSIZEOF(t), ap- _INTSIZEOF(t)) )   // 获取当前ap指向的参数，并将ap指向下一个参数
#define va_end(ap)      ( ap = (va_list)0 )                                     // 清空ap

unsigned char hex_tab[]={'0','1','2','3','4','5','6','7',\
		                 '8','9','a','b','c','d','e','f'};

static int outc(int c)
{
    UART_SendData(c);
    return 0;
}

static int outs(const char *s)
{
    while (*s != '\0')
        UART_SendData((int)(*s++));
    return 0;
}

/**
 * @brief 输出数字的辅助函数
 * @param n 要输出的数字
 * @param base 数字的进制（如10进制，16进制等）
 * @param lead 填充字符
 * @param maxwidth 输出的最小宽度
 * @return 返回输出的字符数
 */
static int out_num(long n, int base, char lead, int maxwidth)
{
    unsigned long m = 0;
    char buf[MAX_NUMBER_BYTES];
    char* s = buf + sizeof(buf); // 假设32字节足够存放数字字符串
    int count = 0, i = 0;

    *--s = '\0';    // 字符串结束符

    // 处理负数
    if (n < 0) {
        m = -n;
    } else {
        m = n;
    }

    // 转换数字为字符串
    do {
        *--s = hex_tab[m % base];
        count++;
    } while ((m /= base) != 0);

    // 处理宽度和填充字符
    if (maxwidth && count < maxwidth) {
        for (i = maxwidth - count; i; i--)
            *--s = lead;
    }

    // 处理负号
    if (n < 0)
        *--s = '-';

    return outs(s);
}

static int my_vprintf(const char *fmt, va_list ap)
{
    char lead = ' ';
    int maxwidth = 0;

    for (; *fmt != '\0'; fmt++) {
        
        // 输入不是转换符，直接输出 
        if (*fmt != '%') {
            UART_SendData(*fmt);
            continue;
        }

        // 处理转换符
        lead = ' ';
        maxwidth = 0;

        // format: %08d, %8d, %d, %u, %x, %f, %c, %s
        fmt++;      // 跳过'%'

        // 处理填充字符
        if (*fmt == '0') {
            lead = '0';
            fmt++;
        }

        // 处理宽度
        while (*fmt >= '0' && *fmt <= '9') {
            maxwidth *= 10;
            maxwidth += (*fmt - '0');
            fmt++;
        }

        // 处理具体转换类型
        switch (*fmt)
        {
            case 'd': out_num(va_arg(ap, int), 10, lead, maxwidth); break;
            case 'o': out_num(va_arg(ap, unsigned int), 8, lead, maxwidth); break;
            case 'x': out_num(va_arg(ap, unsigned int), 16, lead, maxwidth); break;
            case 'u': out_num(va_arg(ap, unsigned int), 10, lead, maxwidth); break;
            case 'c': outc(va_arg(ap, int)); break; 
            case 's': outs(va_arg(ap, char *)); break;
            default: outc(*fmt); break;
        }    
    }
    return 0;
}

int printf(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    my_vprintf(fmt, ap);
    va_end(ap);
    return 0;
}

int Lee_printf_test(void)
{
	printf("This is Chime Lee   Lee_printf_test\n\r") ;	
	printf("test char           =%c,%c\n\r", 'A','a') ;	
	printf("test decimal number =%d\n\r",    123456) ;
	printf("test decimal number =%d\n\r",    -123456) ;	
	printf("test hex     number =0x%x\n\r",  0x55aa55aa) ;	
	printf("test string         =%s\n\r",    "Chime Lee") ;	
	printf("num=%08d\n\r",   12345);
	printf("num=%8d\n\r",    12345);
	printf("num=0x%08x\n\r", 0x12345);
	printf("num=0x%8x\n\r",  0x12345);
	printf("num=0x%02x\n\r", 0x1);
	printf("num=0x%2x\n\r",  0x1);

	printf("num=%05d\n\r", 0x1);
	printf("num=%5d\n\r",  0x1);

	return 0;
}