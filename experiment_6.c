#include <stdio.h>
#include <stdlib.h>

typedef struct GoodsList {
    int data;
    struct GoodsList* next;
} GoodsList;

// 初始化链表，直接在传入的head节点上操作
void init_list(GoodsList *L) {
    L->data = 10;  // 设置头节点的数据
    L->next = NULL;  // 设置头节点的next指针为空，表示链表末尾
}

int main() {
    GoodsList *head;  // 为head分配内存

    if (head == NULL) {
        printf("内存分配失败！\n");
        return -1;  // 如果内存分配失败，退出程序
    }

    // 初始化链表
    init_list(head);  // 直接操作已经分配内存的head节点

    // 输出链表头节点的数据
    printf("链表头节点的数据: %d\n", head->data);

    // 释放分配的内存
    free(head);

    return 0;
}