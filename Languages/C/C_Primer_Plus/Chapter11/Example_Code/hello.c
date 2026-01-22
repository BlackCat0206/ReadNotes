#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i, time;

    if (argc < 2 || (time = atoi(argv[1]) < 1))
    {
        fprintf(stderr, "Usage: %s positive-integer\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        for (i = 0; i < time; i++)
            puts("Hello, World!");
    }

    return 0;
}