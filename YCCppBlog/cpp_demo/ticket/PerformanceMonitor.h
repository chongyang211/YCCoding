//
// Created by 杨充 on 2025/9/9.
//

#ifndef PERFORMANCEMONITOR_H
#define PERFORMANCEMONITOR_H
#include <atomic>
#include "Logger.h"

//性能监控类
//PerformanceMonitor，它记录了销售尝试的总数、成功和失败的次数，并生成性能报告。
//接口简单，只需调用 recordSale 记录销售结果，调用 report 生成报告。
class PerformanceMonitor {
private:
    //使用 std::atomic 保证 totalSales、successfulSales 和 failedSales 的线程安全，适合多线程环境。
    std::atomic<int> totalSales;    //记录总的销售尝试次数（原子变量，线程安全）。
    std::atomic<int> successfulSales;   //记录成功的销售次数（原子变量，线程安全）。
    std::atomic<int> failedSales;   //记录失败的销售次数（原子变量，线程安全）。
    std::chrono::steady_clock::time_point startTime;    //记录性能监控的开始时间。
    Logger& logger;     //日志记录器，用于输出性能报告。
public:
    //构造函数，初始化成员变量并记录开始时间。
    PerformanceMonitor(Logger& log);
    //记录一次销售尝试，根据成功与否更新计数器。
    void recordSale(bool success);
    //生成性能报告并输出到日志。
    void report();
    //提供重置功能，允许重新开始性能监控。
    void reset();
};



#endif //PERFORMANCEMONITOR_H
