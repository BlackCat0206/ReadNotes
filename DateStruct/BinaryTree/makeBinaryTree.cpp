/***
 * @file makeBinaryTree.cpp
 * @brief 本文主要适用通过二叉树的中序遍历和后序遍历来构建二叉树。
 * @author Lee
 * @date 2025-12-06
 * @details:
 * 给定二叉树的前序遍历和中序遍历，构建二叉树。
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* traversal(vector<int>& inorder, vector<int>& postorder)
{
    // 根据后序遍历的最后一个节点进行切割。
    if (inorder.empty() || postorder.empty()) {
        return nullptr;
    }

    // 后序遍历的最后一个元素是根节点
    int rootVal = postorder.back();
    // 若只剩一个节点，其本身为叶子节点
    if (postorder.size() == 1) {
        postorder.pop_back(); // 移除后序遍历的最后一个元素
        return new TreeNode(rootVal);
    }
    postorder.pop_back(); // 移除后序遍历的最后一个元素


    // 创建根节点
    TreeNode* root = new TreeNode(rootVal);

    // 在中序遍历中查找根节点的位置
    auto it = find(inorder.begin(), inorder.end(), rootVal);
    if (it == inorder.end()) {
        return nullptr; // 根节点不在中序遍历中，返回空指针
    }
    int index = distance(inorder.begin(), it);

    // 切割中序遍历和后序遍历 计算区间：前闭后开
    // [begin, begin + index) 是左子树的中序遍历
    vector<int> leftInorder(inorder.begin(), inorder.begin() + index);
    // [begin + index + 1, end) 是右子树的中序遍历
    vector<int> rightInorder(inorder.begin() + index + 1, inorder.end());

    // 切割后序遍历 计算区间：前闭后开 
    int leftSize = leftInorder.size();
    // [begin, begin + leftSize) 是左子树的后序遍历
    vector<int> leftPostorder(postorder.begin(), postorder.begin() + leftSize);
    // [begin + leftSize, end) 是右子树的后序遍历
    vector<int> rightPostorder(postorder.begin() + leftSize, postorder.end());

    // 递归构建左子树和右子树``
    root->left = traversal(leftInorder, leftPostorder);
    root->right = traversal(rightInorder, rightPostorder);
    return root;
}


void traversal(TreeNode* root, vector<int>& result)
{
    if (root == nullptr) {
        return;
    }
    traversal(root->left, result);
    result.push_back(root->val);
    traversal(root->right, result);
    return;
}


int main(int argc, char* argv[])
{
    // 这里可以添加代码来测试构建二叉树的功能
    // 例如，定义前序遍历和中序遍历的数组，然后调用构建函数
    vector<int> inorder = {9, 3, 15, 20, 7};
    vector<int> postorder = {9, 15, 7, 20, 3};

    TreeNode* root = traversal(inorder, postorder);
    // 这里可以添加代码来验证构建的二叉树是否正确
    vector<int> result;
    traversal(root, result);
    cout << "Inorder traversal of the constructed tree: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;
    return 0;
}