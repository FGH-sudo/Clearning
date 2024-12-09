#ifndef MANAGER_H
#define MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define ERRTIMES  3     //登录密码错误次数的上限
#define NAMESIZE  10    //储户名数组大小
#define DATESIZE  30    //操作时间数组大小
#define BANKNAME  10    //银行名数组大小
#define ADDRESSSIZE 20   //银行地址数组大小
#define CARDSIZE   20   //银行卡初始容量

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
void Init(Manager_List **mp);

/*2.管理员注册业务*/
bool M_Register(Manager_List *mp);

/*3.管理员登录业务*/
bool M_Login(Manager_List *mp,char *id,char *passwd);




#endif