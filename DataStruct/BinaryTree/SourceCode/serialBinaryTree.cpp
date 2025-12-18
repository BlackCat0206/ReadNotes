/**
 * @file  serialBinaryTree.cpp
 * @brief LeetCode 297: Serialize and Deserialize Binary Tree
 * @author Lee
 * @date 2025-12-15
 */

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <cstring>

using namespace std;

struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(NULL), right(NULL) {}
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


/**
 * @brief 序列化二叉树（前序遍历）DFS
 * @param [in] TreeNode* root 二叉树根节点
 * @param [in,out] string& result 序列化结果字符串
 * @return void
 */
void serializeHelper(TreeNode* node, string& str){
    if (nullptr == node){
        str += "Null,";
    }
    else{
        str += to_string(node->val) + ",";
        serializeHelper(node->left, str);
        serializeHelper(node->right, str);
    }
    return;
}

/**
 * @brief 序列化二叉树
 * @param [in] TreeNode* root 二叉树根节点
 * @return string 序列化后的字符串
 */
string serialize(TreeNode* root) {
    string result = "";
    serializeHelper(root, result);
    return result;
}

/**
 * @brief 反序列化二叉树 （前序遍历） DFS
 * @param [in] list<string> strs 序列化各个节点元素
 * @return TreeNode* 构造的新节点
 */
TreeNode* deserializeHelper(list<string>& strs){
    if (strs.empty()){
        return nullptr;
    }

    if ("Null" == strs.front()){
        strs.erase(strs.begin());
        return nullptr;
    }

    TreeNode* node = new TreeNode(stoi(strs.front()));
    strs.erase(strs.begin());
    node->left = deserializeHelper(strs);
    node->right = deserializeHelper(strs);
    return node;
}

/**
 * @brief 反序列化二叉树
 * @param [in] string data 序列化后的字符串
 * @return TreeNode* 二叉树根节点
 */
TreeNode* deserialize(const string& data) {
    list<string> dataArray;
    string str;
    for (char c : data){
        if (',' == c){
            dataArray.push_back(str);
            str.clear();
        }
        else{
            str.push_back(c);
        }
    }

    if (!str.empty()){
        dataArray.push_back(str);
        str.clear();
    }
    return deserializeHelper(dataArray);
}

/**
 * @brief 另一种序列化二叉树的方法 （前序遍历）DFS
 * @param [in] TreeNode* root 二叉树根节点
 * @return char* 序列化后的字符串
 */
void serializeHelper2(TreeNode* node, string& str){
    if (nullptr == node){
        str += "#";
        return;
    }

    str += to_string(node->val) + ",";

    serializeHelper(node->left, str);
    serializeHelper(node->right, str);
    return;
}

/**
 * @brief 另一种序列化二叉树的方法
 * @param [in] TreeNode* root 二叉树根节点
 * @return char* 序列化后的字符串
 */
char* Serialize(TreeNode *root) {    
    if (nullptr == root){
        return nullptr;
    }
    string str = "";
    serializeHelper2(root, str);
    
    char* result = new char[str.size() + 1];
    strncpy(result, str.c_str(), str.length());
    result[str.length()] = '\0';
    
    return result;
}

/**
 * @brief 另一种反序列化二叉树的方法 （前序遍历） DFS
 * @param [in] char** str 序列化字符串指针的指针
 * @return TreeNode* 构造的新节点
 */
TreeNode* deserializeHelper2(char** str){
    if ('#' == **str){
        ++(*str);
        return nullptr;
    }
    int val = 0;
    while (',' != **str && '\0' != **str){
        val = val*10 + ((**str) - '0');
        ++(*str);
    }
    TreeNode* node = new TreeNode(val);
    if ('\0' == **str){
        return node;
    }
    else{
        ++(*str);
    }
    node->left = deserializeHelper(str);
    node->right = deserializeHelper(str);
    return node;
}


/**
 * @brief 另一种反序列化二叉树的方法
 * @param [in] char* str 序列化后的字符串
 * @return TreeNode* 二叉树根节点
 */
TreeNode* Deserialize(char *str) {
    if (nullptr == str || '#' == *str){
        return nullptr;
    }
    
    return deserializeHelper2(&str);
}



int main() {
    // 示例用法
    // [1,2,3,null,null,4,5,6,7]

    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(5);
    root->right->left->left = new TreeNode(6);
    root->right->left->right = new TreeNode(7);

    string serialized = serialize(root);
    cout << "Serialized: " << serialized << endl;

    TreeNode* deserializedRoot = deserialize(serialized);
    string reserialized = serialize(deserializedRoot);
    cout << "Reserialized: " << reserialized << endl;

    char* serialized2 = Serialize(root);
    cout << "Serialized2: " << serialized2 << endl;
    TreeNode* deserializedRoot2 = Deserialize(serialized2);
    char* reserialized2 = Serialize(deserializedRoot2);
    cout << "Reserialized2: " << reserialized2 << endl;
    delete[] serialized2;
    delete[] reserialized2;
    

    return 0;
}