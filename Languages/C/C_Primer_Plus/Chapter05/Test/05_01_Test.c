#include <stdio.h>
#define MINTOHOURS 60

int main(void)
{
    int minutes, hours, remaining_minutes;
    printf("Enter the number of minutes(if <=0 quit): ");
    scanf("%d", &minutes);
    while(minutes > 0)
    {
        hours = minutes / MINTOHOURS;
        remaining_minutes = minutes % MINTOHOURS;
        printf("%d minutes is %d hours and %d minutes\n", minutes, hours, remaining_minutes);
        printf("Enter the number of minutes(if <=0 quit): ");
        scanf("%d", &minutes);
    }
    return 0;
}