# 01_Select_Sort

---- Lee

## 一、概述

选择排序（*Select Sort*）：比较符合人类思维，每次从数组中获取到其最小/最大元素的下标，将其对应元素与当前数组的首/尾元素交换。

## 二、思路

* 首先遍历数组，找到其最小元素的下标，将其对应内容与数组首元素替换。
* 再次遍历剩余数组，找到其最小元素的下标，将其对应内容与当前遍历范围的首元素替换。
* 重复，直到“剩余数组”只有一个元素。

动图表示：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_a310ed0a79454fff91ec3b239062e1ea.gif)

## 三、代码

```c
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
```

[^时间复杂度]: O(n^2)
[^空间复杂度]: O(1)
[^稳定性]: 不稳定

## 四、源码

[01_SelectSort.c](./SourceCode/01_SelectSort.c)

