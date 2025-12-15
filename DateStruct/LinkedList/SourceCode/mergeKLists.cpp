/***
 * @file mergeKLists.cpp
 * @brief LeetCode Problem 23: Merge k Sorted Lists
 * @author Lee
 * @date 2025-12-15
 * 
 */

#include <iostream>
#include <vector>

using namespace std;

struct ListNode{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

/**
 * @brief 合并 k 个排序链表
 * @param [in] lists: 链表数组
 * @return 合并后的排序链表头指针
 */
ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (0 == lists.size()){
        return nullptr;
    }
    
    ListNode* fakeHead = new ListNode();
    if (nullptr == fakeHead)
    {
        return nullptr;
    }
    fakeHead->next = lists[0];

    // 合并链表
    ListNode* cur = nullptr;
    // 插入链表
    ListNode* insert = nullptr;
    
    // 依次将每个链表插入到合并链表中
    for (int i = 1; i < lists.size(); ++i){
        cur = fakeHead;
        insert = lists[i];
        // 遍历合并链表
        while (nullptr != cur->next){
            // 若插入链表为空，跳出循环
            if (nullptr == insert){
                break;
            }
            // 若插入元素 <= 当前元素的下一个元素，插入当前节点之后
            if (insert->val <= cur->next->val){
                // 记录临时节点
                ListNode* tempInsert = insert;
                // 插入链表向后走一步
                insert = insert->next;
                // 插入元素
                tempInsert->next = cur->next;
                cur->next = tempInsert;
                // 向后走一步
                cur = cur->next;
            }
            else{
                // 当前插入节点元素值 > 合并节点的下一个节点的值。
                cur = cur->next;
            }
        }
        // 插入链表可能并没有遍历完成,将其剩余的所有元素加入之合并链表之后
        if (nullptr != insert){
            cur->next = insert;
        }
    }
    ListNode* result = fakeHead->next;
    delete fakeHead;
    return result;
}

/**
 * @brief 主函数
 * @return int
 */
int main(){
    // 测试用例
    ListNode* l1 = new ListNode(1, new ListNode(4, new ListNode(5)));
    ListNode* l2 = new ListNode(1, new ListNode(3, new ListNode(4)));
    ListNode* l3 = new ListNode(2, new ListNode(6));
    vector<ListNode*> lists = {l1, l2, l3};

    ListNode* mergedList = mergeKLists(lists);

    // 输出合并后的链表
    ListNode* current = mergedList;
    while (current != nullptr) {
        cout << current->val << " -> ";
        current = current->next;
    }
    cout << "nullptr" << endl;

    // 释放内存
    current = mergedList;
    while (current != nullptr) {
        ListNode* temp = current;
        current = current->next;
        delete temp;
    }

    return 0;
}