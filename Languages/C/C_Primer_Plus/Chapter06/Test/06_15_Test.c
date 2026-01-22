#include <stdio.h>
#include <string.h>

int main(void)
{
    int i = 0;
    char str1[256];

    printf("Please enter a string: ");
    scanf("%255s", str1);
    size_t len = strlen(str1);
    for (i = len - 1; i >= 0; i--)
    {
        printf("%c", str1[i]);
    }

    printf("\n");
    return 0;
}