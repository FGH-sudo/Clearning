#include "ATM.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*获取系统时间*/
void GetSystemTime(char *buff)
{
    assert(buff != NULL);
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    sprintf(buff, "%d-%d-%d %s", ptm->tm_year + 1900,
            ptm->tm_mon + 1, ptm->tm_mday, week[ptm->tm_wday]);
}

/*插入流水记录*/
void InsertRecord(Bankcard *pcard, const char *time, const char *type, float money)
{

    if (pcard->records.r_capacity == pcard->records.r_size)
    {
        Record *tmp = (Record *)realloc(pcard->records.precords, 2 * pcard->records.r_capacity * sizeof(Record));
        assert(tmp != NULL);
        pcard->records.precords = tmp;
        pcard->records.r_capacity *= 2;
    }
    Record *p = &pcard->records.precords[pcard->records.r_size];
    p->r_money = money;
    strcpy(p->time, time);
    strcpy(p->type, type);
    pcard->records.r_size++;
}

/*按储户id寻找，供储户使用*/
Bankcard *CSearchById(CardArray *pcarr, int id)
{
    assert(pcarr != NULL);
    for (int i = 0; i < pcarr->c_size; i++)
    {
        if (pcarr->cards[i].c_id == id)
        {
            return pcarr->cards + i;
        }
    }
    return NULL;
}

/*按身份证号（idcard）寻找，供普通管理员使用*/
Bankcard *CSearchByIdCard(CardArray *pcarr, const char *idcard)
{
    assert(pcarr != NULL);
    for (int i = 0; i < pcarr->c_size; i++)
    {
        if (strcmp(pcarr->cards[i].user.idcard, idcard) == 0)
        {
            return pcarr->cards + i;
        }
    }
    return NULL;
}

/*1.初始化*/
void CInit(CardArray* pcarr)
{
    pcarr->cards = (Bankcard *)malloc(sizeof(Bankcard) * CARDSIZE);
    pcarr->c_capacity = CARDSIZE;
    pcarr->c_size = 0;
    printf("初始化成功\n");
}

/*2.办卡*/
BOOL InsertCard(CardArray *pcarr)
{
    assert(pcarr != NULL);
    if (pcarr->c_size == pcarr->c_capacity)
    {
        Bankcard *tmp = (Bankcard *)realloc(pcarr->cards, 2 * pcarr->c_capacity * sizeof(Bankcard));
        assert(tmp != NULL);
        pcarr->cards = tmp;
        pcarr->c_capacity *= 2;
    }

    printf("请输入帐号：");
    scanf("%d", &pcarr->cards[pcarr->c_size].c_id);
    printf("请输入密码：");
    scanf("%d", &pcarr->cards[pcarr->c_size].c_passwd);
    printf("请输入预存金额：");
    scanf("%d", &pcarr->cards[pcarr->c_size].money);
    printf("请输入储户姓名：");
    scanf("%s", &pcarr->cards[pcarr->c_size].user.name);
    printf("请输入储户电话：");
    scanf("%d", &pcarr->cards[pcarr->c_size].user.phone);
    printf("请输入储户身份证号：");
    scanf("%d", &pcarr->cards[pcarr->c_size].user.idcard);
    printf("请输入银行名（建行or招行or工商：");
    scanf("%s", &pcarr->cards[pcarr->c_size].bankaddress.bankname);

    const char *pname = pcarr->cards[pcarr->c_size].bankaddress.bankname;

    for (int i = 0; i < 3; i++)
    {
        if (strcmp(bankmessage[i].bankname, pname))
        {
            strcpy(pcarr->cards[pcarr->c_size].bankaddress.bankaddress, bankmessage[i].bankaddress);
        }

        pcarr->cards[pcarr->c_size].withdraw_limit = LIMITMONEYDAY;
        pcarr->cards[pcarr->c_size].c_islocked = FALSE;
        GetSystemTime(pcarr->cards[pcarr->c_size].create_date);

        pcarr->cards[pcarr->c_size].records.precords = (Record *)malloc(RECORDNUM * sizeof(Record));
        pcarr->cards[pcarr->c_size].records.r_size = 0;
        pcarr->cards[pcarr->c_size].records.r_capacity = RECORDNUM;

        InsertRecord(&pcarr->cards[pcarr->c_size],
                     pcarr->cards[pcarr->c_size].create_date, "注册卡业务",
                     pcarr->cards[pcarr->c_size].money);

        pcarr->c_size++;
        return TRUE;
    }
}

/*3.销户*/
void DeleteCard(CardArray *pcarr, const char *idcard)
{
    assert(pcarr != NULL);
    Bankcard *pc = CSearchByIdCard(pcarr, idcard);
    if (pc == NULL)
    {
        printf("注销银行卡失败\n");
        return;
    }

    // 将卡的记录数组  释放 //
    free(pc->records.precords);

    // 找到该元素   进行覆盖//
    int movenum = pcarr->c_size - 1 - (pcarr->cards + 1 - pcarr->cards);
    memmove(pc, pc + 1, movenum * sizeof(Bankcard));
    pcarr->c_size--;
    printf("删除成功\n");
}

/*4.挂失（flag==TRUE）   激活（flag==FALSE）*/
// 挂失就是给储户的卡冻结，防止别人使用；激活就是给储户的卡解冻
BOOL SetCLocked(CardArray *pcarr, const char *idcard, BOOL flag)
{
    assert(pcarr != NULL && idcard != NULL);

    Bankcard *pc = CSearchByIdCard(pcarr, idcard);
    if (pc == NULL)
        return FALSE;
    pc->c_islocked = flag;
    return TRUE;
}

/*5.储户登录*/
Bankcard *CLogin(CardArray *pcarr, int id, int passwd)
{
    assert(pcarr != NULL);
    static int c_times = 0;
    Bankcard *pc = CSearchById(pcarr, id);
    if (pc == NULL)
    {
        printf("不存在该用户\n");
    }
    else if (pc->c_passwd == passwd && !pc->c_islocked)
    {
        printf("登录成功\n");
        c_times = 0;
        return pc;
    }
    else if (pc->c_islocked)
    {
        printf("已被锁，请激活\n");
    }
    else if (pc->c_passwd != passwd)
    {
        printf("密码错误%d次,登录失败\n", ++c_times);
        if (c_times == ERRTIMES)
        {
            pc->c_islocked = TRUE;
            c_times = 0;
            printf("账号:%d 的 卡已被冻结\n", pc->c_id);
        }
    }
    return NULL;
}

/*6.存款*/
BOOL SaveMoney(Bankcard *pcard, float money)
{
    assert(pcard != NULL);
    pcard->money += money;
    printf("存款成功\n");
    char buff[30];
    GetSystemTime(buff);
    InsertRecord(pcard, buff, "存款", money);
    return TRUE;
}

/*7.取款*/
BOOL WithDraw(Bankcard *pcard, float money)
{
    assert(pcard != NULL);
    if (pcard->money >= money && pcard->withdraw_limit >= money)
    {
        pcard->money -= money;
        printf("取款成功\n");
        char time[20];
        GetSystemTime(time);
        InsertRecord(pcard, time, "取款", -1 * money);
        return TRUE;
    }
    if (pcard->money < money)
    {
        printf("余额不足，取款失败\n");
    }

    else
    {
        printf("取款超出日取款上限，取款失败\n");
        return FALSE;
    }
}

/*8.转账*/
BOOL Transfer(CardArray *pcarr, Bankcard *pcard, int userid, float money)
{
    assert(pcarr != NULL && pcard != 0); // assert 检验卡号和银行卡数组是否存在，若存在则继续运行

    Bankcard *pusercard = CSearchById(pcarr, userid); //
    if (pusercard == NULL)
    {
        printf("对方账户为空，转账失败\n");
        return FALSE;
    }
    // 自己账户money  是否足够转出
    if (!pusercard->c_islocked && pcard->money >= money)
    {

        pcard->money -= money;
        pusercard->money += money;

        // 流水记录  自己转出    对方转入
        char time[20];
        GetSystemTime(time);
        InsertRecord(pcard, time, "转出", -1 * money);
        InsertRecord(pusercard, time, "转入", money);
        printf("转账成功\n");
        return TRUE;
    }

    if (pusercard->c_islocked)
    {
        printf("对方银行卡状态异常，转账失败\n");
    }
    else
    {
        printf("余额不足，转账失败\n");
    }
    return FALSE;
}

/*9.查余额*/
void GetMoney(Bankcard *pcard)
{
    assert(pcard != NULL);
    printf("账号为:%d 银行卡  余额为:%f\n", pcard->c_id, pcard->money);
}

/*10.查开卡日期  打印日期*/
void GetCreateCardDate(Bankcard *pcard)
{
    assert(pcard != NULL);
    printf("账号为:%d 银行卡 开户日期为:%s\n", pcard->c_id, pcard->create_date);
}

/*11.查流水*/
void GetWaterRecords(Bankcard *pcard)
{
    assert(pcard != NULL);
    Record *pr = pcard->records.precords; // pr 指向记录数组
    printf("序号\t时间\t类型\t金额\n");
    for (int i = 0; i < pcard->records.r_size; i++)
    {
        printf("%d %s %s\t %f\n", i + 1, pr[i].time, pr[i].type, pr[i].r_money);
        // Sleep(1000); //1000ms   windows.h
    }
}

/*12.储户菜单*/
void CustomerMenu(CardArray *pcarr)
{
    assert(pcarr != NULL);

    int id, passwd;
    printf("请输入账号: ");
    scanf("%d", &id);
    printf("请输入密码: ");
    scanf("%d", &passwd);

    // 登录验证
    Bankcard *pcard = CLogin(pcarr, id, passwd);
    if (pcard == NULL)
    {
        printf("登录失败，返回主菜单\n");
        return;
    }

    while (1)
    {
        printf("\n======== 储户功能菜单 ========\n");
        printf("1. 存款\n");
        printf("2. 取款\n");
        printf("3. 转账\n");
        printf("4. 查询余额\n");
        printf("5. 查询开户日期\n");
        printf("6. 查询流水记录\n");
        printf("0. 返回上一级\n");
        printf("请输入您的选择: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // 存款
        {
            float amount;
            printf("请输入存款金额: ");
            scanf("%f", &amount);
            if (amount <= 0)
            {
                printf("金额无效，存款失败\n");
            }
            else
            {
                SaveMoney(pcard, amount);
            }
            break;
        }
        case 2: // 取款
        {
            float amount;
            printf("请输入取款金额: ");
            scanf("%f", &amount);
            if (amount <= 0)
            {
                printf("金额无效，取款失败\n");
            }
            else
            {
                WithDraw(pcard, amount);
            }
            break;
        }
        case 3: // 转账
        {
            int targetId;
            float amount;
            printf("请输入目标账户ID: ");
            scanf("%d", &targetId);
            printf("请输入转账金额: ");
            scanf("%f", &amount);
            if (amount <= 0)
            {
                printf("金额无效，转账失败\n");
            }
            else
            {
                Transfer(pcarr, pcard, targetId, amount);
            }
            break;
        }
        case 4: // 查询余额
            GetMoney(pcard);
            break;

        case 5: // 查询开户日期
            GetCreateCardDate(pcard);
            break;

        case 6: // 查询流水记录
            GetWaterRecords(pcard);
            break;

        case 0: // 返回上一级
            printf("返回上一级菜单\n");
            return;

        default:
            printf("无效输入，请重新选择\n");
            break;
        }
    }
}
