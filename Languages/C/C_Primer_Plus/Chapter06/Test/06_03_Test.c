#include <stdio.h>

int main(void)
{
    char ch = 'F';
    int i , j;
    for(i = 0; i < 6; i++) {
        for(j = 0; j < i + 1; j++) {
            printf("%c", ch - j);
        }
        printf("\n");
    }


    return 0;
}