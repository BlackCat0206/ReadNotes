# 03_Shell_Sort

---- Lee

## 一、概述

希尔排序（*Shell Sort*）：**缩小增量排序**。基于插入排序的思想，先使得相隔`gap`的元素有序，之后逐步缩小`gap`至`1`，以此减少时间复杂度。

## 二、思路

* 计算当前的`gap`：`gap = n; gap = gap / 3 + 1;`
  * 此处的+1是为了保证最后一次`gap`是1。
  * 除数3不是固定的，**变化的gap值应尽量满足没有除1以外的公因子，并且最后一次gap的值是1**。
* 以当前`gap`为基础进行排序，完成后缩小`gap`直至到1。
* `gap`为1完成插入排序后，即可。

![image](https://ucc.alicdn.com/pic/developer-ecology/vjrzezee6fdsm_c0219f1d40b3472881c6a4abc1b17cd8.png)

## 三、代码

```c
/**
 * @brief 对整数数组进行希尔排序。
 * @param [in] arr 数组
 * @param [in] size 数组大小
 * @return 无
 */
void Shell_Sort(int arr[], int size)
{
    if (NULL == arr || 0 >= size)
    {
        return;
    }

    int gap = size;

    while(gap > 1)
    {
        gap  = gap / 3 + 1;

        int i = 0;
        int end = 0;
        for (i = 0; i < size - gap; ++i)
        {
            // 设置有序数组位置
            end = i;
            // 设置插入元素副本
            int temp = arr[end + gap];
            while(end >= 0)
            {
                // 如果未找到位置，则进行替换
                if(arr[end] > temp)
                {
                    arr[end + gap] = arr[end];
                    end -= gap;   // 一次移动gap个位置
                }
                // 找到插入位置
                else
                {
                    break;;
                }
            }
            // 替换元素
            arr[end + gap] = temp;
        }
    }
    return;
}
```

## 四、源码

[03_ShellSort.c](./SourceCode/03_ShellSort.c)