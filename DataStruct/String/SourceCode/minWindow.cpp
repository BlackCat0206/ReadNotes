/**
 * @file minWindow.cpp
 * @brief LeetCode problem 76. Minimum Window Substring
 * @author Lee
 * @date 2025-12-12
 */

#include <string>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

/**
 * @brief 寻找字符串s中包含t的最小窗口
 * @param s 输入字符串
 * @param t 目标字符串
 * @return 最小窗口子串
 */
string minWindow(string s, string t) {
    if (s.empty() || t.empty()) return "";

    int n = s.size(), m = t.size();
    int start = 0, minLen = INT_MAX, left = 0, right = 0;
    int count = 0;

    vector<int> charCount(128, 0); // ASCII字符集大小
    for (char c : t) {
        charCount[c]++;
    }

    while (right <= n){
        // 扩展右边界
        if (charCount[s[right]] > 0) 
        {
            // 如果当前字符在t中，增加计数
            count++;
        }
        charCount[s[right]]--;
        right++;
        // 当窗口包含t的所有字符时，尝试收缩左边界
        while (count == m){
            if (minLen > right - left){
                minLen = right - left;
                start = left;
            }
            // 收缩左边界
            charCount[s[left]]++;
            if (charCount[s[left]] > 0) {
                count--; // 如果左边界字符在t中，减少计数
            }
            left++;
        }
    }
    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}

string minWindow2(string s, string t)
{
            // write code here
        if (s.empty() || t.empty()){
            return "";
        }

        vector<int> charCount(128, 0);
        for (char c : t) 
        {
            ++charCount[c];
        }

        // 滑动窗口左右边界
        int left = 0;
        int right = 0;
        // 计数
        int count = 0;
        // 字串的开始
        int start = 0;

        // 字符长度
        int sizeS = s.size();
        int sizeT = t.size();

        // 最小长度
        int minLen = INT_MAX;

        while(right <= sizeS)
        {
            if (charCount[s[right]] > 0)
            {
                ++count;
            }
            --charCount[s[right]];
            ++right;

            // 匹配结束，收缩左边界
            while (sizeT == count) 
            {
                if (minLen > right - left)
                {
                    minLen = right - left;
                    start = left;
                }

                ++charCount[s[left]];
                if (charCount[s[left]] > 0)
                {
                    --count;
                }
                ++left;
            }
        }
        cout << "minLen: " << minLen << endl;
        return minLen == INT_MAX ? "" : s.substr(start, minLen);
}

/**
 * @brief 测试函数
 * @param argc 参数个数
 * @param argv 参数数组
 * @return 0 成功
 */
int main(int argc, char* argv[]) {
    string s = "ADOBECODEBANC";
    string t = "ABC";

    string result = minWindow(s, t);
    cout << "Minimum window substring: " << result << endl;

    string result2 = minWindow2(s, t);
    cout << "Minimum window substring (method 2): " << result2 << endl;

    return 0;
}