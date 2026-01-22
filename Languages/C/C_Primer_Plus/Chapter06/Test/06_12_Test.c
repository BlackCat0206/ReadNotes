#include <stdio.h>

int main()
{
    int i = 0;

    double lbNum1 = 1.0;
    double lbNum2 = 1.0;

    printf("Please enter a positive integer: ");
    while (1 == scanf("%d", &i) && i > 0) 
    {
        lbNum1 = 1.0;
        lbNum2 = 1.0;
        for (int j = 2; j < i; j++) 
        {
            lbNum1 += (double)(1.0 / j);
            lbNum2 -= (double)(1.0 / j);
        }
        printf("The sum of the series 1 + 1/2 + ... + 1/%d is %.6Lf\n", i, lbNum1);
        printf("The sum of the series 1 - 1/2 + ... + (-1)^%d/%d is %.6Lf\n", i, i, lbNum2);
        printf("Please enter a positive integer (q to quit): ");
    }
    printf("Bye.\n");

    return 0;
}