//
// Created by 杨充 on 2025/9/9.
//

#include "PerformanceMonitor.h"

#include <sstream>

//构造函数，初始化成员变量并记录开始时间。
PerformanceMonitor::PerformanceMonitor(Logger &log) : totalSales(0), successfulSales(0), failedSales(0), logger(log) {
    startTime = std::chrono::steady_clock::now();
}

//记录一次销售尝试，根据成功与否更新计数器。
void PerformanceMonitor::recordSale(bool success) {
    totalSales++;
    if (success) {
        successfulSales++;
    } else {
        failedSales++;
    }
}

//生成性能报告并输出到日志。
void PerformanceMonitor::report() {
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    std::stringstream ss;
    ss << "\n==== PERFORMANCE REPORT ====\n";
    ss << "Total sales attempts: " << totalSales << "\n";
    ss << "Successful sales: " << successfulSales << "\n";
    ss << "Failed sales: " << failedSales << "\n";
    ss << "Success rate: "
       << (totalSales > 0 ? 100.0 * successfulSales / totalSales : 0) << "%\n";
    ss << "Simulation duration: " << duration.count() << " seconds\n";
    ss << "Transactions per second: "
       << (duration.count() > 0 ? totalSales / duration.count() : 0) << "\n";
    logger.log(ss.str());
}

//提供重置功能，允许重新开始性能监控。
void PerformanceMonitor::reset() {
    totalSales = 0;
    successfulSales = 0;
    failedSales = 0;
    startTime = std::chrono::steady_clock::now();
}


