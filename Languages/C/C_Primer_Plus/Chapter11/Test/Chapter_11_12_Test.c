#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int AlphaCount(const char* const str)
{
    if (NULL == str)
    {
        return 0;
    }

    int count = 0;
    const char* p = str;
    while('\0' != *p)
    {
        if (isalpha((unsigned char)*p))
        {
            count++;
        }
        p++;
    }

    return count;
}

int alphaUpCount(const char* const str)
{
    if (NULL == str)
    {
        return 0;
    }

    int count = 0;
    const char* p = str;
    while('\0' != *p)
    {
        if (isupper((unsigned char)*p))
        {
            count++;
        }
        p++;
    }

    return count;
}

int alphaLowCount(const char* const str)
{
    if (NULL == str)
    {
        return 0;
    }

    int count = 0;
    const char* p = str;
    while('\0' != *p)
    {
        if (islower((unsigned char)*p))
        {
            count++;
        }
        p++;
    }

    return count;
}

int alphaDigitCount(const char* const str)
{
    if (NULL == str)
    {
        return 0;
    }

    int count = 0;
    const char* p = str;
    while('\0' != *p)
    {
        if (isalnum((unsigned char)*p))
        {
            count++;
        }
        p++;
    }

    return count;
}

int alphaPunctCount(const char* const str)
{
    if (NULL == str)
    {
        return 0;
    }

    int count = 0;
    const char* p = str;
    while('\0' != *p)
    {
        if (ispunct((unsigned char)*p))
        {
            count++;
        }
        p++;
    }

    return count;
}

int wordsCount(const char* const str)
{
    if (NULL == str)
    {
        return 0;
    }

    int count = 0;
    const char* p = str;
    bool inWord = false;

    while('\0' != *p)
    {
        if (isspace((unsigned char)*p))
        {
            inWord = false;

        }
        else
        {
            if (!inWord)
            {
                count++;
                inWord = true;
            }
        }
        p++;
    }

    return count;
}

int main(void)
{
    char str[128] = {0};
    printf("Please input string to analyze (Ctrl+D to end):\n");
    while(EOF != fgets(str, sizeof(str), stdin))
    {
        printf("Alpha Count: %d\n", AlphaCount(str));
        printf("Uppercase Alpha Count: %d\n", alphaUpCount(str));
        printf("Lowercase Alpha Count: %d\n", alphaLowCount(str));
        printf("Alphanumeric Count: %d\n", alphaDigitCount(str));
        printf("Punctuation Count: %d\n", alphaPunctCount(str));
        printf("Word Count: %d\n", wordsCount(str));
        memset(str, 0, sizeof(str));
    }
    return 0;
}