#include <stdio.h>

int main(void) 
{
    int i , j;

    printf("Please enter rang: ");
    scanf("%d, %d", &i, &j);

    printf("numbers  squares  cubes\n");
    printf("-------  -------  -----\n");
    for ( ; i <= j; i++) 
    {
        printf("%-7d  %-7d  %-5d\n", i, i * i, i * i * i);
    }
    printf("-------  -------  -----\n");

    return 0;
}