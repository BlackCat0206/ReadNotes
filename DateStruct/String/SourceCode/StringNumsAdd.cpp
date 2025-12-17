/**
 * @file StringNumsAdd.cpp
 * @brief This file contains the implementation of a function that adds two numbers represented as strings.
 * @version 1.0
 * @date 2025-12-12
 * @author Lee
 */

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;



string solve(string s, string t)
{
    // Input validation: ensure only digits
    auto isValid = [](const string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    };
    if (!isValid(s) || !isValid(t)) {
        return "Invalid input";
    }

    // Ensure s is the shorter string
    if (s.length() > t.length()) {
        swap(s, t);
    }

    string result;
    int carry = 0;
    int i = s.size() - 1;  // Start from the end
    int j = t.size() - 1;

    // Add digits from right to left
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += s[i--] - '0';
        if (j >= 0) sum += t[j--] - '0';
        carry = sum / 10;
        result.push_back('0' + (sum % 10));
    }

    // Reverse the result to get the correct order
    reverse(result.begin(), result.end());
    return result;
}

/**
 * @brief 测试主函数
 * @param void
 * 
 */
int main()
{
    string s1 = "456";
    string s2 = "77";
    cout << solve(s1, s2) << endl; // 输出: 533

    s1 = "999";
    s2 = "1";
    cout << solve(s1, s2) << endl; // 输出: 1000

    s1 = "0";
    s2 = "0";
    cout << solve(s1, s2) << endl; // 输出: 0

    return 0;
}
