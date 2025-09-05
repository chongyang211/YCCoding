//
// Created by 杨充 on 2025/9/2.
//

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

#include "Looper.h"
#include "IPollingStrategy.h"
#include "Poller.h"
#include "TaskStatistics.h"
#include "TimeConverter.h"
#include "TimeUnit.h"

// namespace PollingSystem {


/**
 * @brief 固定间隔轮询策略
 */
class FixedIntervalStrategy : public IPollingStrategy {
public:
    FixedIntervalStrategy(long interval, TimeUnit unit)
        : interval_ns(TimeConverter::toNanoseconds(interval, unit)) {}

    virtual long getNextInterval() override { return interval_ns; }
    virtual void reset() override {}
    virtual void recordSuccess() override {}
    virtual void recordFailure() override {}

    virtual std::string getConfigInfo() const override {
        std::ostringstream oss;
        oss << "FixedInterval: " << interval_ns << " ns";
        return oss.str();
    }

private:
    const long interval_ns;
};

/**
 * @brief 指数退避策略
 */
class ExponentialBackoffStrategy : public IPollingStrategy {
public:
    ExponentialBackoffStrategy(long initial, long max, TimeUnit unit)
        : initial_ns(TimeConverter::toNanoseconds(initial, unit)),
          max_ns(TimeConverter::toNanoseconds(max, unit)),
          current_ns(initial_ns) {}

    virtual long getNextInterval() override {
        long result = current_ns;
        current_ns = std::min(current_ns * 2, max_ns);
        return result;
    }

    virtual void reset() override { current_ns = initial_ns; }

    virtual void recordSuccess() override { reset(); }

    virtual void recordFailure() override {
        // 失败时自动增加间隔 (在getNextInterval中实现)
    }

    virtual std::string getConfigInfo() const override {
        std::ostringstream oss;
        oss << "ExponentialBackoff: Initial=" << initial_ns << "ns, Max=" << max_ns << "ns";
        return oss.str();
    }

private:
    const long initial_ns;
    const long max_ns;
    long current_ns;
};

/**
 * @brief 随机间隔策略
 */
class RandomIntervalStrategy : public IPollingStrategy {
public:
    RandomIntervalStrategy(long min, long max, TimeUnit unit)
        : min_ns(TimeConverter::toNanoseconds(min, unit)),
          max_ns(TimeConverter::toNanoseconds(max, unit)),
          engine(std::random_device()()) {}

    virtual long getNextInterval() override {
        std::uniform_int_distribution<long> dist(min_ns, max_ns);
        return dist(engine);
    }

    virtual void reset() override {}
    virtual void recordSuccess() override {}
    virtual void recordFailure() override {}

    virtual std::string getConfigInfo() const override {
        std::ostringstream oss;
        oss << "RandomInterval: Min=" << min_ns << "ns, Max=" << max_ns << "ns";
        return oss.str();
    }

private:
    const long min_ns;
    const long max_ns;
    std::default_random_engine engine;
};


// } // namespace PollingSystem

int main() {

    // 示例任务函数
    auto sampleAction = []() -> TaskResult {
        // 模拟任务执行，有30%的失败率
        static int counter = 0;
        counter++;

        if (counter % 10 < 3) {
            // 模拟失败
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            return TaskResult(false, "Simulated failure #" + std::to_string(counter),
                             std::chrono::steady_clock::now());
        }

        // 模拟成功
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return TaskResult(true, "Operation succeeded #" + std::to_string(counter),
                         std::chrono::steady_clock::now());
    };

    // 创建轮询器 - 初始使用固定间隔策略
    Poller poller(
        std::unique_ptr<IPollingStrategy>(
            new FixedIntervalStrategy(1000, TimeUnit::MILLISECONDS)),
        sampleAction
    );

    // 设置任务超时
    poller.setTaskTimeout(100, TimeUnit::MILLISECONDS);

    // 启动轮询器
    poller.start();

    // 运行一段时间
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 暂停轮询
    poller.pause();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 继续轮询
    poller.resume();

    // 运行一段时间
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 切换到指数退避策略
    poller.updateStrategy(
        std::unique_ptr<IPollingStrategy>(
            new ExponentialBackoffStrategy(500, 5000, TimeUnit::MILLISECONDS))
    );

    // 再运行一段时间
    std::this_thread::sleep_for(std::chrono::seconds(8));

    // 手动触发一次任务
    std::cout << "Manually triggering task..." << std::endl;
    poller.triggerNow();

    // 再运行一段时间
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 停止轮询器并打印统计信息
    poller.stop();
    poller.printStatistics();

    return 0;
}
