#ifndef ATM_H
#define ATM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define ERRTIMES 3     // 登录密码错误次数的上限
#define NAMESIZE 10    // 储户名数组大小
#define DATESIZE 30    // 操作时间数组大小
#define BANKSIZE 10    // 银行名数组大小
#define ADDRESSSIZE 20 // 银行地址数组大小
#define CARDSIZE 20    // 银行卡初始容量
#define ID_SIZE 20     // id长度大小
#define PASSWD_SIZE 8  // 密码长度大小

#define LIMITMONEYDAY 1000 // 日取款上限
#define RECORDNUM 50       // 流水个数

typedef int BOOL;

/*管理员状态*/
typedef enum
{
    ROOT = 2,
    MANAGER = 3
} Identity; // （ROOT-超级  MANAGER-普通）

/*管理员数据结构体*/
typedef struct
{
    char m_id[ID_SIZE + 1];         // 管理员账号
    char m_passwd[PASSWD_SIZE + 1]; // 管理员密码
    BOOL m_islocked;                // 身份状态是否被锁
    Identity identity;              // 管理员身份
} Manager;

typedef struct
{
    char name[NAMESIZE]; // 姓名
    long long phone;     // 电话号码
    char idcard[19];     // 身份证号
} USER;                  // 用户信息结构体

typedef struct
{
    char time[DATESIZE]; // 操作时间
    char type[10];       // 具体业务{存款  取款   转入   转出 }
    float r_money;       // 涉及金额(存: 100  取 -100)
} Record;                // 用户流水结构体

typedef struct
{
    Record *precords; // 记录数组 malloc
    int r_size;       // 记录数组 有效记录个数
    int r_capacity;   // 记录数组中 容量大小
} WaterRecord;        // 流水记录数组

typedef struct
{
    char bankname[BANKSIZE];       // 银行名称："建行""招商""工商"
    char bankaddress[ADDRESSSIZE]; // 银行地址： "北大街""东大街""西大街"
} BankAddress;                     // 银行信息结构体

typedef struct
{
    int c_id;                   // 银行卡账号
    int c_passwd;               // 银行卡 密码
    float money;                // 余额
    float withdraw_limit;       // 日取款上限
    BOOL c_islocked;            // 卡是否被锁  ---> 密码错上限
    char create_date[DATESIZE]; // 开卡日期："2024-1-1 星期几 20:20:20"

    USER user;               // 储户信息：姓名 电话 身份证
    WaterRecord records;     // 流水明细
    BankAddress bankaddress; // 办卡地址 ：银行名+办卡地址
} Bankcard;

// 动态数组存储银行卡具体信息
typedef struct
{
    Bankcard *cards;
    int c_size;     // 卡的数量
    int c_capacity; // 卡容量
} CardArray;

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

BankAddress bankmessage[] = {{"招行", "北大街"}, {"建行", "西大街"}, {"工商", "南大街"}};

const char *week[] = {"星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"};

/***********管理员函数声明***********/
/*初始化*/
void M_Init(Manager_List **mp);

/*管理员注册*/
BOOL M_Register(Manager_List *mp);

/*管理员登录*/
BOOL M_Login(Manager_List *mp, char *id, char *passwd);

/*激活管理员(仅限超级管理员)*/
BOOL M_Active(Manager_List *mp, char *id, char *passwd);

/*删除管理员(仅限超级管理员)*/
BOOL M_Delete(Manager_List *mp, char *id, char *passwd);

/*办卡*/
BOOL InsertCard(CardArray *pcarr);

/*销户*/
void DeleteCard(CardArray *pcarr, const char *idcard);

/*挂失、激活*/
BOOL SetCLocked(CardArray *pcarr, const char *idcard, BOOL flag);

/**********储户函数声明*************/
//初始化
void CInit(CardArray* pcarr);

// 登录
Bankcard *CLogin(CardArray *pcarr, int id, int passwd);

// 存款
BOOL SaveMoney(Bankcard *pcard, float money);

// 取款
BOOL WithDraw(Bankcard *pcard, float money);

// 转账
BOOL Transfer(CardArray *pcarr, Bankcard *pcard, int userid, float money);

// 查余额
void GetMoney(Bankcard *pcard);

// 查开卡日期
void GetCreateCardDate(Bankcard *pcard);

// 查流水明细
void GetWaterRecords(Bankcard *pcard);

/********菜单设计声明*********/
/*管理员菜单*/
void ManagerMenu(Manager_List *mp);

/*超级管理员菜单*/
void SuperAdminMenu(Manager_List *mp);

/*普通管理员菜单*/
void NormalAdminMenu(CardArray *pcarr);

/*储户菜单*/
void CustomerMenu(CardArray *pcarr);

/***********辅助函数声明***********/
/*获取系统时间*/
void GetSystemTime(char *buff);

/*插入流水记录*/
void InsertRecord(Bankcard *pcard, const char *time, const char *type, float money);

/*按储户id寻找，供储户使用*/
Bankcard *CSearchById(CardArray *pcarr, int id);

/*按身份证号（idcard）寻找，供普通管理员使用*/
Bankcard *CSearchByIdCard(CardArray *pcarr, const char *idcard);

#endif