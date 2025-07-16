#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>


class Account {
private:
    std::string accountNumber; // 账户号
    std::string name;          // 姓名
    double balance;            // 余额

public:
    // 构造函数
    Account(const std::string& accNumber, const std::string& accName, double initialBalance)
        : accountNumber(accNumber), name(accName), balance(initialBalance) {}

    // 获取账户号
    std::string getAccountNumber() const {
        return accountNumber;
    }

    // 获取姓名
    std::string getName() const {
        return name;
    }

    // 获取余额
    double getBalance() const {
        return balance;
    }

    // 存款
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "存款成功！当前余额: " << balance << std::endl;
        } else {
            std::cout << "存款金额必须大于 0！" << std::endl;
        }
    }

    // 取款
    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "取款成功！当前余额: " << balance << std::endl;
        } else {
            std::cout << "取款金额无效或余额不足！" << std::endl;
        }
    }

    // 转账
    bool transfer(Account& targetAccount, double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            targetAccount.deposit(amount);
            std::cout << "转账成功！当前余额: " << balance << std::endl;
            return true;
        } else {
            std::cout << "转账金额无效或余额不足！" << std::endl;
            return false;
        }
    }

    // 显示账户信息
    void display() const {
        std::cout << "账户号: " << accountNumber << ", 姓名: " << name << ", 余额: " << balance << std::endl;
    }
};


#include <vector>
#include <algorithm>

class Bank {
private:
    std::vector<Account> accounts; // 所有账户

public:
    // 开户
    void createAccount(const std::string& accNumber, const std::string& accName, double initialBalance) {
        Account newAccount(accNumber, accName, initialBalance);
        accounts.push_back(newAccount);
        std::cout << "开户成功！" << std::endl;
    }

    // 根据账户号查找账户
    Account* findAccount(const std::string& accNumber) {
        for (std::vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
            if (it->getAccountNumber() == accNumber) {
                return &(*it);
            }
        }
        return nullptr;
    }

    // 存款
    void deposit(const std::string& accNumber, double amount) {
        Account* account = findAccount(accNumber);
        if (account) {
            account->deposit(amount);
        } else {
            std::cout << "账户不存在！" << std::endl;
        }
    }

    // 取款
    void withdraw(const std::string& accNumber, double amount) {
        Account* account = findAccount(accNumber);
        if (account) {
            account->withdraw(amount);
        } else {
            std::cout << "账户不存在！" << std::endl;
        }
    }

    // 查询余额
    void queryBalance(const std::string& accNumber) {
        Account* account = findAccount(accNumber);
        if (account) {
            std::cout << "账户余额: " << account->getBalance() << std::endl;
        } else {
            std::cout << "账户不存在！" << std::endl;
        }
    }

    // 转账
    void transfer(const std::string& fromAccNumber, const std::string& toAccNumber, double amount) {
        Account* fromAccount = findAccount(fromAccNumber);
        Account* toAccount = findAccount(toAccNumber);
        if (fromAccount && toAccount) {
            fromAccount->transfer(*toAccount, amount);
        } else {
            std::cout << "账户不存在！" << std::endl;
        }
    }

    // 显示所有账户
    void displayAllAccounts() const {
        for (std::vector<Account>::const_iterator it = accounts.begin(); it != accounts.end(); ++it) {
            it->display();
        }
    }

    // 获取所有账户
    const std::vector<Account>& getAccounts() const {
        return accounts;
    }

    // 设置所有账户
    void setAccounts(const std::vector<Account>& accList) {
        accounts = accList;
    }
};
#include <fstream>
#include <sstream>

class FileManager {
public:
    // 保存账户数据到文件
    static void saveToFile(const Bank& bank, const std::string& filename) {
        std::ofstream outFile(filename.c_str());
        if (outFile.is_open()) {
            const std::vector<Account>& accounts = bank.getAccounts();
            for (std::vector<Account>::const_iterator it = accounts.begin(); it != accounts.end(); ++it) {
                outFile << it->getAccountNumber() << ","
                        << it->getName() << ","
                        << it->getBalance() << "\n";
            }
            outFile.close();
            std::cout << "数据保存成功！" << std::endl;
        } else {
            std::cout << "无法打开文件！" << std::endl;
        }
    }

    // 从文件加载账户数据
    static void loadFromFile(Bank& bank, const std::string& filename) {
        std::ifstream inFile(filename.c_str());
        if (inFile.is_open()) {
            std::vector<Account> accounts;
            std::string line;
            while (std::getline(inFile, line)) {
                std::stringstream ss(line);
                std::string accNumber, name;
                double balance;
                char delimiter;
                ss >> accNumber >> delimiter >> name >> delimiter >> balance;
                accounts.push_back(Account(accNumber, name, balance));
            }
            inFile.close();
            bank.setAccounts(accounts);
            std::cout << "数据加载成功！" << std::endl;
        } else {
            std::cout << "无法打开文件！" << std::endl;
        }
    }
};


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

int main() {
    Bank bank;
    std::string filename = "accounts.txt";

    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::string accNumber, name;
                double initialBalance;
                std::cout << "请输入账户号: ";
                std::getline(std::cin, accNumber);
                std::cout << "请输入姓名: ";
                std::getline(std::cin, name);
                std::cout << "请输入初始余额: ";
                std::cin >> initialBalance;
                bank.createAccount(accNumber, name, initialBalance);
                break;
            }
            case 2: {
                std::string accNumber;
                double amount;
                std::cout << "请输入账户号: ";
                std::getline(std::cin, accNumber);
                std::cout << "请输入存款金额: ";
                std::cin >> amount;
                bank.deposit(accNumber, amount);
                break;
            }
            case 3: {
                std::string accNumber;
                double amount;
                std::cout << "请输入账户号: ";
                std::getline(std::cin, accNumber);
                std::cout << "请输入取款金额: ";
                std::cin >> amount;
                bank.withdraw(accNumber, amount);
                break;
            }
            case 4: {
                std::string accNumber;
                std::cout << "请输入账户号: ";
                std::getline(std::cin, accNumber);
                bank.queryBalance(accNumber);
                break;
            }
            case 5: {
                std::string fromAccNumber, toAccNumber;
                double amount;
                std::cout << "请输入转出账户号: ";
                std::getline(std::cin, fromAccNumber);
                std::cout << "请输入转入账户号: ";
                std::getline(std::cin, toAccNumber);
                std::cout << "请输入转账金额: ";
                std::cin >> amount;
                bank.transfer(fromAccNumber, toAccNumber, amount);
                break;
            }
            case 6: {
                bank.displayAllAccounts();
                break;
            }
            case 7: {
                FileManager::saveToFile(bank, filename);
                break;
            }
            case 8: {
                FileManager::loadFromFile(bank, filename);
                break;
            }
            case 0: {
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