#include <stdio.h>

void inputNums(double [][5], int rows);
void printNums(double [][5], int rows);
double average(double [][5], int rows);
double max(double [][5], int rows);

void inputNums(double nums[][5], int rows) {
    for (int i = 0; i < rows; i++) {
        printf("Enter 5 numbers for row %d: ", i + 1);
        for (int j = 0; j < 5; j++) {
            scanf("%lf", &nums[i][j]);
        }

        while (getchar() != '\n')
            continue;
    }
    return;
}

void printNums(double nums[][5], int rows) {
    for (int i = 0; i < rows; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < 5; j++) {
            printf("%6.2f ", nums[i][j]);
        }
        printf("\n");
    }
    return;
}

double average(double nums[][5], int rows) {
    double sum = 0.0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 5; j++) {
            sum += nums[i][j];
        }
    }
    return sum / (rows * 5);
}

double max(double nums[][5], int rows) {
    double maxVal = nums[0][0];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 5; j++) {
            if (nums[i][j] > maxVal) {
                maxVal = nums[i][j];
            }
        }
    }
    return maxVal;
}

int main(void) {
    int rows = 3;
    double nums[3][5];
    inputNums(nums, rows);
    printNums(nums, rows);
    printf("Average: %.2f\n", average(nums, rows));
    printf("Maximum: %.2f\n", max(nums, rows));

    return 0;
}