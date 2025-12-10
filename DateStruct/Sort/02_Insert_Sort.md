# 02_Insert_Sort

----Lee

## 一、概述

插入排序（*Insert Sort*）：将当前元素视为有序数组，选取其后的一个元素，对有序数组进行从后向前遍历，选择合适的位置插入。

## 二、思路

* 将数组首元素视为有序。
* 将有序数组的下一位元素取出，对该元素之前的部分进行遍历，并找到合适的位置插入。
* 将这部分有序数组看为一个整体，重复第二步，直到最后一个元素调整完成。

动图表示：

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_efb245c703e547c9a81e76a5e1e88551.gif)

## 三、代码

```c
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
```

[^时间复杂度]: O(n^2)
[^空间复杂度]: O(1)
[^稳定性]: 稳定

## 四、源码

[02_InsertSort.c](./SourceCode/02_InsertSort.c)