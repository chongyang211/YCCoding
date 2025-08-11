//
// Created by 杨充 on 2025/8/7.
//
#include <future>
#include <iostream>
#include <thread>
using namespace std;


void threadFunction() {
    std::cout << "Hello from thread!" << std::endl;
}

void startThread1() {
    std::thread t(threadFunction); // 创建线程
    t.join();// 等待线程结束
    std::cout << "Main thread finished." << std::endl;
    //打印结果
    // Hello from thread!
    // Main thread finished.
}

void printMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void startThread2() {
    std::thread t(printMessage,"yang chong");
    t.join();
}

void detachThread1() {
    // 创建线程
    std::thread t(printMessage, "Hello, World!");
    std::cout << "Main thread is detaching the worker thread..." << std::endl;
    // 分离线程
    t.detach();
    std::cout << "Worker thread has been detached. Main thread continues execution." << std::endl;
    // 主线程继续执行
    std::this_thread::sleep_for(std::chrono::seconds(3)); // 等待足够时间以确保分离线程完成
}

int main() {
    // startThread1();
    // startThread2();
    //detachThread1();

    return 0;
}

