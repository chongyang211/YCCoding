//
// Created by 杨充 on 2025/9/8.
//

#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
#include "Logger.h"
#include "TicketSystem.h"
#include "TicketWindow.h"


class Customer {
private:
    std::string name;
    TicketSystem& ticketSystem;
    Logger& logger;
public:
    Customer(const std::string& customerName, TicketSystem& system, Logger& log);
    void buyTickets(int num);
    void buyVIP(int num, TicketWindow& vipWindow);
};



#endif //CUSTOMER_H
