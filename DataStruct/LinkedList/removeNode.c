#include <stdio.h>
#include <stdlib.h>

typedef struct listNode
{
    int val;
    struct listNode* prev;
    struct listNode* next;
}listNode;


typedef struct {
    int size;
    listNode* node;
} MyLinkedList;


MyLinkedList* myLinkedListCreate() {
    // 创建节点内容
    MyLinkedList* linkList = (MyLinkedList*)malloc(sizeof(MyLinkedList));
    if (NULL == linkList)
    {
        return NULL;
    }
    listNode* head = (listNode*)malloc(sizeof(listNode));
    if (NULL == head)
    {
        free(linkList);
        linkList = NULL;
        return NULL;
    }

    // 填充节点信息 第一个节点是虚拟头，不存储数据
    linkList->size = 0;
    linkList->node = head;

    head->val = 0;
    head->prev = NULL;
    head->next = NULL;
    return linkList;

}

int myLinkedListGet(MyLinkedList* obj, int index) {
    
    // 入参校验
    if (NULL == obj || NULL == obj->node || obj->size <= index || 0 > index)
    {
        // 返回无效下标
        perror("get invalid index\n");
        return -1;
    }

    listNode* temp = obj->node;
    while(0 <= index--)
    {
        temp = temp->next;
    }

    if (NULL != temp)
    {
        return temp->val;
    }
    else
    {
        printf("list is NULL");
        return -1;
    }
}

void myLinkedListAddAtHead(MyLinkedList* obj, int val) {
    // 入参校验
    if (NULL == obj || NULL == obj->node)
    {
        return;
    }

    // 构建新的节点
    listNode* newNode = (listNode*)malloc(sizeof(listNode));
    if (NULL == newNode)
    {
        return;
    }

    // 更新新节点的指向
    newNode->val = val;

    // 向前指向虚拟链表头
    newNode->prev = obj->node;

    // 向后指向之前表头
    if (NULL != obj->node->next)
    {
        newNode->next = obj->node->next;
    }
    else
    {
        newNode->next = NULL;
    }

    // 更新老链表头的指向
    if (NULL != obj->node->next)
    {
        obj->node->next->prev = newNode;
    }

    // 更新虚拟链表头的指向
    obj->node->next = newNode;

    // 更新链表长度
    obj->size++;

    return;
}

void myLinkedListAddAtTail(MyLinkedList* obj, int val) {
    if (NULL == obj || NULL == obj->node)
    {
        return;
    }

    // 获取链表长度
    int size = obj->size;

    // 遍历链表直至末尾节点
    listNode* nodeTemp = obj->node;
    while(0 < size--)
    {
        nodeTemp = nodeTemp->next;
    }

    // 创建新的节点
    listNode* newNode = (listNode*)malloc(sizeof(listNode));
    if (NULL != newNode && NULL != nodeTemp)
    {
        // 填充数据与指向
        newNode->val = val;
        newNode->prev = nodeTemp;
        newNode->next = NULL;

        // 修改链表尾节点的指向
        nodeTemp->next = newNode;

        // 增减链表长度
        obj->size++;
    }
    return;
}

void myLinkedListAddAtIndex(MyLinkedList* obj, int index, int val) {
    
    // 入参校验
    if (NULL == obj || NULL == obj->node || index < 0 || index > obj->size)
    {
        return;
    }

    // 移动至index的前一个节点
    listNode* nodeTemp = obj->node;
    while(0 < index--)
    {
        nodeTemp = nodeTemp->next;
    }

    // 创建新节点
    listNode* newNode = (listNode*)malloc(sizeof(listNode));
    if (NULL == newNode)
    {
        return;
    }

    // 填充数据
    newNode->val = val;
    newNode->prev = nodeTemp;
    newNode->next = nodeTemp->next;

    // 对老节点进行修改
    if (NULL != nodeTemp->next)
    {
        nodeTemp->next->prev = newNode;
    }

    // 修改前一个节点的 next 指针
    nodeTemp->next = newNode;
    
    // 增减链表长度
    obj->size++;
}

void myLinkedListDeleteAtIndex(MyLinkedList* obj, int index) {
    // 入参校验
    if (NULL == obj || NULL == obj->node || index < 0 || index >= obj->size)
    {
        return;
    }

    // 获取临时节点
    listNode* nodeTempPrev = obj->node;

    // 偏移至要移除节点的前一个节点
    while (index-- > 0)
    {
        nodeTempPrev = nodeTempPrev->next;
    }

    listNode* toDelete = nodeTempPrev->next;
    if (toDelete == NULL) {
        return;
    }

    // 维护 next 指针
    nodeTempPrev->next = toDelete->next;
    // 维护 prev 指针
    if (toDelete->next != NULL) {
        toDelete->next->prev = nodeTempPrev;
    }

    free(toDelete);
    obj->size--;
    return;
}

void myLinkedListFree(MyLinkedList* obj) {
    if (NULL != obj)
    {
        if (NULL != obj->node)
        {
            listNode* temp = obj->node;
            obj->node = obj->node->next;
            free(temp);
        }
        free(obj);
        obj = NULL;
    }
    return;
}

void printLinkList(MyLinkedList* obj)
{
    if (NULL == obj || NULL == obj->node)
    {
        return;
    }

    listNode* temp = obj->node->next;
    printf("link list val:");
    while (NULL != temp)
    {
        printf("%d ", temp->val);
        temp = temp->next;
    }
    printf("\n");
    return;
}

/**
 * Your MyLinkedList struct will be instantiated and called as such:
 * MyLinkedList* obj = myLinkedListCreate();
 * int param_1 = myLinkedListGet(obj, index);
 
 * myLinkedListAddAtHead(obj, val);
 
 * myLinkedListAddAtTail(obj, val);
 
 * myLinkedListAddAtIndex(obj, index, val);
 
 * myLinkedListDeleteAtIndex(obj, index);
 
 * myLinkedListFree(obj);
*/

int main(int argc, char* argv[])
{
    MyLinkedList* obj = myLinkedListCreate();
    myLinkedListAddAtHead(obj, 1);
    myLinkedListAddAtTail(obj, 3);
    myLinkedListAddAtIndex(obj, 1, 2);    // 链表变为1->2->3
    printLinkList(obj);
    int param_1 = myLinkedListGet(obj, 1);              // 返回2
    printf("get index 1 val=%d\n", param_1);
    myLinkedListDeleteAtIndex(obj, 1);    // 现在链表是1->3
    param_1 = myLinkedListGet(obj, 1);              // 返回3
    printf("get index 1 val=%d\n", param_1);
    myLinkedListFree(obj);
    return 0;
}