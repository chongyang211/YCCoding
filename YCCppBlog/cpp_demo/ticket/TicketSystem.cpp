//
// Created by 杨充 on 2025/9/8.
//

#include "TicketSystem.h"
#include <sstream>


TicketSystem::TicketSystem(int total, Logger &log) : totalTicket(total), remainingTickets(total), logger(log) {
    logger.log("Ticket system created with " + std::to_string(total) + " tickets");
}

// 售票功能 (sellTicket)
// 线程安全：使用 std::mutex 和 std::unique_lock 确保对 remainingTickets 的访问是线程安全的。
// 条件变量：使用 std::condition_variable 实现等待机制，避免忙等待。当剩余票数不足时，线程会等待，直到票数增加或被唤醒。
// 日志记录：通过 Logger 记录售票成功或失败的日志。
bool TicketSystem::sellTicket(int num, const std::string &windowName) {
    //使用 std::unique_lock 锁定互斥锁，确保对共享资源 remainingTickets 的访问是线程安全的。
    std::unique_lock<std::mutex> lock(ticketMutex);
    //这个是一个自动锁，作用域结束即锁释放
    //使用条件变量 cv 等待，直到满足条件 remainingTickets >= num 或 remainingTickets == 0。
    //这样可以避免忙等待，提高效率。
    cv.wait(lock,[this,num] {
        //如果没票了
        return remainingTickets >= num || remainingTickets == 0;
    });
    if (remainingTickets >= num) {
        //如果 remainingTickets >= num，则减少剩余票数，记录日志，并通过 cv.notify_all() 通知其他等待的线程。
        remainingTickets -= num;    //售票操作
        //记录日志
        std::stringstream ss;
        ss << windowName << " sold " << num << " tickets. Remaining tickets: " << remainingTickets;
        logger.log(ss.str());
        //刷新所有的线程，通知其他线程等待
        cv.notify_all();
        //卖出票了
        return true;
    } else {
        //如果 remainingTickets < num，则返回 false，表示售票失败。
        std::stringstream ss;
        ss << windowName << " failed to sell " << num << " tickets , Not enough tickets available.";
        //这个日志要打印警告日志
        logger.log(ss.str(), LogLevel::WARNING);
        return false;
    }
}
