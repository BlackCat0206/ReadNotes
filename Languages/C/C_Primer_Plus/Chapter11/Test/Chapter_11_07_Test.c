#include <stdio.h>
#include <string.h>

char* my_strncpy(char* dest, const char* const src, size_t n) 
{
    if (NULL == dest || NULL == src || n == 0) 
    {
        return dest;
    }

    char* ret = dest;
    char* src_ptr = (char*)src;

    while((0 != n) && ('\0' != *src_ptr))
    {
        *dest++ = *src_ptr++;
        n--;
    }

    return ret;
}


int main(void) 
{
    char dest[20];
    const char* src = "Hello, World!";
    size_t n = strlen(src);

    my_strncpy(dest, src, n);
    dest[n] = '\0'; // Null-terminate the destination string

    char dest_full[20];
    strncpy(dest_full, src, sizeof(dest_full) - 1);
    dest_full[sizeof(dest_full) - 1] = '\0'; // Ensure null

    printf("Source: %s\n", src);
    printf("Destination after my_strncpy: %s\n", dest);
    printf("Destination after standard strncpy: %s\n", dest_full);

    return 0;
}