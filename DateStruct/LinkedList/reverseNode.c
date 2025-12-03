
#include <stdlib.h>
#include <stdio.h>


struct ListNode {
    	int val;
    	struct ListNode *next;

};

/**
 * @brief 反转链表
 * @param [in] ListNode* list
 * @param [in/out] int* size
*/
struct ListNode* reversetList(struct ListNode* list, int* size)
{
    if (NULL == list)
    {
        return 0;
    }

    typedef struct ListNode ListNode;
    ListNode* pre = NULL;
    ListNode* cur = list;
    ListNode* temp = NULL;

    int sizeList = 0;

    while (NULL != cur)
    {
        // 获取中间变量
        temp = cur->next;
        // 反转
        cur->next = pre;

        // 双指针移动
        pre = cur;
        cur = temp;

        ++sizeList;
    }
    *size = sizeList;

    return pre;
}


/**
 * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
 *
 * 
 * @param head1 ListNode类 
 * @param head2 ListNode类 
 * @return ListNode类
 */

struct ListNode* addInList(struct ListNode* head1, struct ListNode* head2 ) {
    // write code here
    if (NULL == head1 || NULL == head2)
    {
        return NULL;
    }

    // 将两个链表反转
    typedef struct ListNode ListNode;
    int size1 = 0;
    ListNode* new1 =  reversetList(head1, &size1);
    int size2 = 0;
    ListNode* new2 = reversetList(head2, &size2);

    // 使用较长的链表作为结果
    ListNode* sum = NULL;
    ListNode* base = NULL;
    
    if (size1 >= size2)
    {
        sum = new1;
        base = new2;
    }
    else
    {
        sum = new2;
        base = new1;
    }


    // 按照base的长度进行累加
    int sumNum = 0;
    int currentNum = 0;
    // 标志位判断是否需要进位
    int needUpdate = 0;

    ListNode* result = sum;
    while(NULL != base)
    {
        sumNum = base->val + sum->val;
        // 判断本次运算是否要考虑上一位的进位
        if (1 == needUpdate)
        {
            sumNum++;
            needUpdate = 0;
        }

        // 判断本次运算是否要向上进位
        if (0 < sumNum / 10)
        {
            // 需要进位，保留当前位。改变进位标识符。
            currentNum = sumNum%10;
            needUpdate = 1;
        }
        else
        {
            currentNum = sumNum;
            needUpdate = 0;
        }

        // 修改sum当前位的数据
        sum->val = currentNum;

        // 进行下一位运算
        base = base->next;
        sum = sum->next;
    }

    // 低位数计算完成后，验证是否还需要进位
    while(NULL != sum)
    {
        sumNum = sum->val;
        if (1 == needUpdate)
        {
            sumNum++;
            needUpdate = 0;
        }

        // 判断本次运算是否要向上进位
        if (0 < sumNum / 10)
        {
            // 需要进位，保留当前位。改变进位标识符。
            currentNum = sumNum%10;
            needUpdate = 1;
        }
        else
        {
            currentNum = sumNum;
            needUpdate = 0;
        }

        // 修改sum当前位的数据
        sum->val = currentNum;

        // 位移
        sum = sum->next;
    }

    // 将sum表重新反转
    int finalLength = 0;
    ListNode* resultFinal = reversetList(result, &finalLength);

    // 在此类情况下：{1} {99} = {001} sum已有节点无法完整表达内容，故需要添加新的节点
    if (1 == needUpdate)
    {
        ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
        if (NULL == newNode)
        {
            return NULL;
        }

        newNode->val = 1;
        newNode->next = resultFinal;
        return newNode;
    }

    return resultFinal;
}

int main()
{
    // 
    return 0;
}