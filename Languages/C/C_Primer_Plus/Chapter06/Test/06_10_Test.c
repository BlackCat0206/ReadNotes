#include <stdio.h>

int main(void)
{
    int lvalue, rvalue, sum, i;
    printf("Please enter two integers range: ");
    while (2 == scanf("%d %d", &lvalue, &rvalue))
    {
        if (lvalue >= rvalue)
        {
            break;
        }

        for (i = lvalue, sum = 0; i <= rvalue; i++)
        {
            sum += i * i;
        }

        printf("The sum of squares from %d to %d is %d\n", lvalue, rvalue, sum);
        printf("Please enter two integers range (q to quit): ");
    }
    printf("Done.\n");
    return 0;
}