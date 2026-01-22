#include <stdio.h>

int main(void)
{
    double num = 100.0;
    int year = 0;

    do
    {
        num = num + num * 0.08;
        num -= 10.0;
        year++;
    }while(num > 10.0);

    printf("After %d years, the account value is %.2f\n", ++year, num);
    return 0;
}