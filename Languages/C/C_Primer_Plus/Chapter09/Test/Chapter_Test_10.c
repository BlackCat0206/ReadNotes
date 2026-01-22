#include <stdio.h>

void to_base_n(unsigned long int num, int base);

int main(void)
{
    unsigned long int inputNum; // 输入 number
    int base;                   // 待转换进制

    printf("Enter a number and a base (2-10): ");
    while (2 == scanf("%lu %d", &inputNum, &base))
    {
        if (base < 2 || base > 10)
        {
            printf("Base %d is not supported.\n", base);
            break;
        }

        to_base_n(inputNum, base);
        printf("\n");

        while (getchar() != '\n')
        {
            continue; // clear input line  
        }
        printf("Enter a number and a base (2-10): ");
    }
    printf("Done.\n");
    return 0;
}


void to_base_n(unsigned long int num, int base)
{
    int rmd; // 余数

    if (num >= base)
    {
        to_base_n(num / base, base);
        rmd = num % base;
        printf("%d", rmd);
    }
    else
    {
        printf("%d", num);
    }
    
    return;
}