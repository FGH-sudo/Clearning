#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
typedef int bool;

/*管理员状态*/
typedef enum
{
    ROOT,
    MANAGER
} Identity; // （ROOT-超级  MANAGER-普通）

/*管理员数据结构体*/
typedef struct
{
    char m_id[21];     // 管理员账号
    char m_passwd[9];  // 管理员密码
    bool m_islocked;   // 身份状态是否被锁
    Identity identity; // 管理员身份
} Manager;

/*链表节点*/
typedef struct node
{
    Manager manager;
    struct node *next;
} Node;

typedef struct
{
    Node *head;     // 头指针（指向链表头节点）
    int m_quantity; // 管理员数量
} Manager_List;

/*1.初始化业务*/
void Init(Manager_List **mp)
{
    *mp = (Manager_List *)malloc(sizeof(Manager_List));
    (*mp)->head = NULL;
    (*mp)->m_quantity = 0;
}

/*2.管理员注册业务*/
bool m_register(Manager_List *mp)
{
    Node *p;
    char a[21];
    char b[9];
    int choice;
    if (mp->head == NULL)
    {
        p = (Node *)malloc(sizeof(Node));
        while (1)
        {
            printf("请输入管理员账号（不超过20位）:");
            scanf("%s", a);
            if (strlen(a) > 20)
            {
                printf("id过长！\n");
            }
            else
            {
                strcpy(p->manager.m_id, a);
                break;
            }
        }
        while (1)
        {
            printf("请输入管理员密码（8位）:");
            scanf("%s", b);
            if (strlen(b) != 8)
            {
                printf("密码长度不符！\n");
            }
            else
            {
                strcpy(p->manager.m_passwd, b);
                break;
            }
        }
        printf("请输入管理员身份 (1 超级管理员 or 2 普通管理员):");
        scanf("%d", &choice);
        p->manager.identity = choice == 1 ? ROOT : MANAGER;
        p->manager.m_islocked = FALSE;
        mp->head = p;
        p->next = NULL;
        mp->m_quantity++;
    }
    else
    {
        Node *r;
        r = mp->head;
        while (r->next != NULL)
        {
            r = r->next;
        }
        p = (Node *)malloc(sizeof(Node));
       while (1)
        {
            printf("请输入管理员账号（不超过20位）:");
            scanf("%s", a);
            if (strlen(a) > 20)
            {
                printf("id过长！\n");
            }
            else
            {
                strcpy(p->manager.m_id, a);
                break;
            }
        }
        while (1)
        {
            printf("请输入管理员密码（8位）:");
            scanf("%s", b);
            if (strlen(b) != 8)
            {
                printf("密码长度不符！\n");
            }
            else
            {
                strcpy(p->manager.m_passwd, b);
                break;
            }
        }
        printf("请输入管理员身份 (1 超级管理员 or 2 普通管理员):");
        scanf("%d", &choice);
        p->manager.identity = choice == 1 ? ROOT : MANAGER;
        p->manager.m_islocked = FALSE;
        r->next = p;
        p->next = NULL;
        mp->m_quantity++;
    }
    printf("注册成功!\n");
    return TRUE;
}