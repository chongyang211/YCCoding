//
// Created by 杨充 on 2025/9/8.
//

#ifndef TICKETSYSTEM_H
#define TICKETSYSTEM_H
#include <atomic>
#include <mutex>

#include "Logger.h"


class TicketSystem {
private:
    int totalTicket;    //总票数
    std::atomic<int> remainingTickets;      //正在出的票
    std::mutex ticketMutex;     //票务锁，主要保证线程安全
    std::condition_variable cv;     //条件变量，主要是用于线程通信，可以结合锁一起使用
    Logger &logger;     //日志类
public:
    TicketSystem(int total, Logger& log);
    //售票功能 (sellTicket)
    bool sellTicket(int num, const std::string& windowName);
    //添加票功能 (addTickets)
    void addTickets(int num);
    int getRemainingTickets() const;
    int getTotalTickets() const;
    //动态票务监控 (monitorTickets)
    void monitorTickets();
    //恢复票功能 (recoverTickets)
    void recoverTickets(int num);

};



#endif //TICKETSYSTEM_H
