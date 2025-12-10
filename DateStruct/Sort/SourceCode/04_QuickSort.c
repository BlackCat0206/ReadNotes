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

#define HOARE_QUICK_SORT 0
#define HOLE_QUICK_SORT 0
#define LOMOTO_QUICK_SORT 1

void Sort_Swap(int* left, int* right)
{
    int temp = *left;
    *left = *right;
    *right = temp;
    return;
}

/**
 * @brief hoare排序法
 * @param [in] arr 数组
 * @param [in] left 左边界索引
 * @param [in] right 右边界索引
 * @return int mid 本次基值存放的位置
 * @attention: 在执行操作时，判断left <= right！！！
 */
int _QuickSortHoare(int arr[], int left, int right)
{
    // 确认基值位置，left指针左移
    int base = left++;

    while(left <= right)
    {
        // 右指针向左走，寻找比基值小的元素
        while (left <= right && arr[base] < arr[right])
        {
            --right;
        }

        // 左指针向右走，寻找比基值大的元素
        while (left <= right && arr[base] > arr[left])
        {
            ++left;
        }

        // 左指针找到比基值大的元素，右指针找到比基值小的元素，交换。
        if (left <= right)
        {
            Sort_Swap(&arr[left], &arr[right]);
            ++left;
            --right;
        }
    }
    // 交换基值与右指针位置元素
    Sort_Swap(&arr[base], &arr[right]);
    return right;
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

#ifdef HOARE_QUICK_SORT
    mid = _QuickSortHoare(arr, left, right);
#endif

#ifdef HOLE_QUICK_SORT
    mid = _QuickSortHole(arr, left, right);
#endif

#ifdef LOMOTO_QUICK_SORT
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
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    Quick_Sort(arr, 0, size - 1);
    
    printf("排序后的数组：\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}