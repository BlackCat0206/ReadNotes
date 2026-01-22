#include <stdio.h>

int main(void)
{
    float num1, num2;

    printf("Please enter two float numbers: ");
    while(2 == scanf("%f %f", &num1, &num2)) {
        printf("The result of %.2f and %.2f is %.2f\n", num1, num2, (num1 - num2)/(num1 * num2));
        printf("Please enter two float numbers (q to quit): ");
    }
    printf("Bye.\n");
    return 0;
}