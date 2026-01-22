#include <stdio.h>
#include <stdlib.h>

#define CYCLE 1000
#define NUMRANGE 10

static unsigned int seed = 0;

int rand1(void)
{
    seed = seed * 1103515245 + 12345;
    return (unsigned int)(seed / 65536) % 32768;
}

int srand1(unsigned int newseed)
{
    seed = newseed;
    return 0;
}

void numsCreateAndCount(void)
{
    int i = 0;

    printf("Please enter a seed number: \n");
    scanf("%u", &seed);
    srand1(seed);

    int numArray[NUMRANGE + 1] = {0};

    for (i = 0; i < CYCLE; i++)
    {
        int num = rand1() % NUMRANGE + 1;
        numArray[num]++;
    }

    for (i = 1; i <= NUMRANGE; i++)
    {
        printf("Number %2d: %4d times\n", i, numArray[i]);
    }

    return;
}

int main(void)
{
    numsCreateAndCount();

    return 0;
}