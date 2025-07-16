//
// Created by 杨充 on 2025/7/16.
//
#include <string>
#include <iostream>
#ifndef ACCOUNT_H
#define ACCOUNT_H

//用户类
class Account {
private:
    std::string accountNumber; // 账户号
    std::string name;          // 姓名
    double balance;            // 余额
public:
    // 构造函数
    Account(const std::string& accNumber, const std::string& accName, double initialBalance);

    // 获取账户号
    std::string getAccountNumber() const ;

    // 获取姓名
    std::string getName() const;

    // 获取余额
    double getBalance() const;

    // 存款
    void deposit(double amount);

    // 取款
    void withdraw(double amount);

    // 转账
    bool transfer(Account& targetAccount, double amount);

    // 显示账户信息
    void display() const;
};



#endif //ACCOUNT_H
