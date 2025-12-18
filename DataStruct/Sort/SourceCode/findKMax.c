/**
 * @file findKMax.c
 * @brief 实现查找数组中第K大的元素。leetcode 215
 * 
 */
#include <stdio.h>

void SortSwap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/**
 * @brief hoare排序法三路排序，处理重复元素
 * @param [in] arr 数组
 * @param [in] left 左边界索引
 * @param [in] right 右边界索引
 * @return void
 */
int _QuickSortHoare3(int arr[], int left, int right, int K)
{
    if (left >= right)
    {
        return arr[K - 1];
    }

    // 选择基准值,防止有序数组，选择中间值
    int index = left + (right - left) / 2;
    int baseValue = arr[index];
    printf("arr[%d] is baseValue [%d]\n", index, baseValue);

    int l = left;
    int r = right;
    int i = left;   // 当前遍历指针
    while (i <= r)
    {
        if (arr[i] < baseValue)
        {
            SortSwap(&arr[i], &arr[l]);
            l++;
            i++;        // 注意这里i要前进,因为i的下标一定是 >= l的，既i与l交换后的数据一定是处理过的数据。
        }
        else if (arr[i] > baseValue)
        {
            SortSwap(&arr[i], &arr[r]);
            r--;
        }
        else
        {
            i++;
        }
    }
    

    if (K - 1 < r)
    {
        _QuickSortHoare3(arr, left, l - 1, K);
    }
    else if (K - 1 > r)
    {
        _QuickSortHoare3(arr, r + 1, right, K);
    }
    else
    {
        return arr[K - 1];
    }
}


/**
 * @brief 主函数入口
 * @return int 程序退出码
 * 
 */
int main(void)
{
    int arr[] = {4,5,1,6,2,7,3,8};
    int size = sizeof(arr) / sizeof(arr[0]);
    int K = 4;
    int result = _QuickSortHoare3(arr, 0, size - 1, K);
    printArray(arr, size);
    printf("The %dth max number is %d\n", K, result);
    return 0;
}