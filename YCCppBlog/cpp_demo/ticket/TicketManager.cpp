#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <fstream>
#include <chrono>
#include <atomic>
#include <random>
#include <unordered_map>
#include <memory>
#include <queue>
#include <map>
#include "Logger.h"
#include "ConfigManager.h"
#include "TicketSystem.h"


// 售票窗口
class TicketWindow {
private:
    std::string name;   //窗口名称
    TicketSystem& ticketSystem;     //票务系统
    Logger& logger;     //日志系统
    std::atomic<bool> running;      //是否正在运行中
    std::atomic<int> ticketsSold;   //
    std::thread workerThread;       //工作线程
    std::mutex vipMutex;     //票务锁，保证线程安全

public:
    TicketWindow(const std::string& windowName, TicketSystem& system, Logger& log)
        : name(windowName), ticketSystem(system), logger(log), running(true), ticketsSold(0) {}

    // 禁用拷贝构造函数和拷贝赋值运算符
    TicketWindow(const TicketWindow&) = delete;
    TicketWindow& operator=(const TicketWindow&) = delete;

    void start() {
        std::thread([this]() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 5);

            while (running) {
                int numTickets = dis(gen);
                try {
                      if (ticketSystem.sellTicket(numTickets, name)) {
                          ticketsSold += numTickets;
                          std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟出票时间
                      } else {
                          logger.log(name + " failed to sell tickets. Not enough tickets available.", LogLevel::WARNING);
                          break;
                      }
                } catch (const std::exception & e) {
                    logger.log(name + " encountered error: " + std::string(e.what()), LogLevel::ERROR);
                    // 恢复部分票数
                    ticketSystem.recoverTickets(numTickets);
                }

            }
        }).detach();
    }

    void stop() {
        running = false;
    }

    int getTicketsSold() const {
        return ticketsSold;
    }

    // ==== VIP售票方法 ====
    void sellVIP(int num, const std::string& customerName) {
        std::lock_guard<std::mutex> lock(vipMutex);
        if (ticketSystem.sellTicket(num, name + " (VIP)")) {
            ticketsSold += num;
            std::stringstream ss;
            ss << "VIP customer " << customerName << " bought " << num
               << " tickets from " << name;
            logger.log(ss.str());
        }
    }
};

// 顾客
class Customer {
private:
    std::string name;
    TicketSystem& ticketSystem;
    Logger& logger;

public:
    Customer(const std::string& customerName, TicketSystem& system, Logger& log)
        : name(customerName), ticketSystem(system), logger(log) {}

    void buyTickets(int num) {
        std::thread([this, num]() {
            std::stringstream ss;
            ss << name << " is trying to buy " << num << " tickets.";
            logger.log(ss.str());

            if (ticketSystem.sellTicket(num, name)) {
                ss.str("");
                ss << name << " successfully bought " << num << " tickets.";
                logger.log(ss.str());
            } else {
                ss.str("");
                ss << name << " failed to buy " << num << " tickets. Not enough tickets available.";
                logger.log(ss.str(), LogLevel::WARNING);
            }
        }).detach();
    }

    // ==== VIP购票方法 ====
    void buyVIP(int num, TicketWindow& vipWindow) {
        vipWindow.sellVIP(num, name);
    }
};

// ==== 性能监控类 ====
class PerformanceMonitor {
private:
    std::atomic<int> totalSales;
    std::atomic<int> successfulSales;
    std::atomic<int> failedSales;
    std::chrono::steady_clock::time_point startTime;
    Logger& logger;

public:
    PerformanceMonitor(Logger& log) :
        totalSales(0), successfulSales(0), failedSales(0), logger(log) {
        startTime = std::chrono::steady_clock::now();
    }

    void recordSale(bool success) {
        totalSales++;
        if (success) successfulSales++;
        else failedSales++;
    }

    void report() {
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
};

//g++ -std=c++11 Logger.cpp ConfigManager.cpp TicketSystem.cpp TicketManager.cpp
// 主程序
int main() {
    // 读取配置文件
    ConfigManager config("config.txt");
    int totalTickets = config.get("total_tickets");
    int numWindows = config.get("num_windows");
    int numCustomers = config.get("num_customers");
    int maxTicketsPerCustomer = config.get("max_tickets_per_customer");

    // 初始化日志系统
    Logger logger("ticket_system.log");
    PerformanceMonitor monitor(logger);
    // 初始化票务系统
    TicketSystem ticketSystem(totalTickets, logger);

    // 创建售票窗口
    std::vector<std::unique_ptr<TicketWindow>> windows;
    for (int i = 1; i <= numWindows; ++i) {
        windows.push_back(std::unique_ptr<TicketWindow>(new TicketWindow("Window " + std::to_string(i), ticketSystem, logger)));
    }
    windows.push_back(std::unique_ptr<TicketWindow>(new TicketWindow("VIP Window ", ticketSystem, logger)));

    // 启动售票窗口
    for (auto& window : windows) {
        window->start();
    }

    // 创建顾客
    std::vector<Customer> customers;
    for (int i = 1; i <= numCustomers; ++i) {
        customers.emplace_back("Customer " + std::to_string(i), ticketSystem, logger);
    }

    // 创建VIP窗口
    // auto vipWindow = std::make_unique<TicketWindow>("VIP Window", ticketSystem, logger);
    // vipWindow->start();
    //
    // // VIP顾客购票
    // for (int i = 0; i < 3; i++) {
    //     customers[i].buyVIP(3, *vipWindow);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }

    // 顾客购票
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, maxTicketsPerCustomer);

    for (auto& customer : customers) {
        int numTickets = dis(gen);
        customer.buyTickets(numTickets);
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 模拟顾客购票间隔
    }

    // 启动票务监控
    ticketSystem.monitorTickets();

    // 动态增加票数
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ticketSystem.addTickets(50);

    // 等待所有售票窗口停止
    std::this_thread::sleep_for(std::chrono::seconds(5));
    for (auto& window : windows) {
        window->stop();
    }

    // 输出统计信息
    std::stringstream ss;
    ss << "All tickets sold. Remaining tickets: " << ticketSystem.getRemainingTickets();
    logger.log(ss.str());

    for (const auto& window : windows) {
        ss.str("");
        ss << window->getTicketsSold() << " tickets sold by " << window->getTicketsSold();
        logger.log(ss.str());
    }

    // 结束时报告
    monitor.report();

    return 0;
}