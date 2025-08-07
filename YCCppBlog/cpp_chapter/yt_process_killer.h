//
// Created by 杨充 on 2025/8/7.
//

#ifndef YT_RROCESS_KILLER_H
#define YT_RROCESS_KILLER_H

#include <string>

class ProcessKiller {
public:
    // 使用 pkill 杀死进程
    static bool killByPkill(const std::string& processName);
    // 使用 killall 杀死进程
    static bool killByKillall(const std::string& processName);
    // 使用 pidof + kill 杀死进程
    static bool killByPidof(const std::string& processName);
    // 使用 fork + exec 直接调用 pkill
    static bool killByForkExec(const std::string& processName);
};

#endif //YT_RROCESS_KILLER_H
