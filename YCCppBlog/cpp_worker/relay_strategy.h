//
// Created by 杨充 on 2025/7/28.
//

#ifndef RELAY_STRATEGY_H
#define RELAY_STRATEGY_H
#include <iostream>
#include "access_strategy.h"

class RelayStrategy : public AccessStratgy{
private:
    bool m_isReady;
    bool m_isOpen;
    std::string m_relayFilePath;
public:
    RelayStrategy();
    ~RelayStrategy() override;
    int openDoor() override;
    int openDoor(uint64_t data) override;
    int closeDoor() override;
    int closeDoor(uint64_t data) override;
    AccessDevicesStatus getStatus() override;
    bool initialize() override;
    void release() override;
    void setRelayFilePath(const std::string &relayFilePath);
};

#endif //RELAY_STRATEGY_H
