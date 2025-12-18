/**
 * @file LongestConsecutiveSequence.cpp
 * @brief LeetCode Problem 128: Longest Consecutive Sequence
 * @author Lee
 * @date 2025-12-15
 */

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

/**
 * @brief 统计最长连续序列的长度
 * @param [in] vector<int> nums 输入的整数数组
 * @return int 最长连续序列的长度
 */
 int longestConsecutive(vector<int>& nums){
    if (0 == nums.size()){
        return 0;
    }

    unordered_set<int> set(nums.begin(), nums.end());
    int maxLength = 0;
    int currLenght = 0;
    int currentNum = 0;

    // 遍历去重的每个数字，寻找连续序列的起点
    for (const int& num : set){
        // 仅当num是序列的起点时才进行处理
        if (set.find(num - 1) == set.end()){
            currentNum = num;
            currLenght = 1;

            // 继续查询，直至不连续
            while (set.find(currentNum + 1) != set.end()){
                ++currentNum;
                ++currLenght;
            }

            maxLength = max(currLenght, maxLength);
        }
    }
    
    return maxLength;
 }

 /**
  * @brief 主函数，测试longestConsecutive函数
  * @return int 程序退出状态码
  */
int main()
{
    vector<int> nums = {100, 4, 200, 1, 3, 2};
    int result = longestConsecutive(nums);
    cout << "The length of the longest consecutive sequence is: " << result << endl;
    return 0;
}

