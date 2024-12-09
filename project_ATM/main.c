#include "manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    Manager_List *manager_list;
    int choice1, choice2;
    Init(&manager_list);
    printf("欢迎使用ATM机！\n");
    while (1)
    {
        printf("请选择您的身份（输入0退出系统）\n");
        printf("管理员(1)\n");
        printf("储户(2)\n");
        scanf("%d", &choice1);
        switch (choice1)
        {
        case 1:
        {
            while (1)
            {
                printf("1.注册\n");
                printf("登录\n");
                scanf("%d", &choice2);
                m_register(manager_list);
                break;
            }
        }
        case 2:
        {
            break;
        }
        case 0:
        {
            printf("您已退出该系统！\n");
            return 0;
        }
        }
    }
}

/**************************************************************************************************/
/**************************************************************************************************/
/*1.初始化业务*/
void Init(Manager_List **mp)
{
    *mp = (Manager_List *)malloc(sizeof(Manager_List));
    (*mp)->head = NULL;
    (*mp)->m_quantity = 0;
}

/*2.管理员注册业务*/
bool M_Register(Manager_List *mp)
{
    Node *p;
    char a[21];
    char b[9];
    int choice;
    p = (Node *)malloc(sizeof(Node));
    if (mp->head == NULL)
    {
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

/*3.管理员登录业务*/
bool M_Login(Manager_List *mp, char *id, char *passwd)
{
    if (mp->head == NULL)
    {
        printf("该管理员不存在！\n");
        return FALSE;
    }
    Node *p = mp->head;
    static int times = 0;
    while (p != NULL)
    {
        if (strcmp(p->manager.m_id, id) == 0 && p->manager.m_islocked == TRUE)
        {
            printf("普通管理员：%s已被锁！！！\n", p->manager.m_id);
            return FALSE;
        }
        else if (strcmp(p->manager.m_id, id) == 0 && strcmp(p->manager.m_passwd, passwd) == 0 && !p->manager.m_islocked)
        {
            printf("登录成功！\n");
            times = 0;
            return TRUE;
        }
        else if (strcmp(p->manager.m_id, id) == 0 && strcmp(p->manager.m_passwd, passwd) != 0 && !p->manager.m_islocked)
        {
            switch (p->manager.identity)
            {
            case MANAGER:
            {
                printf("密码错误！还有%d次机会\n", 3 - times);
                times += 1;
                if (times == ERRTIMES)
                {
                    p->manager.m_islocked = TRUE;
                    printf("普通管理员：%s已被锁！！！\n", p->manager.m_id);
                    times = 0;
                }
            }
            case ROOT:
            {
                printf("密码错误！\n");
            }
            }
            return FALSE;
        }
        else
        {
            p = p->next;
        }
    }
    printf("该管理员不存在！\n");
    return FALSE;
}

/*4.激活管理员*/