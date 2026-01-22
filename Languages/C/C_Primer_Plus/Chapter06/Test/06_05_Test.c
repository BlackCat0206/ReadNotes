#include <stdio.h>

int main(void)
{
    char ch = 'A';
    int i , j, k, n;
    for(i = 0; i < 5; i++) {

        for (k = 0; k < 5 - i - 1; k++) 
        {
            printf(" ");
        }

        for(j = 0; j < i + 1; j++) {
            printf("%c", ch + j);
        }

        for (n = i; n > 0; n--)
        {
            printf("%c", ch + n - 1);
        }

        printf("\n");
    }


    return 0;
}