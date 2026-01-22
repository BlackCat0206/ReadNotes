#include <stdio.h>

#define CMTOFEET 30.48
#define CMTOINCH 2.54

int main(void)
{
    float cm, feet, inch;
    printf("Enter the height in cm (if <=0 quit): ");
    scanf("%f", &cm);
    while(cm > 0)
    {
        feet = cm / CMTOFEET;
        inch = cm / CMTOINCH;
        printf("%.2f cm is %.2f feet and %.2f inches\n", cm, feet, inch);
        printf("Enter the height in cm (if <=0 quit): ");
        scanf("%f", &cm);
    }
    return 0;
}