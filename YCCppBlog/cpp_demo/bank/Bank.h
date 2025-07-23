//
// Created by 杨充 on 2025/7/16.
//

#include <string>
#include <iostream>
#include "Account.h"
#ifndef BANK_H
#define BANK_H



class Bank {
private:
    std::vector<Account> accounts; // 所有账户
public:
    // 开户
    void createAccount(const std::string& accNumber, const std::string& accName, double initialBalance);
    // 根据账户号查找账户
    Account* findAccount(const std::string& accNumber);
    // 存款
    void deposit(const std::string& accNumber, double amount);
    // 取款
    void withdraw(const std::string& accNumber, double amount);
    // 查询余额
    void queryBalance(const std::string& accNumber) ;
    // 转账
    void transfer(const std::string& fromAccNumber, const std::string& toAccNumber, double amount);
    // 显示所有账户
    void displayAllAccounts() const ;
    // 获取所有账户
    const std::vector<Account>& getAccounts() const ;
    // 设置所有账户
    void setAccounts(const std::vector<Account>& accList) ;
};



#endif //BANK_H
