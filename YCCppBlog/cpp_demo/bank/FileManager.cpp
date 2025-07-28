//
// Created by 杨充 on 2025/7/16.
//

#include "FileManager.h"


void FileManager::saveToFile(const Bank &bank, const std::string &filename) {
    std::ofstream ofs;
    ofs.open(filename.c_str(),std::ios::out);
    if (ofs.is_open()) {
        //获取所有账户集合
        const std::vector<Account> &accounts = bank.getAccounts();
        //遍历存储数据
        for (std::vector<Account>::const_iterator it = accounts.begin(); it != accounts.end(); ++it) {
            ofs << it->getAccountNumber() << ","
                    << it->getName() << ","
                    << it->getBalance() << "\n";
        }
        ofs.close();
        std::cout << "数据保存成功" << std::endl;
    } else {
        std::cout << "无法打开文件" << std::endl;
    }
}

void FileManager::loadFromFile(Bank &bank, const std::string &filename) {
    std::ifstream ifs;
    ifs.open(filename.c_str(), std::ios::in);
    if (ifs.is_open()) {
        //创建一个集合
        std::vector<Account> accounts;
        //一次读取一行
        std::string line;
        while (std::getline(ifs,line)) {
            // 将每行转换为字符串流
            std::istringstream iss(line);
            std::string accNumber, name, balance;
            // 使用逗号作为分隔符，提取字段
            if (std::getline(iss, accNumber, ',') &&
                std::getline(iss, name, ',') &&
                std::getline(iss, balance, ',')) {
                // 输出解析后的字段
                std::cout << "账号: " << accNumber << ", 用户名: " << name << ", 余额: " << balance << std::endl;
                try {
                    double value = std::stod(balance);
                    std::cout << "余额，转换后的值: " << value << std::endl;
                    accounts.push_back(Account(accNumber, name, value));
                } catch (const std::invalid_argument& e) {
                    std::cerr << "余额，无效的参数: " << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "余额，超出范围: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "解析失败: " << line << std::endl;
            }
        }
        ifs.close();
        std::cout << "数据加载成功！" << std::endl;
    } else {
        std::cout << "无法打开文件" << std::endl;
    }
}

