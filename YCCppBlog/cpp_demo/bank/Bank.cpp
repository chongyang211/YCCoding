//
// Created by 杨充 on 2025/7/16.
//

#include "Bank.h"

// 开户
void Bank::createAccount(const std::string &accNumber, const std::string &accName, double initialBalance) {
    Account newAccount(accNumber, accName, initialBalance);
    accounts.push_back(newAccount);
    std::cout << "开户成功！" << std::endl;
}

// 根据账户号查找账户
Account *Bank::findAccount(const std::string &accNumber) {
    //遍历
    for (std::vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
        //根据账号查找
        if (it->getAccountNumber() == accNumber) {
            return &(*it);
        }
    }
    return nullptr;
}

// 存款
void Bank::deposit(const std::string &accNumber, double amount) {
    Account* account = findAccount(accNumber);
    if (account) {
        account->withdraw(amount);
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

// 查询余额
void Bank::queryBalance(const std::string &accNumber) {
    Account* account = findAccount(accNumber);
    if (account) {
        std::cout << "账户余额: " << account->getBalance() << std::endl;
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

