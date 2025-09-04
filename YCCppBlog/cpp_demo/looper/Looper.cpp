//
// Created by 杨充 on 2025/9/2.
//

#include "Looper.h"


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

// ===========================================
// 时间单位定义和转换工具
// ===========================================
enum class TimeUnit {
    NANOSECONDS,
    MICROSECONDS,
    MILLISECONDS,
    SECONDS,
    MINUTES
};

/**
 * @brief 时间转换工具类
 */
class TimeConverter {
public:
    static long toNanoseconds(long duration, TimeUnit unit) {
        switch (unit) {
            case TimeUnit::NANOSECONDS: return duration;
            case TimeUnit::MICROSECONDS: return duration * 1000L;
            case TimeUnit::MILLISECONDS: return duration * 1000000L;
            case TimeUnit::SECONDS: return duration * 1000000000L;
            case TimeUnit::MINUTES: return duration * 60000000000L;
            default: throw std::invalid_argument("Invalid time unit");
        }
    }

    static std::string unitToString(TimeUnit unit) {
        switch (unit) {
            case TimeUnit::NANOSECONDS: return "ns";
            case TimeUnit::MICROSECONDS: return "us";
            case TimeUnit::MILLISECONDS: return "ms";
            case TimeUnit::SECONDS: return "s";
            case TimeUnit::MINUTES: return "min";
            default: return "unknown";
        }
    }
};

// ===========================================
// 轮询策略接口和实现
// ===========================================
class IPollingStrategy {
public:
    virtual ~IPollingStrategy() {}
    virtual long getNextInterval() = 0;
    virtual void reset() = 0;
    virtual void recordSuccess() = 0;
    virtual void recordFailure() = 0;
    virtual std::string getConfigInfo() const = 0;
};

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

// ===========================================
// 轮询任务和统计系统
// ===========================================
struct TaskResult {
    bool success;
    std::string message;
    std::chrono::steady_clock::time_point timestamp;

    TaskResult(bool s, const std::string& m, std::chrono::steady_clock::time_point t)
        : success(s), message(m), timestamp(t) {}
};

class TaskStatistics {
public:
    TaskStatistics() : total(0), success(0), failure(0) {}

    void recordResult(const TaskResult& result) {
        total++;
        if (result.success) {
            success++;
        } else {
            failure++;
        }

        // 保留最近100条结果
        recentResults.push_back(result);
        if (recentResults.size() > 100) {
            recentResults.pop_front();
        }
    }

    void printSummary() const {
        std::cout << "=== Task Statistics ===" << std::endl;
        std::cout << "Total runs: " << total << std::endl;
        std::cout << "Success: " << success << " ("
                  << (total ? (success * 100.0 / total) : 0.0) << "%)" << std::endl;
        std::cout << "Failure: " << failure << " ("
                  << (total ? (failure * 100.0 / total) : 0.0) << "%)" << std::endl;

        if (!recentResults.empty()) {
            std::cout << "\nRecent results (last " << recentResults.size() << "):" << std::endl;
            for (const auto& res : recentResults) {
                auto timeStr = formatTime(res.timestamp);
                std::cout << "[" << timeStr << "] "
                          << (res.success ? "SUCCESS" : "FAILURE")
                          << ": " << res.message << std::endl;
            }
        }
    }

private:
    std::string formatTime(const std::chrono::steady_clock::time_point& tp) const {
        // 使用相对时间而不是绝对时间
        auto duration = tp.time_since_epoch();
        auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration % std::chrono::hours(1));
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration % std::chrono::minutes(1));
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration % std::chrono::seconds(1));

        std::stringstream ss;
        ss << std::setfill('0')
           << std::setw(2) << hours.count() % 24 << ":"
           << std::setw(2) << minutes.count() << ":"
           << std::setw(2) << seconds.count() << "."
           << std::setw(3) << milliseconds.count();
        return ss.str();
    }

    int total;
    int success;
    int failure;
    std::deque<TaskResult> recentResults;
};

// ===========================================
// 轮询器核心实现
// ===========================================
class Poller {
public:
    typedef std::function<TaskResult()> ActionFunc;

    /**
     * @brief 构造函数
     * @param strategy 轮询策略
     * @param action 要执行的动作
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

    /**
     * @brief 启动轮询器
     */
    void start() {
        if (running_) return;

        running_ = true;
        worker_thread_ = std::thread(&Poller::run, this);

        std::cout << "Poller started with strategy: "
                  << strategy_->getConfigInfo() << std::endl;
    }

    /**
     * @brief 停止轮询器
     */
    void stop() {
        if (!running_) return;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            running_ = false;
            paused_ = false;
        }

        cond_.notify_all();

        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }

        std::cout << "Poller stopped." << std::endl;
    }

    /**
     * @brief 暂停轮询
     */
    void pause() {
        std::lock_guard<std::mutex> lock(mutex_);
        paused_ = true;
        cond_.notify_all();
        std::cout << "Poller paused." << std::endl;
    }

    /**
     * @brief 继续轮询
     */
    void resume() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            paused_ = false;
        }
        cond_.notify_all();
        std::cout << "Poller resumed." << std::endl;
    }

    /**
     * @brief 动态更新轮询策略
     * @param new_strategy 新的轮询策略
     */
    void updateStrategy(std::unique_ptr<IPollingStrategy> new_strategy) {
        std::lock_guard<std::mutex> lock(mutex_);
        strategy_ = std::move(new_strategy);
        strategy_->reset();
        std::cout << "Strategy updated: " << strategy_->getConfigInfo() << std::endl;
    }

    /**
     * @brief 获取统计信息
     */
    void printStatistics() const {
        stats_.printSummary();
    }

    /**
     * @brief 手动触发一次任务执行
     */
    void triggerNow() {
        std::lock_guard<std::mutex> lock(mutex_);
        cond_.notify_all();
    }

    /**
     * @brief 设置任务超时时间
     * @param timeout 超时时间
     * @param unit 时间单位
     */
    void setTaskTimeout(long timeout, TimeUnit unit) {
        std::lock_guard<std::mutex> lock(mutex_);
        task_timeout_ns_ = TimeConverter::toNanoseconds(timeout, unit);
        std::cout << "Task timeout set to: " << task_timeout_ns_ << " ns" << std::endl;
    }

private:
    void run() {
        while (running_) {
            long next_interval = 0;

            {
                std::unique_lock<std::mutex> lock(mutex_);

                // 计算下一次执行间隔
                next_interval = strategy_->getNextInterval();

                // 等待条件：1) 到达间隔时间 2) 被唤醒 3) 暂停状态
                auto now = std::chrono::steady_clock::now();
                auto wake_time = now + std::chrono::nanoseconds(next_interval);

                while (running_ &&
                       std::chrono::steady_clock::now() < wake_time &&
                       !paused_) {
                    cond_.wait_until(lock, wake_time);
                }

                // 检查是否暂停
                if (paused_) {
                    cond_.wait(lock, [this] { return !paused_ || !running_; });
                    continue;
                }

                // 检查是否停止
                if (!running_) break;
            }

            // 执行任务
            executeAction();
        }
    }

    void executeAction() {
        auto start_time = std::chrono::steady_clock::now();
        TaskResult result(false, "Unknown result", std::chrono::steady_clock::now());

        try {
            if (task_timeout_ns_ > 0) {
                // 带超时的任务执行
                std::future<TaskResult> future = std::async(
                    std::launch::async, action_);

                auto status = future.wait_for(
                    std::chrono::nanoseconds(task_timeout_ns_));

                if (status == std::future_status::ready) {
                    result = future.get();
                } else {
                    result = TaskResult(false, "Task timed out", std::chrono::steady_clock::now());
                }
            } else {
                // 不带超时的任务执行
                result = action_();
            }
        } catch (const std::exception& e) {
            result = TaskResult(false, std::string("Exception: ") + e.what(),
                      std::chrono::steady_clock::now());
        } catch (...) {
            result = TaskResult(false, "Unknown exception", std::chrono::steady_clock::now());
        }

        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time).count();

        // 记录结果并更新策略
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stats_.recordResult(result);

            if (result.success) {
                strategy_->recordSuccess();
            } else {
                strategy_->recordFailure();
            }
        }

        // 输出执行信息
        std::cout << "Action executed in " << duration << "ms. Result: "
                  << (result.success ? "SUCCESS" : "FAILURE")
                  << " - " << result.message << std::endl;
    }

    std::unique_ptr<IPollingStrategy> strategy_;
    ActionFunc action_;
    std::atomic<bool> running_;
    bool paused_;
    std::thread worker_thread_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    TaskStatistics stats_;
    long task_timeout_ns_;
};

} // namespace PollingSystem

// ===========================================
// 示例使用场景
// ===========================================
//g++ -std=c++11 Looper.cpp
int main() {
    using namespace PollingSystem;

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