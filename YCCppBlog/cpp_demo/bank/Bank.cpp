//
// Created by 杨充 on 2025/7/16.
//

#include "Bank.h"

// 开户
void Bank::createAccount(const std::string &accNumber, const std::string &accName, double initialBalance) {
    Account newAccount(accNumber, accName, initialBalance);
    accounts.push_back(newAccount);
    std::cout << "开户成功！" << std::endl;
    //todo 打印开户成功后，数量
    //int count = accounts.size;
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
    //定义一个指针用户
    Account* account = nullptr;
    //遍历查找
    for (std::vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->getAccountNumber() == accNumber) {
            account = &(*it);
            break;
        }
    }
    if (account != nullptr) {
        //用户存在
        account->deposit(amount);
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

// 取款
void Bank::withdraw(const std::string &accNumber, double amount) {
    Account* account = nullptr;
    //使用传统的 for 循环。通过索引访问 std::vector 的元素：
    for(size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i].getAccountNumber() == accNumber) {
            account = &accounts[i];
            break;
        }
    }
    if (account != nullptr) {
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

// 转账
void Bank::transfer(const std::string &fromAccNumber, const std::string &toAccNumber, double amount) {
    //先找到转入账户
    Account* fromAccount = findAccount(fromAccNumber);
    //然后找到转出账户
    Account* toAccount = findAccount(toAccNumber);
    if (fromAccount && toAccount) {
        //两个账户不为空时，才可以转账
        fromAccount->transfer(*toAccount, amount);
    } else {
        std::cout << "账户不存在！" << std::endl;
    }
}

// 显示所有账户
void Bank::displayAllAccounts() const {
    for (std::vector<Account>::const_iterator it = accounts.begin(); it != accounts.end(); ++it) {
        it->display();
    }
}

// 获取所有账户
const std::vector<Account> &Bank::getAccounts() const {
    return accounts;
}

void Bank::setAccounts(const std::vector<Account> &accList) {
    this->accounts = accList;
}




