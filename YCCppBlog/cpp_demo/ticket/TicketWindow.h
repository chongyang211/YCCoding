//
// Created by 杨充 on 2025/9/8.
//

#ifndef TICKETWINDOW_H
#define TICKETWINDOW_H
#include <string>
#include <thread>

#include "TicketSystem.h"

//创建票务系统
class TicketWindow {
private:
    std::string name;   //窗口名称
    TicketSystem ticketSystem;    //票务系统
    Logger &logger;     //日志系统
    std::atomic<bool> running;  //是否在工作中
    std::atomic<int> ticketsSold;   //记录售票数量
    std::thread workerThread;   //工作线程
    std::mutex vipMutex;
public:
    TicketWindow(const std::string& windowName, TicketSystem& system, Logger& log);
    // TicketWindow(const TicketWindow&);
    // TicketWindow& operator=(const TicketWindow&);
    TicketWindow(const TicketWindow&) = delete;
    TicketWindow& operator=(const TicketWindow&) = delete;
    //开始窗口售票
    void start();
    //停止窗口售票
    void stop();
    //获取售票数量
    int getTicketsSold() const;
    //vip售票
    void sellVIP(int num, const std::string& customerName);
};

#endif //TICKETWINDOW_H
