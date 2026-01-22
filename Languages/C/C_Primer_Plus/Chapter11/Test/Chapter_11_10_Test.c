#include <stdio.h>
#include <string.h>
#include <stdbool.h>


size_t getStringSize(const char* const str) 
{
    if (NULL == str) 
    {
        return 0;
    }

    const char* ptr = str;
    size_t size = 0;

    while ('\0' != *ptr && '\t' != *ptr && '\n' != *ptr && '\v' != *ptr) 
    {
        size++;
        ptr++;
    }

    return size;
}

void clearblank(char* const str) 
{
    if (NULL == str) 
    {
        return;
    }

    size_t size = getStringSize(str);
    if (0 == size) 
    {
        return;
    }

    char* read_ptr = str;
    int slowIndex = 0;
    int fastIndex = 0;
    for (fastIndex = 0; fastIndex < size; fastIndex++) 
    {
        if (' ' != *(read_ptr + fastIndex)) 
        {
            *(read_ptr + slowIndex) = *(read_ptr + fastIndex);
            slowIndex++;
        }
    }
    *(read_ptr + slowIndex) = '\0'; // 添加字符串结束符
    return;
}

bool is_blank_string(const char* const str) 
{
    if (NULL == str) 
    {
        return true;
    }

    size_t size = getStringSize(str);
    if (0 == size) 
    {
        return 0;
    }

    const char* ptr = str;
    for (size_t i = 0; i < size; i++) 
    {
        if (' ' != *(ptr + i)) 
        {
            return false; // Found a non-blank character
        }
    }

    return true; // All characters are blank
}

int main(void) 
{
    char str[] = " H e l l o ,   W o r l d ! ";
    printf("Original string: '%s'\n", str);

    clearblank(str);
    printf("String after clearblank: '%s'\n", str);

    char str2[10] = {0};
    while (NULL != fgets(str2, sizeof(str2), stdin)) 
    {
        if (is_blank_string(str2)) 
        {
            printf("Input is a blank string. Please enter a non-blank string.\n");
            break;;
        }

        clearblank(str2);
        while('\n' != getchar())        // 清除输入的缓冲区
        {
            continue;
        }
        printf("String after clearblank: '%s'\n", str2);
    }

    printf("End of program.\n");
    return 0;
}