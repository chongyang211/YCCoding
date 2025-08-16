//
// Created by 杨充 on 2025/8/16.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <atomic>

//  g++ -std=c++11
using namespace std;

//日志级别枚举
enum class LogLevel {
    INFO,
    DEBUG,
    WARNING,
    ERROR
};

class Logger {
private:
    //日志文件file
    std::ofstream logFile;
    //日志锁，多线程安全
    std::mutex logMutex;
public:
    //在构造函数中传入文件路径，打开文件。如果文件不存在则抛出错误
    Logger(const std::string& fileName) {
        logFile.open(fileName,std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file.");
        }
    }
    //在析构函数中，关闭文件流
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    //打印日志，需要传入打印内容和打印日志级别，默认是info日志类型
    void log(const std::string& message , LogLevel level = LogLevel::INFO) {
        //多线程安全，这里用lock_guard锁
        std::lock_guard<std::mutex> lock(logMutex);
        std::string levelStr;
        switch (level) {
            case LogLevel::INFO: levelStr = "INFO"; break;
            case LogLevel::DEBUG: levelStr = "DEBUG"; break;
            case LogLevel::WARNING: levelStr = "WARNING"; break;
            case LogLevel::ERROR: levelStr = "ERROR"; break;
        }
        std::string logMessage = "[" + levelStr + "] " + message;
        logFile << logMessage << std::endl;
        std::cout << logMessage << std::endl;
    }
};

int main() {
    // 初始化日志系统
    Logger logger("ticket_system_log.txt");
    logger.log("System initialized", LogLevel::INFO);
    logger.log("System initialized debug", LogLevel::DEBUG);
    logger.log("System initialized warning", LogLevel::WARNING);
    logger.log("System initialized error", LogLevel::ERROR);
    return 0;
}



