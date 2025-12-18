/**
 * @file BubbleSort.c
 * @brief 实现冒泡排序算法对整数数组进行升序排序。
 * @author Lee
 * @date 2025-12-09
 * 
 */

#include <stdio.h>

/**
 * @brief 对整数数组进行冒泡排序。
 * @param [in] arr 数组
 * @param [in] size 数组大小
 * @return 无
 */
void Bubble_Sort(int arr[], int size)
{
    // 入参校验
    if (arr == NULL || size <= 0)
    {
        return;
    }

    int i = 0;
    int j = 0;
    for (i = 0; i < size; i++)
    {
        int flag = 1; // 标志位，若一趟排序未发生交换则表示已排序完成

        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                flag = 0; // 发生交换，标志位置为0
            }
        }

        if (1 == flag)
        {
            break; // 提前结束排序
        }
    }
    return;
}

/**
 * @brief 冒泡排序测试
 * 
 */
int main(int argc, char* argv[])
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    Bubble_Sort(arr, size);
    
    printf("Sorted array: \n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}