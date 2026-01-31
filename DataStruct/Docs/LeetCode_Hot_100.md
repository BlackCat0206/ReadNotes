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
* **新解法：2026-01-30，基于437_路径总和III**
* sum_map[0] = 1;
* 再两数之和中直接计算前缀和，一次for循环。

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

### 2.5 贪心：

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

### 2.6 数组操作：

**189_轮转数组：**适用于嵌入式中环形缓冲区（Ring Buffer/Circular Buffer），数据帧 / 协议包的字节序 / 位序调整。

* k过大时，要取模：`k = k % nums.size();`
* 三次反转，注意范围。

### 2.7 矩阵：

**73_矩阵置零**：

* 参考`Execl`，对为0的元素标记其**第一行所在列**和**第一列所在行**，最后遍历列和行。
* 判断第一行是否有0，有的话做一个**全局标记**，最后处理。
* 遍历除第一行之外的元素，对为0的元素标记其**第一行所在列**和**第一列所在行**。若第一列有0，·则`matrix[0][0]`会是0
* 遍历除第一行和第一列的元素，若其对应第一行所在列**或**第一列所在和为0，则将其置0。
* 处理第一列，判断`matrix[0][0]`。
* 判断全局标记，处理第一行。

**54_螺旋矩阵**：

* 上下左右边界均为可访问值。
* 边界判断：`while (left <= right && top <= bottom)`
* 上侧：`for (int i = left; i <= right; i++)`
* 右侧：`for (int j = top + 1; j <= bottom; j++) top+1 <= bottom本身可以完成去重` 

* 下侧：`for (int i = right - 1; i>= left && top != bottom; i--)` top != bottom去重
* 左侧：`for (int j = bottom - 1; j>=top+1 && left != right; j--)` left != right去重
* 除左侧外，其余三边直接访问到边界，不需要拦截。

**48_旋转图像:**

* 矩阵：转置->行反转
* 转置：访问对角线下方元素与上方元素交换。
* 逐行反转。

**240_搜搜二维矩阵II**

* 左下角出发，比目标元素大上走，比目标元素小左走，匹配返回`true`。
* 边界`while(x <= matrix[0].size() - 1 && y >= 0)`

### 2.8 链表

**160_相交链表：**

* 如果一切顺利的话，我们一定会相遇。
* 实时比较当前两个节点，若有一个节点为空，则进入下一个节点的起点。
* 如果有相交，两者会走相同的路径

```cpp
while (node1 != node2){
    node1 = (nullptr != node1) ? node1->next : headB;
    node2 = (nullptr != node2) ? node2->next : headA;
}
return node1;
```

**206_反转链表：**

* **记录三个节点**：`pre`、`cur`、`next`。
* 保存`next`，反转`cur`，`pre`后移、`cur`后移。

```cpp
while (nullptr != cur){
    next = cur->next;
    cur->next = pre;
    pre = cur;
    cur = next;
}
```

**234_回文链表：**

* 空链表和只有一个元素的链表都是回文链表。
* **寻找链表中间节点的方式**：快指针一次两步，慢指针一次走一步。

```cpp
// fast一次走两步的判断
while (nullptr != fast->next && nullptr != fast->next->next){
    slow = slow->next;
    fast = fast->next->next;
}

// 终点在slow->next
if (nullptr != slow->next){
    slow = slow->next;
}
```

* 每个链表节点指针要初始化。
* 注意回文判断的是链表节点的元素。

**141_环形链表**：

* **快慢指针**，若有环一定会相遇。
* 先移动，再判断。

**142_环形链表II：**

* **快慢指针**：判断是否有环。
* 若有环，一个指针从头走，一个节点从相遇节点走，相遇时就是环入口。

* 数学推导：
  * 节点头到环入口是`a`，环长是`c`。
  * 慢指针走了`b`步，快指针走了`2b`步，那么`2b - b = kc`可得，`b = kc`。
  * 慢指针从**入环口**开始，在**环中**走了`b - a = kc - a`步到达相遇点。
  * 这就说明从**相遇点开始，再走`a`步，就恰好走到了入环口了**。

**21_合并两个有序链表**

* 使用**虚拟头节点**作为开始，同时遍历两个链表中的元素，从小到大逐个填充。
* **结束后判断是否有两个链表是否有剩余的，有的话填充**。
* 返回虚拟头节点的**后继节点**。

**2_两数相加：**

* 将结果存储在新的链表中，不要修改已有的内容。
* **虚拟头节点保存内容**
* 一个循环中解决问题，条件：

```cpp
while (nullptr != l1 || nullptr != l2 || 0 != carry)
```

* 逐个判断解决是否为空，不为空carry与元素相加，节点位移。

```cpp
if (nullptr != l1) {
    carry += l1->val;
    l1 = l1->next;
}
```

* 创建新节点，carry取模。carry / 10，保存节点位移。

**19_删除链表的倒数第N个节点：**

* **虚拟头节点**，规避只有一个元素的链表删除问题。

* **快慢指针**：快指针先走n步，`while ((0 != n) && (nullptr != fast))`
* 快慢指针一起走，`slow`要到被删除节点的前一位置，`while (nullptr != fast->next)`
* **循环条件中什么使用用`node`什么时候用`node->next`？**
  * `node`：目标**访问每个节点**（读值、统计、遍历完所有节点）。
  * `node->next`：目标**操作下一个元素**（删除、添加、找倒数节点、走多步）。

**24_两两交换链表中的节点：**

* **虚拟头节点保存内容。**
* **三指针**：前序、当前、后继。
* 状态转移判断：`while (nullptr != cur && nullptr != cur->next)`当前节点是否还有下一个元素要交换。
* 状态转移过程：

```cpp
        // 逐个遍历 判断当前元素之后是否还有交换元素
        while (nullptr != cur && nullptr != cur->next) {
            nextNode = cur->next;
            
            // 当前指针next = 后继指针的next；
            cur->next = nextNode->next;
            // 后继指针next = 前序;
            nextNode->next = pre->next;
            // 前序指针-next =  后继
            pre->next = nextNode;

            // 位移
            pre = cur;
            cur = cur->next;
        }
```

**25_K个一组翻转链表：**

* 获取链表长度，计算整体反转次数。reverseCount = sizeList / k;
* 虚拟头节点保存结果。
* 三指针：`pre`、`cur`、`next`
* 双循环：
  * 控制整体反转次数：
  * 控制k个元素的翻转：**每k个元素只需要翻转k-1次**，单次翻转后递减计数。
  * 元素翻转后，位移、递减计数。

* 返回结果

**138_随机链表的复制**

* 创建新节点，将新节点填充到原始节点之后。
* 填充新节点的`random`，注意`random`可能为空。

```c++
while (nullptr != cur && nullptr != cur->next) {
    if (nullptr != cur->random) {
        cur->next->random = cur->random->next;
    }
    cur = cur->next->next;
}
```

* 拆分：

```cpp
Node dummy(0);
Node* nList = &dummy;
cur = head;
while (nullptr != cur && nullptr != cur->next) {
    // 获取新节点
    nList->next = cur->next;
    // 断开
    cur->next = cur->next->next;
    
    // 位移 
    nList = nList->next;
    cur = cur->next;
}
```

**146_LRU缓存：**

* HashMap + 双向链表
* 核心逻辑：
  * `get(int key)`：若存在，则将节点存放置链表头。
  * `push(int key, int value)`：若key存在，更新数据，若无则创建新节点，插入链表头部，同时插入至HashMap中。同时判断当前元素个数是否超出限制梳理，若有，获取末尾节点，移除链表中节点，根据节点中key移除hashmap中元素
* HashMap：`unordered_map<int, Node*> m_unMap; // key: key, value : Node*`
* 双向链表：

```cpp
typedef struct Node{
    Node* pre;
    NOde* next;
    int key;
    int value;
    Node(int key, int value):pre(nullptr), next(nullptr), key(key),value(value){}
}Node;
```

**23_合并K个有序链表**

* **分治+迭代：自底向上合并链表**
* 两两合并：`lists[0]`和`lists[1]`合并，合并后的链表保存在`lists[0]`中；把 `lists[2] `和 *lists*[3] 合并，合并后的链表保存在 `lists[2]` 中；依此类推。

* 四四合并：把 `lists[0]` 和 `lists[2]` 合并（相当于合并前四条链表），合并后的链表保存在 `lists[0]` 中；把 `lists[4]` 和 `lists[6]` 合并，合并后的链表保存在 `lists[4]` 中；依此类推。
* 八八合并：把 `lists[0]` 和~合并（相当于合并前八条链表），合并后的链表保存在 `lists[0]` 中；把 `lists[8]` 和 `lists[12]` 合并，合并后的链表保存在 `lists[8]` 中；依此类推。。

* 链表合并逻辑：**21_合并两个有序链表**

```cpp
for (int step = 1; step < lists.size(); step *= 2) {
    for (int i = 0; i < lists.size() - step; i += step*2) {
        lists[0] = merageList(lists[i], lists[i + step]);
    }
}
return lists[0];
```

### 2.9 链表

**94_二叉树的中序遍历：**

* 确认函数返回值和参数列表；
* 确认终止条件；
* 确认单次遍历的内容；

**104_二叉树的最大深度**

* 后续遍历

* 确认函数返回值`int`和参数列表`TreeNode*`;
* 确认终止条件：`if (nullptr == node) { return 0; }`
* 确认单次遍历内容：获取左右子树的深度，取最大值+1.

```cpp
int leftDepth = traversal(node->left);
int rightDepth = traversal(node->right);
return max(leftDepth, rightDepth) + 1;
```

**226_翻转二叉树**

* 前序遍历：

* 确认函数返回值类型`TreeNode*`和参数列表`TreeNode* node`;
* 确认终止条件`if (nullptr == node) { return node;}`
* 确认单次遍历内容：交换左右子节点，递归处理新的左右子节点。

```cpp
swap(node->left, node->right);
node->left = dfs(node->left);
node->right = dfs(node->right);
return node;
```

**101_对称二叉树**

* 前序遍历

* 确认函数返回值类型`bool`和参数列表`TreeNode* left, TreeNode* right`;
* 确认终止条件：
  * 节点为空：`true`
  * 左空右不空：`false`
  * 左不空右空：`false`
  * 左右节点元素不相等`false`
* 单次处理逻辑：两节点外侧比较，两节点内侧比较，返回结果。

```cpp
bool isOutSideSym = dfs(left->left, right->right);
bool isInSideSym = dfs(left->right, right->left);
return (isOutSideSym && isInSideSym);
```

**543_二叉树的直径**：

* 后续遍历

* 直径：**任意两节点的最长路径的长度**，不一定要经过根节点

* 使用成员遍历记录，实时更新。
* 返回值类型`int`参数类型`TreeNode*`;
* 终止条件：遇到空节点，返回0。
* 单次遍历执行内容：获取当前节点到到**左右节点的最大深度**。**更新最大直径**，返回**当前节点的最大深度**。

```cpp
int leftDiameter = dfs(node->left);
int rightDiameter = dfs(node->right);
maxDiameter = max(maxDiameter, leftDiameter + rightDiameter);

return max(leftDiameter, rightDiameter) + 1;
```

**102_二叉树的层序遍历**

* 使用`queue`，支持操作`push()`、`pop()`、`front()`。
* 填充元素，逐层遍历。

**108_将有序数组转化为二叉搜索树**

* 前序遍历

* 使用二分法，控制左右边界即可。

**98_验证二叉搜索树：**

* 中序遍历：
* 返回值类型`bool`，参数类型`TreeNode*`;
* 确认终止条件：空节点返回`true`；
* 单次遍历的内容：获取左侧结果，若记录遍历的上一个元素不为空，且上一个元素>=当前元素，返回`false`。获取右侧结果，返回整合结果。

```cpp
bool isLeftBTS = isVaildBTS(node->left);
if (nullptr != pre && pre->val >= node->val) {
    return false;
}
pre = node;
bool isRightBTS = isVaileBTS(node->right);
return (isLeftBTS && isRightBTS);
```

**199_二叉树右视图**

* 层序遍历，填充队列中最后一个元素。
* 递归：中右左
* 确认函数返回值`void`参数`TreeNode* node, int depth， vector<int>& ans`;
* 确认终止条件：空节点返回`if (nullptr == node) {return;}`;
* 单次执行逻辑：首次到达深度，填充元素。优先遍历右子树。

```cpp
if (depth == ans.size()) {
    ans.push_bakc(node->val);
}
dfs(node->right, depth + 1, ans);
dfs(node->left, depth + 1, ans);
return;
```

**114_二叉树展开为链表**

* 后续遍历：
* 确认返回值类型`TreeNode*`和参数列表 `TreeNode*`

* 终止条件：遇到空节点返回空指针
* 单次遍历过程：
  * 获取左子树尾节点
  * 获取右子树尾节点
  * 左子树尾节点指向右子树
  * 当前节点的右节点指向左子树。
  * 左节点制空。
  * 优先返回右子树尾节点，左子树尾节点，当前节点。

```cpp
TreeNode* leftTail = dfs(node->left);
TreeNode* rightTail = dfs(node->right);

if (nullptr != leftTail) {
    leftTail->right = node->right;
    node->right = node->left;
    node->left = nullptr;
}

return (nullptr != rightTail) ? rightTail : (nullptr != leftTail) ? leftTail : node;
```

**105_从前序与中序遍历序列构造二叉树**

* 使用区间：**左闭右开**，好处：右边不用取。

* 方便计算中序左子树长度，使用`unordered_map<int, int> index`记录中序遍历的元素与对应长度。
* 返回值类型`TreeNode* `参数列表：
  * 前序遍历数组、前序开始位置、前序结束位置（开区间）
  * 中序遍历数组、中序开始位置、中序结束位置（开区间）
  * 中序元素与下标对应`unordered_map<int, int>&`
* 终止条件：前闭后开区间，相等返回。
* 单次遍历逻辑：
  * 以**前缀的首元素**计算**左子树的中序数组对应长度**，创建左子树
  * 基于上述计算，创建右子树
  * 返回基于当前元素，左右子节点的新元素。

**437_路径总和 III**

解法：560_和为K的子数组相同，注意回溯掉当前节点影响。

* 前序遍历

* HashMap初始化`unordered_map<long long ,int> sum_map;sum_map[0] = 1;`
* 返回值类型`void`参数列表`(TreeNode* node, int sum, int tragetsum, unordered_map<long long, int>& sum_map);`
* 终止条件：空节点直接返回
* 单次遍历逻辑：
  * 计算前缀和
  * 统计`preSUm - tragetSum`的次数
  * 前缀和次数++
  * 遍历左右子树
  * **前缀和次数--**；

**124_二叉树中的最大路径和**

解法：543_二叉树的直径 演变

* 实时计算路径和，记录最大值。

* 对于结果的返回要考虑：所有元素为负数的情况，要使用**0作为限制点**。
