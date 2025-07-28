//
// Created by 杨充 on 2025/7/28.
//

#include "relay_strategy.h"

#include <fstream>

#include "strategy_utils.h"

RelayStrategy::~RelayStrategy() {
   release();
}

RelayStrategy::RelayStrategy() {
    m_isReady = false;
    m_isOpen = false;
}

int RelayStrategy::openDoor() {
    if (!m_isReady) {
        return ERROR_NOT_INIT;
    }
    // 系统调用继电器文件 "1" 表示继电器on
    if (StrategyUtils::GetInstance()->writeRelayFile(m_relayFilePath, "1")) {
        m_isOpen = true;
        return SUCCEED;
    } else {
        return ERROR_RELAY_FILE_WRITE_ERROR;
    }
}

int RelayStrategy::openDoor(uint64_t data) {
    return openDoor();
}

int RelayStrategy::closeDoor() {
    if (!m_isReady) {
        return ERROR_NOT_INIT;
    }
    // 系统调用继电器文件 "0" 表示继电器off
    if (StrategyUtils::GetInstance()->writeRelayFile(m_relayFilePath, "0")) {
        m_isOpen = false;
        return SUCCEED;
    }
    return ERROR_RELAY_FILE_WRITE_ERROR;
}

int RelayStrategy::closeDoor(uint64_t data) {
    return closeDoor();
}

AccessDevicesStatus RelayStrategy::getStatus() {
    if (!m_isReady) {
        return kUnknown;
    }
    if (m_isOpen) {
        return kOpened;
    }
    return kClosed;
}

bool RelayStrategy::initialize() {
    std::string relayFilePath = "/sys/ex_ctl/relay_switch";
    std::ifstream ifs;
    ifs.open(relayFilePath.c_str());
    if(ifs.is_open()) {
        setRelayFilePath(relayFilePath);
        m_isReady = true;
        return true;
    }
    m_isReady = false;
    return false;
}

void RelayStrategy::release() {
    m_isReady = false;
    m_isOpen = false;
}

void RelayStrategy::setRelayFilePath(const std::string &relayFilePath) {
    m_relayFilePath = relayFilePath;
}






