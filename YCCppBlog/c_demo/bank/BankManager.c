//
// Created by 杨充 on 2025/7/13.
//
#include "BankManager.h"

// 暂停程序
void pauseProgram() {
    printf("Press Enter to continue...\n");
    getchar();
}

// 清屏
void clearScreen() {
    system("clear"); // 或使用 printf("\033[H\033[J");
}

int main() {
    int choice;
    while (1) {
        showMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("开户\n");
                break;
            case 2:
                printf("存款\n");
                break;
            case 3:
                printf("取款\n");
                break;
            case 4:
                printf("查询余额\n");
                break;
            case 5:
                printf("转账\n");
                break;
            case 6:
                printf("保存数据到文件\n");
                break;
            case 7:
                printf("退出系统，再见！\n");
                exit(0);
                break;
            default:
                printf("无效的选择，请重新输入！\n");
                break;
        }
    }
    return 0;
}


//展示菜单
void showMenu() {
    printf("\n银行账户管理系统\n");
    printf("1. 开户\n");
    printf("2. 存款\n");
    printf("3. 取款\n");
    printf("4. 查询余额\n");
    printf("5. 转账\n");
    printf("6. 保存数据到文件\n");
    printf("7. 退出\n");
    printf("请输入您的选择: ");
}
