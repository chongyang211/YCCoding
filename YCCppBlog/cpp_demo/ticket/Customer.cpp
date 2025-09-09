//
// Created by 杨充 on 2025/9/8.
//

#include "Customer.h"
#include <sstream>


Customer::Customer(const std::string &customerName, TicketSystem &system, Logger &log)
    : name(customerName) , ticketSystem(system) , logger(log){
    logger.log("Customer " + name + " created");
}

//顾客购票方法
void Customer::buyTickets(int num) {
    purchaseThread = std::thread([this,num]() {
        std::stringstream ss;
        ss << name << " is trying to buy " << num << " tickets.";
        logger.log(ss.str());
        if (ticketSystem.sellTicket(num,name)) {
            //用户买票成功
            ss.str("");
            ss << name << " successfully bought " << num << " tickets.";
            logger.log(ss.str());
        } else {
            //用户买票失败
            ss.str("");
            ss << name << " failed to buy " << num << " tickets. Not enough tickets available.";
            logger.log(ss.str(), LogLevel::WARNING);
        }
    });
    if (purchaseThread.joinable()) {
        //开始线程
        purchaseThread.join();
    }
}

// void Customer::buyVIP(int num, TicketWindow &vipWindow) {
//     vipWindow.sellVIP(num,name);
// }



