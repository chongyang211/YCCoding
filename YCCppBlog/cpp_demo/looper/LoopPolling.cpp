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
        engine(std::random_device()()) {
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
        std::cout << "Poller started with strategy: " << strategy_->getConfigInfo() << std::endl;
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

    // 更新轮询策略
    void Poller::updateStrategy(std::unique_ptr<IPollingStrategy> new_strategy) {
        std::lock_guard<std::mutex> lock(mutex_);
        strategy_ = std::move(new_strategy);
        strategy_->reset();
        std::cout << "Strategy updated: " << strategy_->getConfigInfo() << std::endl;
    }

    // 打印统计信息
    void Poller::printStatistics() const {
        // stats_.printSummary();
    }

    // 手动触发任务
    void Poller::triggerNow() {
        std::lock_guard<std::mutex> lock(mutex_);
        cond_.notify_all();
    }

    // 设置任务超时时间
    void Poller::setTaskTimeout(long timeout, TimeUnit unit) {
        std::lock_guard<std::mutex> lock(mutex_);
        task_timeout_ns_ = TimeConverter::toNanoseconds(timeout, unit);
        std::cout << "Task timeout set to: " << task_timeout_ns_ << " ns" << std::endl;
    }

    // 轮询线程主循环
    void Poller::run() {
        while (running_) {
            //设置间隔时间。
            long next_interval = 0;
            {
                //根据不同的策略设置时间
                std::unique_lock<std::mutex> lock(mutex_);
                //计算下一次执行间隔时间
                next_interval = strategy_->getNextInterval();
                // 等待条件：1) 到达间隔时间 2) 被唤醒 3) 暂停状态
                auto now = std::chrono::steady_clock::now();
                auto wake_time = now + std::chrono::nanoseconds(next_interval);
                while (running_ &&std::chrono::steady_clock::now() < wake_time &&!paused_) {
                    cond_.wait_until(lock, wake_time);
                }
                // 检查是否暂停
                if (paused_) {
                    cond_.wait(lock, [this] { return !paused_ || !running_; });
                    continue;
                }
                // 检查是否停止
                if (!running_) {
                    break;
                }
            }
            // 执行任务
            executeAction();
        }
    }

    void Poller::executeAction() {
        auto start_time = std::chrono::steady_clock::now();
        TaskResult result(false, "Unknown result", std::chrono::steady_clock::now());
        try {
            if (task_timeout_ns_ > 0) {
                // 带超时的任务执行
                std::future<TaskResult> future = std::async(std::launch::async, action_);
                auto status = future.wait_for(std::chrono::nanoseconds(task_timeout_ns_));
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
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        // 记录结果并更新策略
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (result.success) {
                strategy_->recordSuccess();
            } else {
                strategy_->recordFailure();
            }
        }
        // 输出执行信息
        std::cout << "Action executed in " << duration << "ms. Result: "
                  << (result.success ? "SUCCESS" : "FAILURE") << " - " << result.message << std::endl;
    }


}



void test1() {
    PollingSystem::FixedIntervalStrategy strategy(1000, PollingSystem::TimeUnit::MILLISECONDS); // 固定间隔 1000ms
    std::cout << "Config: " << strategy.getConfigInfo() << std::endl;
    // 获取下一次轮询间隔
    std::cout << "Next interval: " << strategy.getNextInterval() << " ns" << std::endl;
}

void test9() {
    auto aciton = []() -> PollingSystem::TaskResult {
        std::cout << "开始执行轮训action" << std::endl;
        static int counter = 0;
        counter++;
        if (counter % 10 < 3) {
            // 模拟失败
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            return PollingSystem::TaskResult(false, "Simulated failure #" + std::to_string(counter),
                                             std::chrono::steady_clock::now());
        }
        return PollingSystem::TaskResult(true, "Operation succeeded #" + std::to_string(counter),
                                         std::chrono::steady_clock::now());
    };
    PollingSystem::IPollingStrategy *strategy = new PollingSystem::FixedIntervalStrategy(1000,PollingSystem::TimeUnit::MILLISECONDS);
    PollingSystem::Poller poller(std::unique_ptr<PollingSystem::IPollingStrategy> (strategy),aciton);
    // 启动轮询器
    poller.start();
}

//g++ -std=c++11 LoopPolling.cpp
int main() {
    // test1();
    test9();
    // 示例使用场景
    return 0;
}