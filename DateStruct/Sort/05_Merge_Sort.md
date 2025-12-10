# 05_Merge_Sort

---- Lee

## 一、概述

归并排序（*Merge Sort*）：其核心思想是：**将复杂问题分解成许多个“小问题”，然后将问题的结果进行合并，以达到排序的效果**。

## 二、思路

* 将待划分的数组进行对半划分，直至划分每一部分都是最小单元（一个元素）。
* 将最小单位是为有序数组，与其他的有序数组合并，合并后的数组依然是有序数组。
* 将所有数组合并好后，排序完成。

归并排序的图解：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_3432bf6a115041bd8fa006750a4b8583.png)

* 模板代码

```c
void MerageSort(int arr[], int left, int right)
{
    // 申请临时空间存放临时排序结果
    int* tmp = (int*)malloc(sizeof(int));
    if (NULL == tmp)
    {
        return;
    }
    _MergeSort(arr, left, right, tmp);
    free(tmp);
    tmp = NULL;
}
```

## 三、代码

```c
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
```

## 四、源码

[05_MergeSort.c](./SourceCode/05_MergeSort.c)