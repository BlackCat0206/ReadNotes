/* addemup.c -- five kinds of statements */
#include <stdio.h>
int main(void)                /* finds sum of first 20 integers */
{
    int count, sum, days, countTemp;           /* declaration statement          */
    
    count = 0;                /* assignment statement           */
    sum = 0;                  /* ditto                          */

    printf("Please input the number of days: ");
    scanf("%d", &days);       /* function statement             */

    while (count++ < days)              /* while                          */
        sum = sum + (count * count);    /*     statement                  */
    printf("sum = %d\n", sum);          /* function statement             */
    
    return 0;                     /* return statement               */
}
