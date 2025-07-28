//
// Created by 杨充 on 2025/7/28.
//

#ifndef STRATEGY_UTILS_H
#define STRATEGY_UTILS_H
#include <string>


class StrategyUtils {
public:
    static StrategyUtils* GetInstance() {
        static StrategyUtils instance;
        return &instance;
    }
    /**
     * @brief 写入文件
     * @param filePath 文件路径
     * @param data 数据
     * @return 是否成功
     */
    static bool writeRelayFile(const std::string &filePath, const char *data);
};


#endif //STRATEGY_UTILS_H
