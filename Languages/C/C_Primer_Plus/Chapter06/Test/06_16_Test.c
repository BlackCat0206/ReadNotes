#include <stdio.h>
#include <math.h>

int main(void)
{
    float num1, num2;
    int year = 0;

    num1 = 100.0;
    num2 = 100.0;

    do
    {
        num1 = num1 + (100 * 0.10);
        num2 = num2 + (num2 * 0.05);
        year++;
    }while(num1 - num2 > 0);

    printf("After %d years, the investment value of the first account is %.2f, and the second account is %.2f\n", year, num1, num2);
    return 0;

}