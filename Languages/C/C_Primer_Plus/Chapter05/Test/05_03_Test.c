#include <stdio.h>

#define DAYSTOWEEK 7

int main(void)
{
    int days, weeks, remaining_days;
    printf("Enter the number of days (if <=0 quit): ");
    scanf("%d", &days);

    while(days > 0)
    {
        weeks = days / DAYSTOWEEK;
        remaining_days = days % DAYSTOWEEK;
        printf("%d days is %d weeks and %d days\n", days, weeks, remaining_days);
        printf("Enter the number of days (if <=0 quit): ");
        scanf("%d", &days);
    }

    return 0;
}