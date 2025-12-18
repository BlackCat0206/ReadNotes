/**
 * @file matrix.cpp
 * @brief LeetCode problem #54 - Spiral Matrix
 * @author Lee
 * @date 2025-12-12
 * 
 */

#include <iostream>
#include <vector>

using namespace std;

vector<int> spiralOrder(const vector<vector<int>>& matrix)
{
    if (matrix.empty()){
        return {};
    }

    // 设置边界条件
    int left = 0;
    int right = matrix[0].size() - 1;
    int top = 0;
    int bottom = matrix.size() - 1;

    // 设置结果
    vector<int> res;

    while(left <= right && top <= bottom){
        // 上侧：从左到右
        for (int i = left; i <= right; i++)
        {
            res.push_back(matrix[top][i]);
        }

        // 右侧：从上到下
        for (int j = top + 1; j <= bottom; j++)
        {
            res.push_back(matrix[j][right]);
        }

        // 下侧：从右向左；注意规避若只传入奇数行，重复填入的问题（top < bottom）
        for (int i = right - 1; i >= left && top != bottom; i--){
            res.push_back(matrix[bottom][i]);
        }
        
        // 左侧：从下到上；注意规避若只传入奇数行，重复添加问题（left < right）
        for (int j = bottom - 1; j > top && left != right; j--){
            res.push_back(matrix[j][left]);
        }

        // 缩小边界
        left++;
        right--;
        top++;
        bottom--;
    }
    return res;
}

/***
 * @brief 主函数
 * 测试spiralOrder函数
 * 
 */
int main()
{
    vector<vector<int>> matrix = {
        {1, 2, 3}
    };

    vector<int> result = spiralOrder(matrix);

    cout << "Spiral Order: ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}