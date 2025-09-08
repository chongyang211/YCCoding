//
// Created by 杨充 on 2025/9/8.
//

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <string>
#include <unordered_map>


//主要是从配置文件中读取配置数据
class ConfigManager {
private:
    //用map集合存储配置数据
    std::unordered_map<std::string,int> config;
public:
    //从本地文件读取配置数据，然后存储到map集合
    ConfigManager(const std::string &fileName);
    //获取某项属性。map集合存储key-value
    int get(const std::string &key) const;
};



#endif //CONFIGMANAGER_H
