#include <stdio.h>

#define f(x) ((x) - 1)
#define NUMPRINT(x) printf("This number " #x " is %d\n", x)
#define NUMX(n) x##n
#define assert(x) if (!(x)) {printf("Assertion failed:" #x ", file %s, line %d\n",  __FILE__, __LINE__);}

static int x = 1;
static int y = 1;

const int x1 = 2;
const int x3 = x1 + 1;
const int x2 = x3 + 7;

static const int z = x2 + 3;

int main(void)
{
    int i = 10;

    const int x = f(i);
    printf("x is %d\n", x);

    printf("x1 is %d\n", x1);
    printf("x2 is %d\n", x2);
    printf("x3 is %d\n", x3);

    return 0;
}