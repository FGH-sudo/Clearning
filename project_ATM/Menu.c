#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "ATM.h"


void MainMenu(Manager_List *manager_list, CardArray *pcarr)
{
    int choice;

    while (1)
    {
        printf("\n======== 银行系统主菜单 ========\n");
        printf("1. 管理员操作\n");
        printf("2. 储户操作\n");
        printf("0. 退出系统\n");
        printf("=================================\n");
        printf("请输入您的选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // 管理员操作
            ManagerMenu(manager_list);
            break;

        case 2: // 储户操作
            CustomerMenu(pcarr);
            break;

        case 0: // 退出系统
            printf("感谢使用银行系统，再见！\n");
            return;

        default:
            printf("无效输入，请重新选择！\n");
            break;
        }
    }
}


