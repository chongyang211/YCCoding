//
// Created by 杨充 on 2025/9/8.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <mutex>

//日志级别枚举
enum class LogLevel {
    INFO,
    DEBUG,
    WARNING,
    ERROR
};

// 日志需要存储到file文件中
class Logger {
private:
    //日志文件file
    std::ofstream logFile;
    //要保证线程安全，日志锁
    std::mutex logMutex;
public:
    //构造函数，传入文件路径，打开文件。如果文件不存在则抛出错误
    Logger(const std::string &fileName);
    //析构函数。做一些资源清理操作
    ~Logger();
    //打印日志，需要传入日志信息，以及日志级别，默认是info日志
    void log(const std::string &message , LogLevel level = LogLevel::INFO);
};


#endif //LOGGER_H
