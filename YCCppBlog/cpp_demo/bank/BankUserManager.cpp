#include "BankUserManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

#include "Bank.h"
#include "FileManager.h"

//执行指令：g++ Account.cpp Bank.cpp FileManager.cpp BankUserManager.cpp -o BankUserManager

Bank bank;

int main() {
    std::string filename = "accounts.txt";
    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: {
                printf("1. 开户\n");
                openAccount();
                break;
            }
            case 2: {
                printf("2. 存款\n");
                deposit();
                break;
            }
            case 3: {
                printf("3. 取款\n");
                withdraw();
                break;
            }
            case 4: {
                printf("4. 查询余额\n");
                checkBalance();
                break;
            }
            case 5: {
                printf("5. 转账\n");
                transfer();
                break;
            }
            case 6: {
                printf("6. 显示所有账户\n");
                bank.displayAllAccounts();
                break;
            }
            case 7: {
                printf("7. 保存数据到文件\n");
                // FileManager::saveToFile(bank, FILE_NAME);
                FileManager::saveToFile(bank, filename);
                break;
            }
            case 8: {
                printf("8. 从文件加载数据\n");
                FileManager::loadFromFile(bank, filename);
                bank.displayAllAccounts();
                break;
            }
            case 0: {
                printf("0. 退出\n");
                std::cout << "退出系统。\n";
                return 0;
            }
            default: {
                std::cout << "无效选择，请重试！\n";
                break;
            }
        }
    }
    return 0;
}

void displayMenu() {
    std::cout << "\n银行账户管理系统\n";
    std::cout << "1. 开户\n";
    std::cout << "2. 存款\n";
    std::cout << "3. 取款\n";
    std::cout << "4. 查询余额\n";
    std::cout << "5. 转账\n";
    std::cout << "6. 显示所有账户\n";
    std::cout << "7. 保存数据到文件\n";
    std::cout << "8. 从文件加载数据\n";
    std::cout << "0. 退出\n";
    std::cout << "请选择操作: ";
}

// 开户
void openAccount() {
    std::string accNumber, name;
    double initialBalance;
    std::cout << "请输入账户号: ";
    std::getline(std::cin, accNumber);
    std::cout << "请输入姓名: ";
    std::getline(std::cin, name);
    std::cout << "请输入初始余额: ";
    std::cin >> initialBalance;
    bank.createAccount(accNumber, name, initialBalance);
}

// 存款
void deposit() {
    std::string accNumber;
    double amount;
    std::cout << "请输入账户号:";
    //获取输入的字符串
    std::getline(std::cin , accNumber);
    std::cout << "请输入存款金额:";
    std::cin >> amount;
    //然后根据账户号找到对应的账户，判断存款金额大于0后即存款
    bank.deposit(accNumber, amount);
}

// 取款
void withdraw() {
    std::string accNumber;
    double amount;
    std::cout << "请输入账户号: ";
    std::getline(std::cin, accNumber);
    std::cout << "请输入取款金额: ";
    std::cin >> amount;
    bank.withdraw(accNumber, amount);
}

// 查询余额
void checkBalance() {
    std::string accNumber;
    std::cout << "请输入账户号: ";
    std::getline(std::cin, accNumber);
    bank.queryBalance(accNumber);
}

// 用户转账
void transfer() {
    // 定义转入和转出账号
    std::string fromAccNumber, toAccNumber;
    //转出金额
    double amount;
    std::cout << "请输入转出账户号: ";
    std::getline(std::cin, fromAccNumber);
    std::cout << "请输入转入账户号: ";
    std::getline(std::cin, toAccNumber);
    std::cout << "请输入转账金额: ";
    std::cin >> amount;
    bank.transfer(fromAccNumber, toAccNumber, amount);
}