#include <stdio.h>

int main(void) 
{
    double arrNum1[8], arrNum2[8];
    double sum1 = 0.0;
    int i = 0;
    printf("Please input 8 numbers for array 1:\n");
    for (i = 0; i < 8; i++) 
    {
        scanf("%lf", &arrNum1[i]);
        sum1 += arrNum1[i];
        arrNum2[i] = sum1;
    }

    printf("The numbers in array 1 are:\n");
    for (i = 0; i < 8; i++) 
    {
        printf("%.2f ", arrNum1[i]);
    }
    printf("\n");

    printf("The numbers in array 2 are:\n");
    for (i = 0; i < 8; i++) 
    {
        printf("%.2f ", arrNum2[i]);
    }
    printf("\n");

    return 0;
}