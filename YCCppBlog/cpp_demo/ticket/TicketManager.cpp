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
#include <unordered_map>

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

class ConfigManager {
private:
    //用map集合存储配置数据
    std::unordered_map<std::string,int> config;
public:
    //从本地文件读取配置数据，然后存储到map集合
    ConfigManager(const std::string & fileName) {
        std::ifstream file(fileName);
        if (file.is_open()) {
            std::string key;
            int value;
            while (file >> key >> value) {
                config[key] = value;
                std::cout << "config key : " << key << " , value : " << value << endl;
            }
        } else {
            //如果没有本地文件，则默认配置
            config["total_tickets"] = 100;
            config["num_windows"] = 3;
            config["num_customers"] = 10;
            config["max_tickets_per_customer"] = 5;
            config["simulation_duration"] = 10; // 秒
        }
    }

    //获取某项属性
    int get(const std::string & key) const {
        //auto 自动变量，编译器自动推导类型
        auto it = config.find(key);
        if (it != config.end()) {
            return it->second;
        }
        throw std::runtime_error("Config key not found: " + key);
    }
};

//设计票务系统类
class TicketSystem {
private:
    int totalTicket;    //总票数
    std::atomic<int> remainingTickets;      //正在出的票
    std::mutex ticketMutex;     //票务锁，保证线程安全
    std::condition_variable cv;     //条件变量，用于线程通信，可以和锁结合使用
    Logger & logger;
public:
    TicketSystem(int total , Logger & log) :
        totalTicket(total) , remainingTickets(total) , logger(log) {
        logger.log("Ticket system created with " + std::to_string(total) + " tickets");
    }

    //售票方法
    bool sellTicket(int num , const std::string &windowName) {
        //使用 std::unique_lock 锁定互斥锁，确保对共享资源 remainingTickets 的访问是线程安全的。
        std::unique_lock<std::mutex> lock(ticketMutex);
        //使用条件变量 cv 等待，直到满足条件 remainingTickets >= num 或 remainingTickets == 0。
        //这样可以避免忙等待，提高效率。
        cv.wait(lock, [this, num] {
            return remainingTickets >= num || remainingTickets == 0;
        });
        if (remainingTickets >= num) {
            //如果 remainingTickets >= num，则减少剩余票数，记录日志，并通过 cv.notify_all() 通知其他等待的线程。
            remainingTickets -= num;
            std::stringstream ss;
            ss << windowName << " sold " << num << " tickets. Remaining tickets: " << remainingTickets;
            logger.log(ss.str());
            cv.notify_all();    //刷新
            return true;
        } else {
            //如果 remainingTickets < num，则返回 false，表示售票失败。
            //记录警告日志
            std::stringstream ss;
            ss << windowName << " failed to sell " << num << " tickets. Not enough tickets available.";
            logger.log(ss.str(), LogLevel::WARNING);
            return false;
        }
    }

    //加票方法
    void addTickets(int num) {
        // 检查参数
        if (num <= 0) {
            std::stringstream ss;
            ss << "Failed to add tickets. Invalid number of tickets: " << num;
            logger.log(ss.str(), LogLevel::WARNING);
            return;
        }
        std::lock_guard<std::mutex> lock(ticketMutex);
        remainingTickets+=num;
        std::stringstream ss;
        ss << "Added " << num << " tickets. Remaining tickets: " << remainingTickets;
        logger.log(ss.str());
        cv.notify_all();
    }

    int getRemainingTickets() const {
        return remainingTickets;
    }

    int getTotalTickets() const {
        return totalTicket;
    }
};

//售票窗口类
class TicketWindow {
private:
    std::string name;   //窗口名称
    TicketSystem& ticketSystem;     //票务系统
    Logger& logger;     //日志系统
    std::atomic<bool> running;      //是否正在运行中
    std::atomic<int> ticketsSold;   //
    std::thread workerThread;       //工作线程
public:
    TicketWindow(const std::string& windowName, TicketSystem& system, Logger& log)
        : name(windowName), ticketSystem(system), logger(log), running(true), ticketsSold(0) {
        logger.log("Ticket window " + name + " created");
    }

    // 禁用拷贝构造函数和拷贝赋值运算符
    TicketWindow(const TicketWindow&) = delete;
    TicketWindow& operator=(const TicketWindow&) = delete;

    //启动售票线程
    void start() {
        workerThread = std::thread([this] {
            logger.log(name + " started selling tickets");
            //使用随机数生成器生成随机数。
            std::random_device rd;
            std::mt19937 gen(rd());
            //生成 1 到 5 之间的随机数，模拟每次售票的数量。
            std::uniform_int_distribution<> dis(1, 5);
            while (running) {
                int numTickets = dis(gen);
                //尝试售票。
                if (ticketSystem.sellTicket(numTickets, name)) {
                    //如果售票成功，增加 ticketsSold 并模拟出票时间。
                    ticketsSold += numTickets;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟出票时间
                } else {
                    //如果售票失败，记录警告日志并退出循环。
                    logger.log(name + " failed to sell tickets. Not enough tickets available.", LogLevel::WARNING);
                    break;
                }
            }
            logger.log(name + " stopped selling tickets");
        });
    }

    void stop() {
        running = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    int getTicketsSold() const {
        return ticketsSold;
    }
};

//定义顾客类
class Customer {
private:
    std::string name;
    TicketSystem &ticketSystem;
    Logger &logger;
    std::thread purchaseThread;
public:
    Customer(const std::string &customerName , TicketSystem &system , Logger &log):
        name(customerName),ticketSystem(system),logger(log) {
        logger.log("Customer " + name + " created");
    }

    //顾客购票方法
    void buyTickets(int num) {
        purchaseThread = std::thread([this,num]() {
            std::stringstream ss;
            ss << name << " is trying to buy " << num << " tickets.";
            logger.log(ss.str());
            if (ticketSystem.sellTicket(num,name)) {
                ss.str("");
                ss << name << " successfully bought " << num << " tickets.";
                logger.log(ss.str());
            } else {
                ss.str("");
                ss << name << " failed to buy " << num << " tickets. Not enough tickets available.";
                logger.log(ss.str(), LogLevel::WARNING);
            }
        });
    }

    //等待购票完成
    void waitForPurchase() {
        if (purchaseThread.joinable()) {
            purchaseThread.join();
        }
    }
};

void test(Logger& logger) {
    TicketSystem ticketSystem(10 , logger);
    std::thread t1([&ticketSystem] {
        ticketSystem.sellTicket(3, "Window 1");
    });
    std::thread t2([&ticketSystem] {
        ticketSystem.sellTicket(5, "Window 2");
    });
    std::thread t3([&ticketSystem] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ticketSystem.addTickets(10);
    });

    t1.join();
    t2.join();
    t3.join();
}

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






