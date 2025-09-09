//
// Created by 杨充 on 2025/9/8.
//

#include "Customer.h"


Customer::Customer(const std::string &customerName, TicketSystem &system, Logger &log)
    : name(customerName) , ticketSystem(system) , logger(log){
    logger.log("Customer " + name + " created");
}


