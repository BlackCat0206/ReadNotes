/**
 * @file 03_ShellSort.c
 * @brief 实现希尔排序算法对整数数组进行升序排序。
 * @author Lee
 * @date 2025-12-09
 * @note:
 * 
 */

#include <stdio.h>
/**
 * @brief 对整数数组进行希尔排序。
 * @param [in] arr 数组
 * @param [in] size 数组大小
 * @return 无
 */
void Shell_Sort(int arr[], int size)
{
    if (NULL == arr || 0 >= size)
    {
        return;
    }

    int gap = size;

    while(gap > 1)
    {
        gap  = gap / 3 + 1;

        int i = 0;
        int end = 0;
        for (i = 0; i < size - gap; ++i)
        {
            // 设置有序数组位置
            end = i;
            // 设置插入元素副本
            int temp = arr[end + gap];
            while(end >= 0)
            {
                // 如果未找到位置，则进行替换
                if(arr[end] > temp)
                {
                    arr[end + gap] = arr[end];
                    end -= gap;   // 一次移动gap个位置
                }
                // 找到插入位置
                else
                {
                    break;;
                }
            }
            // 替换元素
            arr[end + gap] = temp;
        }
    }
    return;
}

/**
 * @brief 希尔排序测试函数
 * 
 */
int main(int argc, char* argv[])
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    Shell_Sort(arr, size);
    
    printf("排序后的数组：\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}