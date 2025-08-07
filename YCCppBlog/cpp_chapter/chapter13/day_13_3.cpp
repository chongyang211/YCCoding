//
// Created by 杨充 on 2025/8/7.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;


std::mutex mtx;
std::condition_variable halCv;
bool isHalReady = false;

void delayed_task() {
    // 模拟任务准备（可以是复杂计算或IO操作）
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << ">>> 任务准备完成 <<<" << std::endl;
        //如果 ready提前变为 true（即任务准备完成），则立即唤醒
        isHalReady = true;
    }
    halCv.notify_one();  // 通知等待中的线程
}

//在 C++ 中使用 std::condition_variable实现延迟执行任务的功能，可以通过以下方式实现：开启任务后让线程等待 2 秒再唤醒执行，同时主线程可以继续其他操作。
//g++ -std=c++11 day_13_3.cpp
int main() {
    // 启动延迟任务的线程
    std::thread worker(delayed_task); {
        std::unique_lock<std::mutex> lock(mtx);
        // 主线程继续执行其他工作
        std::cout << "主线程执行其他任务..." << std::endl;
        // 等待 2 秒或被提前唤醒
        // cv.wait_for(lock, std::chrono::seconds(2), []{ return ready; });
        // bool isFinish = cv.wait_for(lock, std::chrono::seconds(2), [](){ return ready; });
        bool isFinish = halCv.wait_for(lock, std::chrono::seconds(2), []() -> bool { return isHalReady; });
        if (isFinish) {
            std::cout << "★ 条件达成 - 提前执行任务" << std::endl;
        } else {
            std::cout << "⏰ 等待超时 - 强制唤醒执行" << std::endl;
        }
        std::cout << "=== 继续任务 ===" << std::endl;
    }
    worker.join();  // 确保任务线程结束
    // 任务执行
    std::cout << "=== 执行核心任务 ===" << std::endl;
    return 0;
}