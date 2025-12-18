/**
 * @file TrappingRainWater.cpp
 * @brief LeetCode 42 - Trapping Rain Water
 * @author Lee
 * @date 2025-12-16
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief 计算雨水的总量
 * @param [in] vector<int>& height: 地形高度数组
 * @return int: 可以收集的雨水总量
 */
int trap(vector<int>& height) {
    // 入参校验
    if (height.empty()){
        return 0;
    }

    // 定义双指针，移动至第一个不为0的元素
    int left = 0;
    int right = height.size() - 1;
    int area = 0;
    int left_max = height[left];
    int right_max = height[right];

    while (left < right) {
        left_max = max(left_max, height[left]);
        right_max = max(right_max, height[right]);

        // 若左侧最大值小于右侧最大值，则说明左侧可以存水。
        if (left_max < right_max)
        {
            // 计算左侧存水量
            area += left_max - height[left];
            left++;
        }
        else
        {
            // 计算右侧存水量
            area += right_max - height[right];
            right--;
        }
    }

    return area;
}

/**
 * @brief 主函数
 */
int main() {
    vector<int> height = {4,2,3};
    int result = trap(height);
    cout << "Total trapped rainwater: " << result << endl;
    return 0;
}