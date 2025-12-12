/**
 * @file 04_QuickSort.c
 * @brief 实现快速排序算法对整数数组进行升序排序。
 * @author Lee
 * @date 2025-12-09
 * @note:
 * 1.hoare划分方案
 * 
 */

#include <stdio.h>

#define HOARE_QUICK_SORT 1
#define HOLE_QUICK_SORT 0
#define LOMOTO_QUICK_SORT 0

void Sort_Swap(int* left, int* right)
{
    int temp = *left;
    *left = *right;
    *right = temp;
    return;
}

/**
 * @brief hoare排序法三路排序，处理重复元素
 * @param [in] arr 数组
 * @param [in] left 左边界索引
 * @param [in] right 右边界索引
 * @return void
 */
void _QuickSortHoare3(int arr[], int left, int right)
{
    if (left >= right)
    {
        return;
    }

    // 选择基准值,防止有序数组，选择中间值
    int index = left + (right - left) / 2;
    int baseValue = arr[index];

    int l = left;
    int r = right;
    int i = left;   // 当前遍历指针
    while (i <= r)
    {
        if (arr[i] > baseValue)
        {
            Sort_Swap(&arr[i], &arr[l]);
            l++;
            i++;
        }
        else if (arr[i] < baseValue)
        {
            Sort_Swap(&arr[i], &arr[r]);
            r--;
        }
        else
        {
            i++;
        }
    }

    
    _QuickSortHoare3(arr, left, l - 1);
    _QuickSortHoare3(arr, r + 1, right);
    return;
}

/**
 * @brief 挖坑法 hole
 * @param [in] arr 数组
 * @param [in] left 区间左边界
 * @param [in] right 区间右边界
 * @return int holeIndex 最后坑的位置
 */
int _QuickSortHole(int arr[], int left, int right)
{
    // 记录基准坑值
    int baseValue = arr[left];
    int holeIndex = left;
    while (left < right)
    {
        // 右指针向左走，寻找比基准值小的数
        while(left < right && baseValue < arr[right])
        {
            --right;
        }

        // 将查找的值填坑，并将坑位指向右指针
        if (left < right)
        {
            arr[holeIndex] = arr[right];
            holeIndex = right;
        }

        // 左指针向右走，寻找比基准值大的数
        while (left < right && baseValue > arr[left])
        {
            ++left;
        }

        // 将查找的值填坑，并将坑位指向左指针
        if (left < right)
        {
            arr[holeIndex] = arr[left];
            holeIndex = left;
        }
    }

    // 左右指针相遇，将基准值填入相遇位置，并返回位置
    arr[holeIndex] = baseValue;
    return holeIndex;
}

/**
 * @brief lomoto排序法
 * @param [in] arr 数组
 * @param [in] left 左边界索引
 * @param [in] right 右边界索引
 * @return int mid 本次基值存放的位置
 */
int _QuickSortLomoto(int arr[], int left, int right)
{
    int prev = left;
    int baseIndex = left;
    int baseValue = arr[left];
    int cur = left + 1;
    // cur 向后遍历，直到越界
    for (cur = left + 1; cur <= right; ++cur)
    {
        // 当cur遍历到小于基准值时，prev向前走一步，并交换prev和cur的数据
        if (baseValue > arr[cur] && ++prev != cur)
        {
            Sort_Swap(&arr[prev], &arr[cur]);
        }
    }
    // 交换prev和基准值的数据
    Sort_Swap(&arr[baseIndex], &arr[prev]);
    return prev;
}

/**
 * @brief [基本框架] 对整数数组进行快速排序。
 * @param [in] arr 数组
 * @param [in] left 左边界索引
 * @param [in] right 右边界索引
 * @return 无
 * @attention:划分的边界是左闭右闭区间：[left, right]
 */
void Quick_Sort(int arr[], int left, int right)
{
    // 确认终止添加，当划分区域边界相同，返回。
    if (left >= right)
    {
        return;
    }

    // 各类快排执行
    int mid = 0;

#if HOARE_QUICK_SORT
    // mid = _QuickSortHoare(arr, left, right);
    printf("index=%d\n", mid);
    int size = right - left + 1;
    printf("Sorted array is:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
#endif

#if HOLE_QUICK_SORT
    mid = _QuickSortHole(arr, left, right);
#endif

#if LOMOTO_QUICK_SORT
    mid = _QuickSortLomoto(arr, left, right);
#endif

    // 递归划分
    Quick_Sort(arr, left, mid - 1);
    Quick_Sort(arr, mid + 1, right);
}

/**
 * @brief 快速排序测试函数
 * 
 */
int main(int argc, char* argv[])
{
    int arr[] = {3,2,1,5,6,4};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    // Quick_Sort(arr, 0, size - 1);
    _QuickSortHoare3(arr, 0, size - 1);
    
    printf("排序后的数组：\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}