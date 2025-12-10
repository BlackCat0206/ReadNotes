/**
 * @file 01_SelectSort.c
 * @brief 实现选择排序算法对整数数组进行升序排序。
 * @author Lee
 * @date 2025-12-09
 * @note:
 * 
 */

#include <stdio.h>

/**
 * @brief 对整数数组进行选择排序。
 * @param [in] arr 数组
 * @param [in] size 数组大小
 * @return 无
 */
void Select_Sort(int arr[], int size)
{
    if (NULL == arr || 0 >= size)
    {
        return;
    }

    int i = 0;
    int j = 0;
    int min = 0;
    for (i = 0; i < size; ++i)
    {
        min = i;
        for (j = i + 1; j < size; ++j)
        {
            if (arr[min] > arr[j])
            {
                min = j;
            }
        }

        // 若最小值不是当前位置，则交换
        if (i != min)
        {
            int temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }
    return;
}

/**
 * @brief 选择排序测试函数
 * 
 */
int main(int argc, char* argv[])
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    Select_Sort(arr, size);

    printf("Sorted array: \n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}