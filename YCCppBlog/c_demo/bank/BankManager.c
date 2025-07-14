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

BankAccount accounts[MAX_ACCOUNTS];
int account_count = 0;

int main() {
    int choice;
    while (1) {
        showMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("开户\n");
                openAccount();
                break;
            case 2:
                printf("存款\n");
                deposit();
                break;
            case 3:
                printf("取款\n");
                withdraw();
                break;
            case 4:
                printf("查询余额\n");
                checkBalance();
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

// 开户
void openAccount() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("账户数量已达到上限，无法继续开户！\n");
        return;
    }
    BankAccount newAccount;
    newAccount.accountNumber = account_count + 1;
    printf("请输入账户姓名: ");
    scanf("%s", newAccount.name);
    pauseProgram();
    //fgets(newAccount.name, NAME_LENGTH, stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = '\0'; // 去掉换行符
    printf("请输入初始余额: ");
    //scanf("%lf", &newAccount.balance);
    while (scanf("%lf", &newAccount.balance) != 1) {
        printf("输入无效，请输入一个数字: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }
    accounts[account_count] = newAccount;
    account_count++;
    printf("账户开户成功！账户号: %d\n", newAccount.accountNumber);
    pauseProgram();
    //clearScreen();
}

// 存款
void deposit() {
    int accountNumber;
    double amount;
    printf("请输入账户号：");
    scanf("%d",&accountNumber);
    //输入卡号后，开始查找用户
    for (int i=0 ; i<account_count; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("请输入存款金额：");
            scanf("%lf",&amount);
            if (amount <= 0) {
                printf("存款金额必须大于0！\n");
                return;
            }
            accounts[i].balance += amount;
            printf("存款成功！当前余额: %.2lf\n", accounts[i].balance);
            return;
        }
    }
    printf("未找到账户号为%d的账户！\n", accountNumber);
}

// 取款
void withdraw() {
    int accountNumber;
    double amount;
    printf("请输入账户号：");
    scanf("%d",&accountNumber);
    for (int i = 0 ; i < account_count ; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("请输入要取出的金额：");
            scanf("%lf",&amount);
            if (amount <= 0) {
                printf("取款金额必须大于0！\n");
                return;
            }
            if (amount > accounts[i].balance) {
                printf("余额不足，无法取款！\n");
                return;
            }
            accounts[i].balance -= amount;
            printf("取款成功！当前余额: %.2lf\n", accounts[i].balance);
            return;
        }
    }
    printf("未找到账户号为%d的账户！\n", accountNumber);
}

// 查询余额
void checkBalance() {
    int accountNumber;
    printf("请输入账户号: ");
    scanf("%d", &accountNumber);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("账户号: %d, 姓名: %s, 当前余额: %.2lf\n",
                   accounts[i].accountNumber,
                   accounts[i].name,
                   accounts[i].balance);
            return;
        }
    }
    printf("未找到账户号为%d的账户！\n", accountNumber);
}

// 转账
void transfer() {

}


