#include <stdio.h>

int main(void)
{
    char inputChar;
    
    printf("Enter a character: ");
    while (1 == scanf("%c", &inputChar))
    {
        if (inputChar >= 'a' && inputChar <= 'z')
        {
            printf("The uppercase of %c c position is %d\n", inputChar, (int)(inputChar - 'a' + 1));
        }
        else if (inputChar >= 'A' && inputChar <= 'Z')
        {
            printf("The lowercase of %c position is %d\n", inputChar, (int)(inputChar - 'A' + 1));
        }
        else
        {
            printf("%c is not an alphabetic character.\n", inputChar);
            break;
        }
        while (getchar() != '\n')
        {
            continue; // clear input line  
        }
        printf("Enter a character: ");
    }

    printf("Done.\n");
    return -1;
}