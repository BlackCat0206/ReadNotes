#include <stdio.h>
#include <string.h>

int main(void)
{
    char str1[64];

    printf("Please enter a string: ");
    scanf("%63s", str1);

    size_t len = strlen(str1);

    int i = 0;
    for (i = len - 1; i >= 0; i--)
    {
        printf("%c", str1[i]);
    }
    printf("\n");

    return 0;
}