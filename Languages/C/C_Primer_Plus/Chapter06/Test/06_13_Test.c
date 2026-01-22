#include <stdio.h>

int main(void) 
{
    int num[8];
    int i = 0;
    int sum = 2;

    for (i = 0; i < 8; i++, sum *= 2) 
    {
        num[i] = sum;
    }

    do
    {
        printf("%d ", num[--i]);
    }while(i > 0);
    printf("\n");

    return 0;
}