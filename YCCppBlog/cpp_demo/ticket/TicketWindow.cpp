//
// Created by 杨充 on 2025/9/8.

#include <sstream>
#include <random>

#include "TicketWindow.h"
#include "TicketSystem.h"


TicketWindow::TicketWindow(const std::string &windowName, TicketSystem &system, Logger &log)
    : name(windowName), ticketSystem(system), logger(log), running(true), ticketsSold(0) {
    //ticketSystem(system)一直报错提示
    //Attempt to use deleted constructor TicketSystem::TicketSystem(const TicketSystem &)
    // 隐式拷贝：
    // 尝试将 TicketSystem 对象作为值传递给函数或从函数返回时，会触发拷贝构造函数。
    // 尝试将 TicketSystem 对象赋值给另一个对象时，会触发拷贝赋值运算符。

    // 显式删除拷贝构造函数：
    // 如果 TicketSystem 类中显式删除了拷贝构造函数（例如 TicketSystem(const TicketSystem&) = delete;），则任何尝试拷贝 TicketSystem 对象的行为都会导致编译错误。

    //这里 ticketSystem(system) 试图将传入的 TicketSystem& system 赋值给 ticketSystem 引用成员变量。
    //然而，如果 TicketSystem 类的拷贝构造函数被删除，编译器会报错。
}

//开始窗口售票
void TicketWindow::start() {
    std::thread([this]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 5);
        //开启一个循环
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

//停止窗口售票
void TicketWindow::stop() {
    running = false;
}

//获取售票数量
int TicketWindow::getTicketsSold() const {
    return ticketsSold;
}

//vip售票
void TicketWindow::sellVIP(int num, const std::string &customerName) {
    std::lock_guard<std::mutex> lock(vipMutex);
    if (ticketSystem.sellTicket(num, name + "(VIP)")) {
        ticketsSold += num;
        std::stringstream ss;
        ss << "VIP customer " << customerName << " bought " << num
           << " tickets from " << name;
        logger.log(ss.str());
    }
}


