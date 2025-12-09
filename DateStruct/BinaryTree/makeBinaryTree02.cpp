/**
 * @file makeBinaryTree02.cpp
 * @brief 本文主要适用通过二叉树的前序遍历和中序遍历来构建二叉树。之后输出二叉树的右视图。
 * @author Lee
 * @date 2025-12-08
 * @details:
 * 给定二叉树的前序遍历和中序遍历，构建二叉树，并输出二叉树的右视图。
 * 
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct TreeNode{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * @brief 根据前序和中序遍历构建二叉树
 * @param [in] preorder 前序遍历
 * @param [in] inorder 中序遍历
 * @return 返回构建好的二叉树的根节点指针
 */
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder)
{
    // 如果前序遍历或中序遍历为空，返回空指针
    if (preorder.empty() || inorder.empty()) {
        return nullptr;
    }

    // 如果前序遍历或中序遍历数组大小为1，返回叶子节点
    if (1 == inorder.size()) 
    {
        return new TreeNode(inorder[0]);
    }

    // 前序遍历的第一个元素是根节点
    int rootVal = preorder[0];
    TreeNode* root = new TreeNode(rootVal);

    // 在中序遍历中查找根节点的位置
    auto it = find(inorder.begin(), inorder.end(), rootVal);
    if (it == inorder.end()) {
        return nullptr; // 根节点不在中序遍历中，返回空指针
    }
    int index = distance(inorder.begin(), it);

    // 切割中序遍历和前序遍历
    // [begin, begin + index) 是左子树的中序遍历
    vector<int> leftInorder(inorder.begin(), inorder.begin() + index);
    // [begin + index + 1, end) 是右子树的中序遍历
    vector<int> rightInorder(inorder.begin() + index + 1, inorder.end());

    // 切割前序遍历
    // [begin + 1, begin + 1 + leftSize) 是左子树的前序遍历
    int leftSize = leftInorder.size();
    vector<int> leftPreorder(preorder.begin() + 1, preorder.begin() + 1 + leftSize);
    // [begin + 1 + leftSize, end) 是右子树的前序遍历
    vector<int> rightPreorder(preorder.begin() + 1 + leftSize, preorder.end());

    // 递归构建左子树和右子树
    root->left = buildTree(leftPreorder, leftInorder);
    root->right = buildTree(rightPreorder, rightInorder);
    return root;
}

/**
 * @brief 通过层序遍历输出二叉树的右视图
 * @param [in] root 二叉树的根节点
 * @param [out] result 存储右视图的结果
 */
vector<int> rightSideView(TreeNode* root)
{
    vector<int> result;
    if (root == nullptr) {
        return result;
    }

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) 
    {
        int size = q.size();
        for (int i = 0; i < size; ++i) 
        {
            TreeNode* node = q.front();
            q.pop();
            // 如果是当前层的最后一个节点，添加到结果中
            if (i == size - 1) {
                result.push_back(node->val);
            }
            // 将左子节点和右子节点入队
            if (node->left) {
                q.push(node->left);
            }
            if (node->right) {
                q.push(node->right);
            }
        }
    }
    return result;
}

/**
 * @brief [辅助函数] 计算二叉树的每一层的节点数
 * @param [in] level 二叉树的层数
 * @return 返回节点总数
 */
int countNodesAtLevel(int level)
{
    if (level <= 0) {
        return 0;
    }
    return (1 << level) - 1; // 2^level - 1
}