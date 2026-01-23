# LeetCode_Hot_100

---- Chime Lee

## 一、概述

二刷`Hot100`过程中遇到的问题。

## 二、问题

### 2.1 哈希

**01_两数之和**：

* `iter`是指针：`return {nums[i], iter->second};`

* `unordered_map`如何插入数据：`un_map.insert({nums[i], i});`

**49_字母异位词分组**：

* 如何将计算的key和对应value存储：`unordered_map<string, vector<string>>`

**128_最长连续序列**：

* 如何去除重复元素：`unordered_set<int> un_set(nums.begin(), nums.end());`
* 要遍历去重的元素，假设每个数做起点：`for (const int& num : un_set)`
* 如何过滤非**最小起点的数**：`if (0 != un_set.count(num - 1)){continue;}`
* 确认起点后如何计算长度`while(0 != un_set.count(curNum + 1)){curNum++; curLen++};`

**41_缺失的第一个正整数**：

* 原地`Hash`

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

**3_无重复字符的最长子串**：右指针遇到重复字符，左指针收缩至重复位置 + 1

* **怎么统计字符串中的元素**：`cnt[128] = { 0 };` ASCII表中128位。`cnt[s[i] - 'a']++`
* **窗口右移的逻辑**：当前元素统计次数为0，添加统计，更新长度（`end - start + 1`），`end`右移。
* **窗口收缩的逻辑**：当前元素出现过，收缩边界`start++;`，**极限情况下`start`位移至`end`位置。**

**438_找到字符串中所有字母异位词**：字符计数 < 0（出现非目标字符 / 目标字符超量）

* **如何统计子串中元素出现的次数**：`cnt[p[i] - 'a']++;`

* **遍历字符串中元素**：

  * 若匹配（`if (0 != cnt[s[end] - 'a'])`），则减去对应元素计数，判断当前长度是否和子串长度匹配`winSize == end - start + 1`。匹配则填充`start`。
  * 不匹配：`start`收缩，**极限情况下`start`位移至`end`位置**。

  ```cpp
  int c = s[end] - 'a';
  cnt[c]--;
  
  while (cnt[c] < 0){
      cnt[s[start] - 'a']++;
      start++;
  }
  ```

* 注意：`right`要记得++;

**239_滑动窗口的最大值**：窗口长度超过 k 时，左指针收缩（弹出过期最大值）

* **如何实时记录窗口的最大值，且不会误删下一个最大元素？**`deque`双向队列，其`front()`为当前窗口最大元素，后续队列依次递减（如何实现：传入元素大于队列尾元素，**pop_back()**，**尾元素继续比，直到为空或尾元素大于传入元素**）
* **如何保证出队时不会误删当前窗口最大值？**`pop`时传入要出队的元素，若当前窗口最大值不匹配，则直接返回，不出队。
* **整体流程**：
  * 实现自定义滑动窗口`myQueue`
  * 先填充`k`个元素，获取最大值`front()`
  * 逐个出队（pop(nums[k - i])），填充，获取。

**76_最小覆盖子串**：窗口满足 “覆盖目标” 后，左指针收缩以找最小值

* 遇到子串匹配，且不需要完全匹配。统计子串的字符出现次数。
* 本次遇到的问题：陷入了匹配思路僵局，刻意的元素出现再去执行递减操作。
* 判断元素是否大于0：计数+1
* 直接递减。
* 长度符合后，记录判断并更新最小长度，收缩左窗口满足不匹配即可。
* 注意`substr`用法，len是要包含开始下标的，故计算长度使用`right - left + 1`即可。
* 最后的返回判断也很重要：`return (minLen == INT_MAX) ? "" : s.substr(start, minLen);`

```cpp
while (count == t.size()){
    curLen = right - left;
    if (curLen < minLen){
        minLen = curLen;
        start = left;
    }
    
    // 这里的思路很重要，right就不需要刻意匹配指定的元素。
    cnt[s[left]]++;
    if (cnt[s[left] > 0]){
        count--;
    }
    left++;
}
```

### 2.4 前缀和

**560_和为k的子数组**：

* 滑动窗口为什么不用？不能修改输入`nums`的顺序，无法确认收缩条件。
* 前缀和的作用：`preSum[i] = nums[0] + nums[1] + ... + nums[ i - 1];`
* 如何计算前缀和：`preSum[i + 1] = preSum[i] + nums[i];`(保留一个0，用于计算单个元素满足条件的问题)。
* 问题转换满足条件的次数`preSum[i] - preSum[i - x] == k` -> `preSum[i] - k == preSum[i - x]`。
* 回归到两数之和的问题。`key`是前缀和，`value`是出现的次数。

**53_最大子数组和：**

* 对于求连续和，都可以试试前缀和。
* 有了前缀和，如何求最大数组和呢？暴力双重`for`会超时。
* 计算前缀和时，每次统计最小前缀和，当前前缀和-最小前缀和，求最大值。

```cpp
for (int num : nums){
    preSum += num;
    maxVlaue = max(maxValue, preSum - min_preSum);
    min_preSum = min(min_preSum, preSum);
}
return maxValue;
```

**238_除了自身以外数组的乘积**：

* 维护一个前缀，维护一个后缀

* 后缀写法：

```cpp
vector<int> backMulti(nums.size(), 1);		// 这里可以不+1
for (int i = nums.size() - 1; i > 0; i++){
    backMulti[i - 1] = backMulti[i] * nums[i];
}
```

* 使用O(1)解法，维护一个后缀数组，使用一个遍历实时维护`pre`，将计算结果填充后缀数组。

```cpp
int pre = 1;
for (int i = 0; i < nums.size(); i++){
    backMulti[i] *= pre;
    pre *= nums[i];
}
return backMulti;
```

### 贪心：

**56_合并区间：**

* 先对当前区间**按起点排序**，若相同则使用终点。

```cpp
// 对现有元素进行排序
sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int>&b)->bool{
    if (a[0] != b[0]){
        return a[0] < b[0];
    }
    return a[1] < b[1];
});
```

* 比较待**合并区间的终点**是否覆盖了**当前区间的起点**。覆盖则取终点的最大值合并，否则作为新的待合并区间。

```cpp
vector<vector<int>> ans;
for (auto& p : intervals)
{
    // 比较待合并区间的尾端和当前区间的起点，若覆盖则取最大范围合并。
    if (!ans.empty() && p[0] <= ans.back()[1]){
        ans.back()[1] = max(ans.back()[1], p[1]);
    } else {    // 不覆盖则作为新的待合并区间
        ans.push_back(p);
    }
}
```

### 数组操作：

**189_轮转数组：**适用于嵌入式中环形缓冲区（Ring Buffer/Circular Buffer），数据帧 / 协议包的字节序 / 位序调整。

* k过大时，要取模：`k = k % nums.size();`
* 三次反转，注意范围。

