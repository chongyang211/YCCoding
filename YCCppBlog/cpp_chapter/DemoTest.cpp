//
// Created by 杨充 on 2025/8/7.
//

#include "DemoTest.h"

#include <future>

void DemoTest::_DelayedTask() {
    std::cout << ">>> _DelayedTask <<<" << std::endl;
    // 模拟任务准备（可以是复杂计算或IO操作）
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << ">>> 任务准备完成 <<<" << std::endl;
        // 如果 ready 提前变为 true（即任务准备完成），则立即唤醒
        isHalReady = true;
    }
    halCv.notify_one();  // 通知等待中的线程
}

int DemoTest::_InitAndApp() {
    // 启动延迟任务的线程
    std::thread worker(&DemoTest::_DelayedTask, this); {
        std::unique_lock<std::mutex> lock(mtx);
        // 主线程继续执行其他工作
        std::cout << "主线程执行其他任务..." << std::endl;
        // 等待 2 秒或被提前唤醒
        bool isFinish = halCv.wait_for(lock, std::chrono::seconds(4), [this]() -> bool { return isHalReady; });
        if (isFinish) {
            std::cout << "★ 条件达成 - 提前执行任务" << std::endl;
        } else {
            std::cout << "⏰ 等待超时 - 强制唤醒执行" << std::endl;
        }
    }
    worker.join();  // 确保任务线程结束
    // 任务执行
    std::cout << "=== 执行核心任务 ===" << std::endl;
    return 0;  // 返回一个整数值
}



void DemoTest::_DelayedTask2() {
    std::cout << "后台任务开始运行..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));{
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << ">>> 后台任务准备完成 <<<" << std::endl;
        isHalReady = true;
    }
    halCv.notify_one();
    std::cout << "后台任务完成通知已发送" << std::endl;
}

int DemoTest::_InitAndApp2() {
    std::cout << "★ 主线程开始执行 ★" << std::endl;
    // 启动后台准备任务
    std::thread worker(&DemoTest::_DelayedTask2, this);
    worker.detach();  // 分离线程使其独立运行
    std::cout << "主线程开始执行任务" << std::endl;
    // 创建任务执行10秒耗时操作。模拟初始化工作
    std::this_thread::sleep_for(std::chrono::seconds(5));
    // 主线程继续处理其他任务
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "主线程执行其他非耗时任务..." << std::endl;
        // 等待后台任务完成（最多等2秒）
        bool isFinish = halCv.wait_for(lock, std::chrono::seconds(2),
            [this]() { return isHalReady; });
        if (isFinish) {
            std::cout << "★ 后台任务提前完成 - 准备核心任务" << std::endl;
        } else {
            std::cout << "⏰ 后台任务等待超时 - 继续核心任务" << std::endl;
            // 启动异步任务
            auto future = std::async(std::launch::async, []{
                std::this_thread::sleep_for(std::chrono::seconds(3));
                std::cout << "启动异步等待3秒执行任务" << std::endl;
            });
        }
    }
    // 执行核心任务
    std::cout << "=== 执行核心任务 ===" << std::endl;

    std::cout << "★ 主线程结束 ★" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;  // 返回一个整数值
}



