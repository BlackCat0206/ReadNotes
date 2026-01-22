#include <stdio.h>

#define DunbarBumber 150

int main(void)
{
    int weeks = 1;
    int sum = 5;

    do
    {
        sum = (sum - weeks) * 2;
        printf("After %2d week(s), the number of rabbits is %3d\n", weeks++, sum);
    }while(sum < DunbarBumber);

    return 0;

}