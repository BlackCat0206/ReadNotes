#include <stdio.h>

int main(void) 
{
    
    int arrayNum[8];
    int i = 0;
    printf("Please input 8 numbers\n");
    for (i = 0; i < 8; i++) {
        scanf("%d", &arrayNum[i]);
    }

    printf("The numbers in reverse order are:\n");
    for (i = 7; i >= 0; i--) {
        printf("%d ", arrayNum[i]);
    }

    return 0;
}