//
// Created by 杨充 on 2025/7/16.
//

#include "FileManager.h"


void FileManager::saveToFile(const Bank &bank, const std::string &filename) {
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

void FileManager::loadFromFile(Bank &bank, const std::string &filename) {
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

