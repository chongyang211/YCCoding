//
// Created by 杨充 on 2025/8/10.
//
#include <future>
#include <iostream>
#include <thread>
using namespace std;


//13.6.2 std::async
void delayedTask() {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << "Task executed after 3000ms." << std::endl;
}

void asyncTest() {
    std::cout << "Task scheduled." << std::endl;
    // 使用 std::async 异步执行任务
    auto future = std::async(std::launch::async, delayedTask);
    // 主线程继续执行其他任务
    std::cout << "Main thread is doing other work..." << std::endl;
    // 等待异步任务完成
    future.wait();
    std::cout << "Main thread finished." << std::endl;
}

//使用 `std::thread` 和 `std::this_thread::sleep_for`。
void thisThreadSleep() {
    std::cout << "Task scheduled." << std::endl;
    // 创建线程执行任务
    std::thread t(delayedTask);
    // 主线程继续执行其他任务
    std::cout << "Main thread is doing other work..." << std::endl;
    // 等待线程完成
    t.join();
    std::cout << "Main thread finished." << std::endl;
}

//g++ -std=c++11
int main() {
    // asyncTest();
    thisThreadSleep();
    return 0;
}

