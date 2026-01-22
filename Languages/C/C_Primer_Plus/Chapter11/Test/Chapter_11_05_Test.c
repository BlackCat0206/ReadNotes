#include <stdio.h>
#include <string.h>

char* my_strchr(const char* str, int c) 
{
    if (NULL == str) 
    {
        return NULL;
    }

    while('\0' != *str)
    {
        if (*str == (char)c) 
        {
            return (char*)str;
        }
        str++;
    }

    return NULL;
}


int main(void) 
{
    const char* test_str = "Hello, World!";
    char ch = 'W';
    char* result = my_strchr(test_str, ch);

    if (result) 
    {
        printf("Character '%c' found at position: %ld\n", ch, result - test_str);
    } 
    else 
    {
        printf("Character '%c' not found in the string.\n", ch);
    }

    return 0;
}