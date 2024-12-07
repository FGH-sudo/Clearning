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
        printf("请选择需要办理的业务（输入0退出系统）\n");
        scanf("%d", &choice1);
        switch (choice1)
        {
        case 1:
        {
            m_register(manager_list);
            break;
        }
        case 0:{
            printf("您已退出该系统！\n");
            return 0;
        }
        }
    }
}