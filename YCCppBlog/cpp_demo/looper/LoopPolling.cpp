//
// Created by 杨充 on 2025/9/4.
//

#include "LoopPolling.h"


namespace PollingSystem {
    // 实现代码将放在这里

    class TimeConverter {
    public:
        // 将时间转化为秒
        static long toNanoseconds(long duration , TimeUnit unit) {
            switch (unit) {
                case TimeUnit::NANOSECONDS:
                    return duration;
                case TimeUnit::MICROSECONDS:
                    return duration * 1000;
                case TimeUnit::MILLISECONDS:
                    return duration * 1000 * 1000;
                case TimeUnit::SECONDS:
                    return duration * 1000 * 1000 * 1000;
                case TimeUnit::MINUTES:
                    return duration * 60000000000L;
                default: throw std::invalid_argument("Invalid time unit");
            }
        }

        // 获取时间单位的字符串表示
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

    //固定间隔策略
    class FixedIntervalStrategy : public IPollingStrategy {
    private:
        const long interval_ns;
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
    };

    //指数退避策略
    class ExponentialBackoffStrategy : public IPollingStrategy {
    private:
        const long initial_ns;  //初始等待时间，由构造函数传入。
        const long max_ns;  //最大等待时间，由构造函数传入。
        long current_ns;
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
        virtual void recordFailure() override {}

        virtual std::string getConfigInfo() const override {
            std::ostringstream oss;
            oss << "ExponentialBackoff: Initial=" << initial_ns
                << "ns, Max=" << max_ns << "ns";
            return oss.str();
        }
    };

    //随机间隔策略
    class RandomIntervalStrategy : public IPollingStrategy {
    private:
        const long min_ns;
        const long max_ns;
        std::default_random_engine engine;
    public:
        RandomIntervalStrategy(long min, long max, TimeUnit unit) :
        min_ns(TimeConverter::toNanoseconds(min,unit)),
        max_ns(TimeConverter::toNanoseconds(max,unit)),
        engine(std::random_device()) {
        }

        virtual long getNextInterval() override {
            std::uniform_int_distribution<long> dist(min_ns,max_ns);
            return dist(engine);
        }
        virtual void reset() override {}
        virtual void recordSuccess() override {}
        virtual void recordFailure() override {}

        virtual std::string getConfigInfo() const override {
            std::ostringstream oss;
            oss << "RandomInterval: Min=" << min_ns
                << "ns, Max=" << max_ns << "ns";
            return oss.str();
        }
    };


    /**
     * @brief 任务统计系统
     */
    class TaskStatistics {
    public:
        TaskStatistics() : total(0), success(0), failure(0) {}

        // 记录任务结果
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

        // 打印统计摘要
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
                    std::cout << "[" << timeStr << "] " << (res.success ? "SUCCESS" : "FAILURE")
                              << ": " << res.message << std::endl;
                }
            }
        }

    private:
        // 格式化时间戳
        std::string formatTime(const std::chrono::steady_clock::time_point& tp) const {
            // 使用相对时间而不是绝对时间
            auto duration = tp.time_since_epoch();
            auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(
                duration % std::chrono::hours(1));
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                duration % std::chrono::minutes(1));
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                duration % std::chrono::seconds(1));
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

    void Poller::start() {
        if (running_) {
            return;
        }
        running_ = true;
        worker_thread_ = std::thread(&Poller::run,this);
        std::cout << "Poller started with strategy: "
             << strategy_->getConfigInfo() << std::endl;
    }

    void Poller::stop() {
        if (!running_) {
            return;
        }
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

    // 暂停轮询
    void Poller::pause() {
        std::lock_guard<std::mutex> lock(mutex_);
        paused_ = true;
        cond_.notify_all();
        std::cout << "Poller paused." << std::endl;
    }

    void Poller::resume() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            paused_ = false;
        }
        cond_.notify_all();
        std::cout << "Poller resumed." << std::endl;
    }

}



void test1() {
    PollingSystem::FixedIntervalStrategy strategy(1000, PollingSystem::TimeUnit::MILLISECONDS); // 固定间隔 1000ms
    std::cout << "Config: " << strategy.getConfigInfo() << std::endl;
    // 获取下一次轮询间隔
    std::cout << "Next interval: " << strategy.getNextInterval() << " ns" << std::endl;
}

//g++ -std=c++11 LoopPolling.cpp
int main() {
    test1();
    // 示例使用场景
    return 0;
}