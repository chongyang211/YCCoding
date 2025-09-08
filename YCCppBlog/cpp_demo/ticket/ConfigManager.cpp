//
// Created by 杨充 on 2025/9/8.
//
#include <fstream>
#include <iostream>
#include "ConfigManager.h"



//从本地文件读取配置数据，然后存储到map集合
ConfigManager::ConfigManager(const std::string &fileName) {
    //从文件读取信息
    std::ifstream file(fileName);
    if (file.is_open()) {
        //文件可以打开
        std::string key;
        int value;
        while (file >> key >> value) {
            config[key] = value;
            std::cout << "config key : " << key << " , value : " << value << std::endl;
        }
    } else {
        //如果没有文件，则默认配置
        config["total_tickets"] = 100;
        config["num_windows"] = 3;
        config["num_customers"] = 10;
        config["max_tickets_per_customer"] = 5;
        config["simulation_duration"] = 10; // 秒
    }
}

int ConfigManager::get(const std::string &key) const {

}

