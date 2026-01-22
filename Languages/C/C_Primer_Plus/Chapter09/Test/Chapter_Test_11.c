#include <stdio.h>

int fibonacci(int n);

int main(void)
{
    int inputNum; // 输入 number
    int result;   // 结果

    printf("Enter a positive integer: ");
    while (1 == scanf("%d", &inputNum))
    {
        if (inputNum < 0)
        {
            printf("Negative number is not supported.\n");
            break;
        }

        result = fibonacci(inputNum);
        printf("Fibonacci of %d is %d\n", inputNum, result);

        while (getchar() != '\n')
        {
            continue; // clear input line  
        }
        printf("Enter a positive integer: ");
    }
    return 0;
}

/***
 * @brief Fibonacci sequence
 * @note
 * The Fibonacci sequence is a series of numbers where each number is the sum of the two preceding ones, usually starting with 0 and 1. That is,
 * F(0) = 0, F(1) = 1, and F(n) = F(n-1) + F(n-2) for n > 1.
 */
int fibonacci(int n)
{
    int sum = 0;
    int i = 0;

    if (n == 0)
    {
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    else
    {
        int a = 0; // F(0)
        int b = 1; // F(1)
        for (i = 2; i <= n; i++)
        {
            sum = a + b;    // F(n) = F(n-1) + F(n-2)
            a = b;          // Move to next
            b = sum;        // Move to next
        }
    }

    return sum;
}


int test_chapter_11(void)
{
    return 0;
}