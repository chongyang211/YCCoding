//
// Created by 杨充 on 2025/9/5.
//

#ifndef IPOLLINGSTRATEGY_H
#define IPOLLINGSTRATEGY_H

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

// ===========================================
// 轮询策略接口和实现
// ===========================================
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

#endif //IPOLLINGSTRATEGY_H
