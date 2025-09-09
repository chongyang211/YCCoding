//
// Created by 杨充 on 2025/9/8.
//

#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
#include <thread>
#include "Logger.h"
#include "TicketSystem.h"
// #include "TicketWindow.h"

class Customer {
private:
    std::string name;           //用户名
    TicketSystem& ticketSystem;     //购票系统
    Logger& logger;         //日志记录
    std::thread purchaseThread;     //线程
public:
    Customer(const std::string& customerName, TicketSystem& system, Logger& log);
    void buyTickets(int num);
    // void buyVIP(int num, TicketWindow& vipWindow);
};



#endif //CUSTOMER_H
