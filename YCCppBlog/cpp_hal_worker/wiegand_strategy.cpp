//
// Created by 杨充 on 2025/7/28.
//

#include "wiegand_strategy.h"

WiegandStrategy::WiegandStrategy() {
    m_isOpen = false;
    m_isReady = false;
    m_bits = 26;
    setRelayFilePath(WIEGAND_OUT_DEV_NAME);
}

WiegandStrategy::~WiegandStrategy() {
    release();
}

int WiegandStrategy::openDoor(uint64_t data) {

}

