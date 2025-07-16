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
