#include <stdio.h>

void cube(double x)      // 计算x的立方
{
    x = x * x * x;
    printf("The cube of the number is %.2f\n", x);
}

int main(void)
{
    double x;
    printf("Please input a number: ");
    scanf("%lf", &x);
    cube(x);
    printf("The original number is %.2f\n", x);
    return 0;
}