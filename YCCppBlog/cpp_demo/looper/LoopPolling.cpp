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

}

int main() {
    // 示例使用场景
    return 0;
}