# LeetCode_Hot_100

---- Chime Lee

## 一、概述

二刷`Hot100`过程中遇到的问题。

## 二、问题

### 2.1 哈希

**01_两数之和**：

* `iter`是指针：`return {nums[i], iter->second};`

* `unordered_map`如何插入数据：`un_map.insert({nums[i], i});`

**49_字母异未词分组**：

* 如何将计算的key和对应value存储：`unordered_map<string, vector<string>>`

**128_最长连续序列**：

* 如何去除重复元素：`unordered_set<int> un_set(nums.begin(), nums.end());`
* 要遍历去重的元素，假设每个数做起点：`for (const int& num : un_set)`
* 如何过滤非**最小起点的数**：`if (0 != un_set.count(num - 1)){continue;}`
* 确认起点后如何计算长度`while(0 != un_set.count(curNum + 1)){curNum++; curLen++};`

### 2.2 双指针

**283_移动零**：

双指针：O(n)

* left指向“已整理区域的末尾，已整理区域第一个0的下标”。

* right向后遍历，遇到不为0的数和`left`交互，将`left`所指0置换到末尾，`left++`指向新的第一个为0的位置。

插入排序：逐步选择整理的末尾下标。O(n^2)

* 如何整理：待插入元素不为0，整理下标对应为0，继续移动（元素右移）直至元素不为0，插入。
* 按照动画理解：`if (tempNum != 0 && nums[end] == 0){nums[end + 1]  = nums[end]; end--}`
* 插入：`nums[end + 1] = tempNum;`

**11_盛水最多的容器**：

* 如何计算容量：`are = (right - left) * min  (height[left], height[right]);`
* 如何收缩边界：左 > 右，收缩右，反之收缩左。

**15_三数之和**：

* 先将数组排序

* 枝叶裁剪：`if (nums[i] > 0){break};`

* 去重（**两部分，很重要！！！**）

  * 外部：`if (i > 0 && nums[i] == nums[i - 1]){continue;}`
  * 内部：找到对应元素后，对其左右元素继续去重：

  `while (left < right && nums[left] == nums[left+1]){left++;}`

  `while (left < right && nums[right] == nums[right-1]){right--;}`

  * 继续缩减边界（**本次编写代码时进入死循环**！！！）
  * `left++; right--;`

* ans的内容：是**满足条件的元素**，不是下标。

**42_接雨水**：

* 接雨水的容量，**依赖于两则最高边界的最小值与当前容器的差**。
* 实时更新：`leftMax = max(leftMax, nums[left]); rightMax = max(rightMax, nums[right]);`
* 左侧最小：`ans += leftMax - height[left]; left++;`
* 右侧最小：`ans += rightMax - height[right]; right--;`

### 2.3 滑动窗口

