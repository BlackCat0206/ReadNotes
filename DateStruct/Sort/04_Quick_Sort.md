# 04_Quick_Sort

---- Lee

## 一、概述

 快速排序（*Quick Sort*）：将待排序的数组划分成**几乎均等的两份**，左半部分数值比右半部分数值小。递归性质将这两份数据按照上述规则继续划分，**直到每一部分都是最小单元为止**。此时，数组排序已经完成。	

在划分数组时，需要找一个参考基准，一部分小于这个基准，一部分大于这个基准。在这里，**默认数组首元素为基准值**。

## 二、思路

* 快排的大框架：（二叉树的递归三要素再次回归(⌐■_■)）

```c
// 1.返回值类型和参数列表，数组和左右区间边界，是左闭右闭区间
void Quick_Sort(int arr[], int left, int right)
{
    // 2.终止条件：划分的左右区间相等，划分结束，回归
    if (left >= right)
    {
        return;
    }
    
    // 3.单次遍历要实现的内容
    int key = _QuickSort(arr, left, right);
    
    // 递归，对两部分数组进行分别处理，使其继续划分
    Quick_Sort(arr, left, key - 1);
    Quick_Sort(arr,  key + 1, right);
    return;
}
```

* `_QuickSort`子方法就是我们划分的思路。

**三种划分思路**：

* `hoare`版本：
  * 创建左右“指针”分别指向数组两端。
  * **右指针**向左走，寻找比**基准值小**的值；**左指针**向右走，寻找比**基准值大**的值。
  * 将左右指针的数据交换。交换之后继续移动寻找数据。
  * 当左指针在右指针之后时，停止遍历，将基准值放在**中间位置（右值值位置）**，返回其地址（右值值的下标）。

画图模拟划分过程：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_57edbc30614943279a4746d7bb4eaa72.png)

* **挖坑法**
  * 左右两个指针分别指向数组的两侧，将坑位`hole`设置位数组的首元素的位置，并记录基准值。
  * 右指针向左走，遇到比基数小的值，将该数填坑，并将`hole`指向右指针。
  * 左指针向右走，遇到比基数大的值，将该数填坑，并将`hole`指向左指针。
  * 重复2，3步，当左右指针相遇时，将记录的基准坑值填入当前位置，并返回该坑位置。

画图模拟划分过程：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_5df80d8273b24809b2ec122544fc713f.png)

动图表示：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_0b8bdde40f334c4fa8b7f5b3f7e1a9fc.gif)

* **`lomoto`版本**
  * 实现`prev`和`cur`指针，`prev`指向元素首地址，`cur`指向下一个元素，**首元素为基准值**。
  * `cur`向右遍历，遇到比**基准值小的数据**，`prev`向前走一步，和`cur`交换元素，`cur`一直向右走，直到超出数组边界。
  * 交换基准值与`prev`所指向的元素，返回当前基准值的位置。

画图模拟划分过程：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_9c9fbbf6488c48a19661634c6444e1c2.png)

动图表示：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_604773aa651640eab735154bf8185c6c.gif)

## 三、代码

```c
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
    int base = left;
    int l = left - 1;
    int r = right + 1;
    while (l < r)
    {
        // 指针向左走，寻找基准值大的值
        do
        {
            l++;
        }
        while(arr[l] < arr[base]);

        // 指针向右走，寻找基准值小的值
        do
        {
            r--;
        }
        while(arr[r] > arr[base]);

        if (l < r)
        {
            // 交换两个指针位置元素
            Sort_Swap(&arr[l], &arr[r]);
        }
    }

    // 交换基值与右指针位置元素
    Sort_Swap(&arr[base], &arr[r]);
    return r;
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
```

[^时间复杂度]: O(NlogN)
[^空间复杂度]: O(logN)
[^稳定性]: 不稳定

## 四、源码

[04_QuickSort.c](./SourceCode/04_QuickSort.c)

## 五、拓展

* `Hoare`现有的逻辑对于处理数组中有重复元素时，会有异常。可以使用Hoare三排指针排序。
* [215_数组中的第K个最大元素.md](../Array/215_数组中的第K个最大元素.md)