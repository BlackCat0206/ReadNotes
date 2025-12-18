/***
 * @file 02_InsertSort.c
 * @brief 实现插入排序算法对整数数组进行升序排序。
 * @author Lee
 * @date 2025-12-09
 * @note:
 */

#include <stdio.h>
/**
 * @brief 对整数数组进行插入排序。
 * @param [in] arr 数组
 * @param [in] size 数组大小
 * @return 无
 */
void Insert_Sort(int arr[], int size)
{
    if (NULL == arr || 0 >= size)
    {
        return;
    }

    int i = 0;
    int end = 0;
    for (i = 0; i < size - 1; ++i)
    {
        // 确认有序数组最后一个元素
        end = i;
        // 确认要选择插入的元素
        int temp = arr[end + 1];
        
        // 从有序数组末尾开始比较，找到插入位置
        while (end >= 0)
        {
            // 未找到插入位置，数据后移
            if (arr[end] > temp)
            {
                arr[end + 1] = arr[end];
                --end;
            }
            else
            {
                break; // 找到插入位置，退出循环
            }
        }
        // 插入数据
        arr[end + 1] = temp;
    }
    return;
}

/**
 * @brief 插入排序测试函数
 * 
 */
int main(int argc, char* argv[])
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    Insert_Sort(arr, size);
    
    printf("排序后的数组：\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}