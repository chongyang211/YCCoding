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