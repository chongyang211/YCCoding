//
// Created by 杨充 on 2025/9/4.
//

#ifndef LOOPPOLLING_H
#define LOOPPOLLING_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <atomic>
#include <queue>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <random>
#include <deque>
#include <future>
#include <ctime>

namespace PollingSystem {
    // 所有实现将放在这个命名空间内
    enum class TimeUnit {
        NANOSECONDS,
        MICROSECONDS,
        MILLISECONDS,
        SECONDS,
        MINUTES
    };

    class IPollingStrategy {
    public:
        //析构函数
        virtual ~IPollingStrategy(){}
        //获取下一次轮训
        virtual long getNextInterval() = 0;
        //重置策略状态
        virtual void reset() = 0;
        //记录任务成功
        virtual void recordSuccess() = 0;
        //记录任务失败
        virtual void recordFailure() = 0;
        //获取策略配置信息
        virtual std::string getConfigInfo() const = 0;
    };

    struct TaskResult {
        bool success;
        std::string message;
        std::chrono::steady_clock::time_point timestamp;
        TaskResult(bool s, const std::string& m,
           std::chrono::steady_clock::time_point t)
            : success(s), message(m), timestamp(t) {}
    };

}

#endif //LOOPPOLLING_H
