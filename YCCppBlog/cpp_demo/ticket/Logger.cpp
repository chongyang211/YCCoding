//
// Created by 杨充 on 2025/9/8.
//

#include "Logger.h"

#include <iostream>

//在构造函数中传入文件路径，打开文件。如果文件不存在则抛出错误
Logger::Logger(const std::string &fileName) {
    //往里面写日志，并且可以追加
    //std::ios::out表示写入内容
    //可以使用std::ios::app标志来指定追加模式，它会将数据追加到文件的末尾而不是覆盖原有内容。
    logFile.open(fileName, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file.");
    }
}

//在析构函数中，关闭文件流
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string &message, LogLevel level) {
    //多线程安全锁，这里用一下 lock_guard 锁
    //自动管理锁的生命周期，离开作用域时，自动释放锁
    std::lock_guard<std::mutex> lock(logMutex);
    std::string levelStr;
    switch (level) {
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::DEBUG: levelStr = "DEBUG"; break;
        case LogLevel::WARNING: levelStr = "WARNING"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
    }
    std::string logMessage = "[" + levelStr + "]" + message;
    //将日志追加到file文件中
    logFile << logMessage << std::endl;
    //将日志打印出来
    std::cout << logMessage << std::endl;
}


