# Bubble_Sort

---- Lee

## 一、概述

冒泡排序（Bubble Sort）：简单的排序方式，通过逐个遍历元素将其最大值放置在数组末尾，实现排序。

## 二、实现逻辑

* 遍历数组，比较所有相邻元素，若前者大于后者，则交换两者位置。
* 当遍历到数组的最后一组相邻元素后，最大的元素已经在数组的最右侧，冒泡实现了第一轮。
* 重新遍历数组元素，除去最后一个。一趟结束，数组中第二大的元素将位于倒数第二位。

动图表示：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_0b183026ba5e4eda90a0fea9dbd6fba8.gif)

* 可以在每趟开始前假设数组是有序的，若本趟没有出现元素交换，则说明排序已经完成，直接结束。

## 三、代码实现

```cpp
/**
 * @brief 对整数数组进行冒泡排序。
 * @param [in] arr 数组
 * @param [in] size 数组大小
 * @return 无
 */
void Bubble_Sort(int arr[], int size)
{
    // 入参校验
    if (arr == NULL || size <= 0)
    {
        return;
    }

    int i = 0;
    int j = 0;
    for (i = 0; i < size; i++)
    {
        int flag = 1; // 标志位，若一趟排序未发生交换则表示已排序完成

        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                flag = 0; // 发生交换，标志位置为0
            }
        }

        if (1 == flag)
        {
            break; // 提前结束排序
        }
    }
    return;
}
```

[^时间复杂度]: O(n^2)
[^空间复杂度]: O(1)
[^稳定性（相同值的元素排序前后的相对次序是否保持不变）]: 稳定

## 四、源码

[00_BubbleSort.c](./SourceCode/00_BubbleSort.c)