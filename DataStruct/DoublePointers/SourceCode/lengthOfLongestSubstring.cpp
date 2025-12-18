/**
 * @file lengthOfLongestSubstring.cpp
 * @brief LeetCode 3 - Longest Substring Without Repeating Characters
 * @author Lee
 * @date 2025-12-16
 */

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

/**
 * @brief 计算无重复字符的最长子串长度
 * @param [in] string s: 输入字符串
 * @return int: 最长无重复字符子串的长度
 */
int lengthOfLongestSubstring(string s) {
    // 入参校验
    if (s.empty()) {
        return 0;
    }

    // 定义返回结果
    int maxLength = 0;
    int curLength = 0;

    // 定义字符索引数组，初始化为-1
    int countChar[128] = {0};

    // 定义滑动窗口边界
    int left = 0;
    int right = 0;

    int size = s.size();
    
    // 滑动窗口遍历字符串
    while (right < size){
        // 当前字符未出现过，扩展窗口
        if (0 == countChar[s[right]]){
            // 标记当前字符已出现
            ++countChar[s[right]];
            ++curLength;
            // 更新最大长度
            maxLength = max(curLength, maxLength);
            ++right;
        }
        // 当前字符已出现过，收缩窗口
        else{
            // 移动左指针，直到当前字符未出现
            --countChar[s[left]];
            --curLength;
            
            ++left;
        }
    }
    return maxLength;
}

/**
 * @brief 主函数
 */
int main() {
    string s = "pwwkew";
    int result = lengthOfLongestSubstring(s);
    cout << "Length of longest substring without repeating characters: " << result << endl;
    return 0;
}