//
// Created by 杨充 on 2025/9/8.
//

#include "TicketWindow.h"

#include <bits/random.h>

#include "TicketSystem.h"

TicketWindow::TicketWindow(const std::string &windowName, TicketSystem &system, Logger &log)
    : name(windowName), ticketSystem(system), logger(log), running(true), ticketsSold(0) {
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

}

//获取售票数量
int TicketWindow::getTicketsSold() const {

}

//vip售票
void TicketWindow::sellVIP(int num, const std::string &customerName) {

}


