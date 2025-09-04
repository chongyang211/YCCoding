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

    class Poller {
    public:
        // 任务函数类型定义
        typedef std::function<TaskResult()> ActionFunc;
    private:
        // 轮询线程主循环
        void run();
        // 执行任务
        void executeAction();
        std::unique_ptr<IPollingStrategy> strategy_;
        ActionFunc action_;
        std::atomic<bool> running_;
        bool paused_;
        std::thread worker_thread_;
        mutable std::mutex mutex_;
        std::condition_variable cond_;
        long task_timeout_ns_;
    public:
        /**
        * @brief 构造函数
        * @param strategy 轮询策略
        * @param action 要执行的任务函数
        */
        Poller(std::unique_ptr<IPollingStrategy> strategy, ActionFunc action)
            : strategy_(std::move(strategy)),
              action_(action),
              running_(false),
              paused_(false),
              task_timeout_ns_(0) {}
        ~Poller() {
            stop();
        }
        // 启动轮询器
        void start();
        // 停止轮询器
        void stop();
        // 暂停轮询
        void pause();
        // 继续轮询
        void resume();
        // 更新轮询策略
        void updateStrategy(std::unique_ptr<IPollingStrategy> new_strategy);
        // 打印统计信息
        void printStatistics() const;
        // 手动触发任务
        void triggerNow();
        // 设置任务超时时间
        void setTaskTimeout(long timeout, TimeUnit unit);

    };
}

#endif //LOOPPOLLING_H
