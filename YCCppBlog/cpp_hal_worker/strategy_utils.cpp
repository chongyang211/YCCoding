//
// Created by 杨充 on 2025/7/28.
//

#include "strategy_utils.h"

bool StrategyUtils::writeRelayFile(const std::string &filePath, const char *data) {
    FILE* file = fopen(filePath.c_str(), "w");
    if (file == nullptr) {
        return false;
    }
    int ret = fprintf(file, "%s", data);
    fclose(file);
    if (ret < 0) {
        return false;
    }
    return true;
}
