//
// Created by 杨充 on 2025/7/28.
//

#ifndef ACCESSSTRATEGY_H
#define ACCESSSTRATEGY_H
#include <cstdint>

enum AccessDevicesStatus {
    kUnknown = 0,
    kReady, // 设备就绪
    kOpened, // 门已开
    kClosed, // 门已关
    kError, // 设备异常
    kOffline // 设备离线
};

enum ErrorCode {
    SUCCEED = 0,
    ERROR_INVALID_PARAM = 1,
    ERROR_NOT_INIT = 2,
    ERROR_TIMEOUT = 3,
    ERROR_NOT_DEFINED = 4,
    ERROR_PARSE_JSON_FAILED = 5,
    ERROR_SERVER_CODE_ERROR = 6,
    ERROR_NOT_SUPPORT = 7,
    ERROR_NETWORK_FAILED = 8,
    ERROR_PROCESS_BUSY = 9, // 通用流程繁忙/互斥
    ERROR_RELAY_FILE_NOT_FOUND = 221, // 系统调用文件不存在
    ERROR_RELAY_FILE_WRITE_ERROR = 222, // 系统调用文件写入错误
    ERROR_NO_STRATEGY = 223, // 没有策略，没有设置策略
};

class AccessStratgy {
public:
    //virtual ~AccessStratgy();
    virtual ~AccessStratgy() {}
    virtual int openDoor(uint64_t data) = 0;
    virtual int closeDoor(uint64_t data) = 0;
    virtual AccessDevicesStatus getStatus() = 0;
    virtual bool initialize() = 0;
    virtual void release() {}
};

#endif //ACCESSSTRATEGY_H
