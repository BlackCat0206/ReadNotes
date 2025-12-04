#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int arrayLength = 0;

    scanf("%d", &arrayLength);

    if (0 >= arrayLength)
    {
        perror("Input ArrayLength is error!!!");
        return -1;
    }

    // 构建数组
    int* arr = (int*)malloc(sizeof(int) * arrayLength);
    if (NULL == arr)
    {
        perror("malloc array failed!");
        return -1;
    }

    // 依次获取输入数据，构成区间和
    int num = 0;
    int sum = 0;
    int i = 0;
    for (i = 0; i < arrayLength; i++)
    {
        scanf("%d", &num);
        sum += num;
        arr[i] = sum;
    }

    // 获取计算区间
    int left = 0;
    int right = 0;

    int result = 0;
    while(0 != scanf("%d %d", &left, &right))
    {
        if (left < right)
        {
            perror("Input space is error! exit");
            free(arr);
            arr = NULL;
            return -1;
        }

        result = arr[right] - arr[left - 1];

        printf("%d\n", result);
    }

    free(arr);
    arr = NULL;
    return 0;
}