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
};



#endif //BANK_H
