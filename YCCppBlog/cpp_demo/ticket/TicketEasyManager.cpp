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

// 日志级别
enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

// 日志系统
class Logger {
private:
    std::ofstream logFile;
    std::mutex logMutex;

public:
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file.");
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const std::string& message, LogLevel level = LogLevel::INFO) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::string levelStr;
        switch (level) {
            case LogLevel::INFO: levelStr = "INFO"; break;
            case LogLevel::WARNING: levelStr = "WARNING"; break;
            case LogLevel::ERROR: levelStr = "ERROR"; break;
        }
        std::string logMessage = "[" + levelStr + "] " + message;
        logFile << logMessage << std::endl;
        std::cout << logMessage << std::endl;
    }
};

// 系统配置管理
class ConfigManager {
private:
    std::unordered_map<std::string, int> config;

public:
    ConfigManager(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string key;
            int value;
            while (file >> key >> value) {
                config[key] = value;
            }
        } else {
            // 默认配置
            config["total_tickets"] = 100;
            config["num_windows"] = 3;
            config["num_customers"] = 10;
            config["max_tickets_per_customer"] = 5;
        }
    }

    int get(const std::string& key) const {
        auto it = config.find(key);
        if (it != config.end()) {
            return it->second;
        }
        throw std::runtime_error("Config key not found: " + key);
    }
};

// 票务系统
class TicketSystem {
private:
    int totalTickets;
    std::atomic<int> remainingTickets;
    std::mutex ticketMutex;
    std::condition_variable cv;
    Logger& logger;

public:
    TicketSystem(int total, Logger& log) : totalTickets(total), remainingTickets(total), logger(log) {}

    bool sellTicket(int num, const std::string& windowName) {
        std::unique_lock<std::mutex> lock(ticketMutex);
        cv.wait(lock, [this, num] { return remainingTickets >= num; });

        if (remainingTickets >= num) {
            remainingTickets -= num;
            std::stringstream ss;
            ss << windowName << " sold " << num << " tickets. Remaining tickets: " << remainingTickets;
            logger.log(ss.str());
            cv.notify_all();
            return true;
        }
        return false;
    }

    void addTickets(int num) {
        std::lock_guard<std::mutex> lock(ticketMutex);
        remainingTickets += num;
        std::stringstream ss;
        ss << "Added " << num << " tickets. Remaining tickets: " << remainingTickets;
        logger.log(ss.str());
        cv.notify_all();
    }

    int getRemainingTickets() const {
        return remainingTickets;
    }

    int getTotalTickets() const {
        return totalTickets;
    }
};

// 售票窗口
class TicketWindow {
private:
    std::string name;
    TicketSystem& ticketSystem;
    Logger& logger;
    std::atomic<bool> running;
    std::atomic<int> ticketsSold;

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
                if (ticketSystem.sellTicket(numTickets, name)) {
                    ticketsSold += numTickets;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟出票时间
                } else {
                    logger.log(name + " failed to sell tickets. Not enough tickets available.", LogLevel::WARNING);
                    break;
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
};

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

    // 初始化票务系统
    TicketSystem ticketSystem(totalTickets, logger);

    // 创建售票窗口
    std::vector<std::unique_ptr<TicketWindow>> windows;
    for (int i = 1; i <= numWindows; ++i) {
        windows.push_back(std::unique_ptr<TicketWindow>(new TicketWindow("Window " + std::to_string(i), ticketSystem, logger)));
    }

    // 启动售票窗口
    for (auto& window : windows) {
        window->start();
    }

    // 创建顾客
    std::vector<Customer> customers;
    for (int i = 1; i <= numCustomers; ++i) {
        customers.emplace_back("Customer " + std::to_string(i), ticketSystem, logger);
    }

    // 顾客购票
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, maxTicketsPerCustomer);

    for (auto& customer : customers) {
        int numTickets = dis(gen);
        customer.buyTickets(numTickets);
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 模拟顾客购票间隔
    }

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

    return 0;
}