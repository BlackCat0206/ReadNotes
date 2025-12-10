/**
 * @file 05_MergeSort.c
 * @brief 实现归并排序算法对整数数组进行升序排序。
 * @author Lee
 * @date 2025-12-09
 * @note: 核心思想：
 * 将数组对半分至最小单位（一个元素），
 * 默认有序，然后两两合并，合并时进行排序，最终完成排序。
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 合并两个有序子数组arr[left..mid]和arr[mid+1..right]。
 * @param [in] arr 数组
 * @param [in] left 左边界索引
 * @param [in] right 右边界索引
 * @param [in] tmp 临时数组
 * @return 无
 */
void Merge_Sort(int arr[], int left, int right, int* tmp)
{
    if (left >= right)
    {
        return;
    }

    // 计算中间位置
    int mid = left + (right - left) / 2;

    // 递归拆分排序左半部分
    Merge_Sort(arr, left, mid, tmp);
    // 递归拆分排序右半部分
    Merge_Sort(arr, mid + 1, right, tmp);

    // 合并两个有序子数组
    int begin1 = left, end1 = mid;
    int begin2 = mid + 1, end2 = right;

    // 临时数组下标
    int index = left;

    // 当两个子数组均未遍历完时，进行比较，放入临时数组
    while (begin1 <= end1 && begin2 <= end2)
    {
        if (arr[begin1] <= arr[begin2])
        {
            tmp[index++] = arr[begin1++];
        }
        else
        {
            tmp[index++] = arr[begin2++];
        }
    }

    // 将未遍历完的子数组数据放入临时数组
    while (begin1 <= end1)
    {
        tmp[index++] =  arr[begin1++];
    }

    while (begin2 <= end2)
    {
        tmp[index++] = arr[begin2++];
    }
    
    // 将临时数组数据拷贝回原数组
    for (int i = left; i <= right; ++i)
    {
        arr[i] = tmp[i];
    }
    return;
}

/**
 * @brief 归并排序主函数
 * @param [in] arr 数组
 * @param [in] size 数组大小
 * @return 无
 */
void MergeSort(int arr[], int size)
{
    if (NULL == arr || 0 >= size)
    {
        return;
    }

    // 创建临时数组
    int* tmp = (int*)malloc(sizeof(int) * size);
    if (NULL == tmp)
    {
        return;
    }

    // 调用递归归并排序函数
    Merge_Sort(arr, 0, size - 1, tmp);

    // 释放临时数组内存
    free(tmp);
    tmp = NULL;
    return;
}

/**
 * @brief 归并排序测试函数
 * 
 */
int main(int argc, char* argv[])
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    MergeSort(arr, size);
    
    printf("排序后的数组：\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}