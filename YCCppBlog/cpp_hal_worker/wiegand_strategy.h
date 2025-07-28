//
// Created by 杨充 on 2025/7/28.
//

#ifndef WIEGAND_STRATEGY_H
#define WIEGAND_STRATEGY_H
#include <iostream>
#include "access_strategy.h"

// 韦根设备相关宏定义
#define WIEGAND_OUT_DEV_NAME "/dev/wiegand_out"

class WiegandStrategy : public AccessStratgy {
private:
    int m_bits;               ///< 韦根位数
    bool m_isReady;           ///< 是否可操作
    bool m_isOpen;            ///< 当前门状态
    std::string m_relayFilePath;
public:
    WiegandStrategy();
    ~WiegandStrategy() override;
    int openDoor(uint64_t data) override;
    int closeDoor(uint64_t data) override;
    AccessDevicesStatus getStatus() override;
    bool initialize() override;
    void release() override;
    void setRelayFilePath(const std::string &relayFilePath);
};

#endif //WIEGAND_STRATEGY_H
