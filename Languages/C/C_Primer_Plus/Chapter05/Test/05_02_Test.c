#include <stdio.h>

int main(void)
{
    int i = 0;
    int num = 0;

    printf("Please input a number: ");
    scanf("%d", &num);

    while(i <= 10)
    {
        printf("%04d\n", num);
        i++;
        num++;
    }
    printf("Done!\n");
    return 0;
}