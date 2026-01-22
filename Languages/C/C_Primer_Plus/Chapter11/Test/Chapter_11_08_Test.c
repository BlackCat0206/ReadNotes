#include <stdio.h>
#include <string.h>

char* my_string_in(const char* const str, const char* const substr) 
{
    if (NULL == str || NULL == substr) 
    {
        return NULL;
    }

    const char* s1 = str;
    const char* s2 = substr;
    const char* p = str;

    while ('\0' != *p) 
    {
        s1 = p;
        s2 = substr;

        while('\0' != *s2 && '\0' != s1 && *s1 == *s2) 
        {
            s1++;
            s2++;
        }

        if ('\0' == *s2) 
        {
            return (char*)p; // Substring found
        }

        p++;
    }

    return NULL; // Substring not found
}

int main(void) 
{
    const char* str = "Hello, World!";
    const char* substr = "World";
    char* result = my_string_in(str, substr);

    if (result) 
    {
        printf("Substring '%s' found at position: %ld\n", substr, result - str);
    } 
    else 
    {
        printf("Substring '%s' not found in the string.\n", substr);
    }

    return 0;
}