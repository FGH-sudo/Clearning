#include "ATM.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*1.初始化*/
void M_Init(Manager_List **manager_list)
{
    *manager_list = (Manager_List *)malloc(sizeof(Manager_List));
    (*manager_list)->head = NULL;
    (*manager_list)->m_quantity = 0;
}

/*2.管理员注册*/
BOOL M_Register(Manager_List *manager_list)
{
    Node *p;
    char id[ID_SIZE + 1];
    char passwd[PASSWD_SIZE + 1];
    int choice;
    p = (Node *)malloc(sizeof(Node));
    if (manager_list->head == NULL) // 没有已注册的管理员
    {
        while (1)
        {
            printf("请输入管理员账号（不超过20位）:");
            scanf("%s", id);
            if (strlen(id) > ID_SIZE)
            {
                printf("id过长！\n");
            }
            else
            {
                strcpy(p->manager.m_id, id);
                break;
            }
        }
        while (1)
        {
            printf("请输入管理员密码（8位）:");
            scanf("%s", passwd);
            if (strlen(passwd) != PASSWD_SIZE)
            {
                printf("密码长度不符！\n");
            }
            else
            {
                strcpy(p->manager.m_passwd, passwd);
                break;
            }
        }
        printf("请输入管理员身份 (1 超级管理员 or 2 普通管理员):");
        scanf("%d", &choice);
        p->manager.identity = choice == 1 ? ROOT : MANAGER;
        p->manager.m_islocked = FALSE;
        manager_list->head = p;
        p->next = NULL;
        manager_list->m_quantity++;
    }
    else // 存在已注册的管理员
    {
        Node *r;
        r = manager_list->head;
        while (r->next != NULL)
        {
            r = r->next;
        } // 寻找链表中最后一位管理员，新增管理员插入到链表末端
        while (1)
        {
            printf("请输入管理员账号（不超过20位）:");
            scanf("%s", id);
            if (strlen(id) > ID_SIZE)
            {
                printf("id过长！\n");
            }
            else
            {
                strcpy(p->manager.m_id, id);
                break;
            }
        }
        while (1)
        {
            printf("请输入管理员密码（8位）:");
            scanf("%s", passwd);
            if (strlen(passwd) != PASSWD_SIZE)
            {
                printf("密码长度不符！\n");
            }
            else
            {
                strcpy(p->manager.m_passwd, passwd);
                break;
            }
        }
        printf("请输入管理员身份 (1 超级管理员 or 2 普通管理员):");
        scanf("%d", &choice);
        p->manager.identity = choice == 1 ? ROOT : MANAGER;
        p->manager.m_islocked = FALSE;
        r->next = p;
        p->next = NULL;
        manager_list->m_quantity++;
    }
    printf("注册成功!\n");
    return TRUE;
}

/*3.管理员登录*/
BOOL M_Login(Manager_List *manager_list, char *id, char *passwd)
{
    Node *p = manager_list->head;
    static int times = 0; // 静态内存区，计算密码错误次数
    while (p != NULL)
    {
        if (strcmanager_list(p->manager.m_id, id) == 0)
        {
            if (p->manager.m_islocked == TRUE)
            {
                printf("普通管理员：%s已被锁！！！\n", p->manager.m_id);
                return FALSE; // 锁定状态下直接返回FALSE
            }
            else
            {
                if (strcmanager_list(p->manager.m_passwd, passwd) == 0)
                {
                    switch (p->manager.identity)
                    {
                    case MANAGER:
                    {
                        printf("普通管理员登录成功！\n");
                        times = 0;
                        return MANAGER;
                    }
                    case ROOT:
                    {
                        printf("超级管理员登录成功！\n");
                        times = 0;
                        return ROOT;
                    }
                    } // 便于接下来区分普通管理员和超级管理员的业务
                }
                else
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
                        } // 超过三次管理员账号锁定，只锁定普通管理员
                    }
                    case ROOT:
                    {
                        printf("密码错误！\n");
                    }
                    }
                    return FALSE;
                }
            }
        }
        p = p->next;
    }
    printf("该管理员不存在！\n"); // 遍历链表未查找到符合的id，提醒用户id不存在
    return FALSE;
}

/*4.激活管理员(仅限超级管理员)*/
BOOL M_Active(Manager_List *manager_list, char *id, char *passwd)
{
    Node *p = manager_list->head;
    while (p != NULL)
    {
        if (strcmanager_list(p->manager.m_id, id) == 0) // 通过id遍历链表查询
        {
            if (p->manager.m_islocked != TRUE) // 判断锁定状态
            {
                printf("该管理员未被锁定\n");
                return FALSE;
            }
            else
            {
                if (strcmanager_list(p->manager.m_passwd, passwd) != 0) // 判断密码输入是否正确
                {
                    printf("密码错误\n");
                    return FALSE;
                }
                else
                {
                    p->manager.m_islocked = FALSE;
                    return TRUE;
                }
            }
        }
        p = p->next;
    }
    printf("该管理员不存在！\n"); // 遍历链表未查找到符合的id，提醒用户id不存在
    return FALSE;
}

/*5.注销管理员(仅限超级管理员)*/
BOOL M_Delete(Manager_List *manager_list, char *id, char *passwd)
{
    if (manager_list->head == NULL)
    {
        printf("该管理员不存在！\n");
        return FALSE;
    } // 空链表检查
    Node *p = manager_list->head;
    if (strcmanager_list(p->manager.m_id, id) == 0) // 判断是否需要删除链表头节点
    {
        if (strcmanager_list(p->manager.m_passwd, passwd) != 0) // 判断密码输入是否正确
        {
            printf("密码错误\n");
            return FALSE;
        }
        else
        {
            manager_list->head = p->next; // 将该节点从链表中删除
            free(p);            // 释放内存
            return TRUE;
        }
    }
    while (p->next != NULL)
    {
        if (strcmanager_list(p->next->manager.m_id, id) == 0) // 通过id遍历链表查询
        {
            if (strcmanager_list(p->next->manager.m_passwd, passwd) != 0) // 判断密码输入是否正确
            {
                printf("密码错误\n");
                return FALSE;
            }
            else
            {
                Node *tmp;
                tmp = p->next;
                p->next = tmp->next;
                free(tmp);
                return TRUE;
            }
        }
        p = p->next;
    }
    printf("该管理员不存在！\n"); // 遍历链表未查找到符合的id，提醒用户id不存在
    return FALSE;
}

/*6.管理员菜单*/
void ManagerMenu(Manager_List *manager_list)
{
    int choice;
    char id[ID_SIZE + 1];
    char passwd[PASSWD_SIZE + 1];

    while (1)
    {
        printf("\n========== 管理员操作菜单 ==========\n");
        printf("1. 管理员注册\n");
        printf("2. 管理员登录\n");
        printf("3. 返回主菜单\n");
        printf("===================================\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            M_Register(manager_list);
            break;
        case 2:
        {
            int identity;
            printf("欢迎使用登录系统！\n");
            do
            {
                printf("按下任意键继续，输入q退出\n");
                getchar(); // 清空输入缓冲区
                if (getchar() == 'q')
                {
                    break;
                }
                printf("请输入id：");
                scanf("%s", id);
                printf("请输入密码：");
                scanf("%s", passwd);
            } while ((identity = M_Login(manager_list, id, passwd)) == FALSE);
            if (identity == ROOT)
            {
                SuperAdminMenu(manager_list);
            }
            else if (identity == MANAGER)
            {
                NormalAdminMenu(manager_list);
            }
            else
            {
                ;
            }
            break;
        }
        case 3:
            return; // 返回主菜单
        default:
            printf("无效选择，请重新输入！\n");
        }
    }
}

/*7.超级管理员菜单*/
void SuperAdminMenu(Manager_List *manager_list)
{
    int choice;
    char id[ID_SIZE + 1], passwd[PASSWD_SIZE + 1];
    while (1)
    {
        printf("\n======== 超级管理员菜单 ========\n");
        printf("1. 激活管理员\n");
        printf("2. 注销管理员\n");
        printf("3. 返回上一级\n");
        printf("===============================\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("请输入要激活的管理员账号：");
            scanf("%s", id);
            printf("请输入管理员密码：");
            scanf("%s", passwd);

            if (M_Active(manager_list, id, passwd))
            {
                printf("管理员激活成功！\n");
            }
            else
            {
                printf("管理员激活失败！\n");
            }
            break;
        case 2:
            printf("请输入要注销的管理员账号：");
            scanf("%s", id);
            printf("请输入管理员密码：");
            scanf("%s", passwd);

            if (M_Delete(manager_list, id, passwd))
            {
                printf("管理员注销成功！\n");
            }
            else
            {
                printf("管理员注销失败！\n");
            }
            break;
        case 3:
            return; // 返回上一级菜单
        default:
            printf("无效选择，请重新输入！\n");
        }
    }
}

/*8.普通管理员菜单*/
void NormalAdminMenu(CardArray *pcarr)
{
    assert(pcarr != NULL);

    int choice;      // 菜单选项
    char idcard[19]; // 用于操作的身份证号
    BOOL flag;       // 挂失/激活标志
    while (1)
    {
        printf("\n========== 普通管理员菜单 ==========\n");
        printf("1. 办卡\n");
        printf("2. 销户\n");
        printf("3. 挂失银行卡\n");
        printf("4. 激活银行卡\n");
        printf("5. 返回上一级\n");
        printf("=====================================\n");
        printf("请输入您的选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // 办卡
            if (InsertCard(pcarr))
            {
                printf("办卡成功！\n");
            }
            else
            {
                printf("办卡失败！\n");
            }
            break;

        case 2: // 销户
            printf("请输入要销户的身份证号：");
            scanf("%s", idcard);
            DeleteCard(pcarr, idcard);
            break;

        case 3: // 挂失银行卡
            printf("请输入要挂失的身份证号：");
            scanf("%s", idcard);
            if (SetCLocked(pcarr, idcard, TRUE))
            {
                printf("挂失成功！\n");
            }
            else
            {
                printf("挂失失败，未找到银行卡！\n");
            }
            break;

        case 4: // 激活银行卡
            printf("请输入要激活的身份证号：");
            scanf("%s", idcard);
            if (SetCLocked(pcarr, idcard, FALSE))
            {
                printf("激活成功！\n");
            }
            else
            {
                printf("激活失败，未找到银行卡！\n");
            }
            break;

        case 5: // 返回上一级
            printf("返回上一级菜单...\n");
            return; // 退出循环，返回上一级菜单

        default:
            printf("无效的选项，请重新输入！\n");
            break;
        }
    }
}