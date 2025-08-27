//
// Created by 杨充 on 2025/8/26.
//


#include <iostream>
#include <memory>
#include "ThreadPool.h"

int test1(std::string name, int age) {
    std::cout << "打印1：" << name << " , age " << age << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "打印1：等待了100ms" << std::endl;
    return 1;
}

int test2(std::string name, int age) {
    std::cout << "打印2：" << name << " , age " << age << std::endl;
    return 2;
}

//g++ -std=c++11 ThreadPoolTest.cpp ThreadPool.cpp
int main() {
    TaskThreadPtr m_callback_thread;
    m_callback_thread = std::make_shared<TaskThread>();
    auto future1 = m_callback_thread->postTask(&test1, "yangchong", 30);
    auto future2 = m_callback_thread->postTask(&test2, "tangxinyi", 29);
    // 提交普通任务
    auto future3 = m_callback_thread->postTask([]() {
        std::cout << "打印3：提交普通任务" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "打印3：提交普通任务执行结束" << std::endl;
        return 42;
    });
    // 提交优先任务
    auto future4 = m_callback_thread->postPreTask([]() {
        std::cout << "打印4：提交优先任务" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "打印4：提交优先任务执行结束" << std::endl;
        return 100;
    });
    // 获取任务结果
    std::cout << "Result 1: " << future1.get() << std::endl;
    std::cout << "Result 2: " << future2.get() << std::endl;
    std::cout << "Result 3: " << future3.get() << std::endl;
    std::cout << "Result 4: " << future4.get() << std::endl;
    return 0;
}


// Result 1: 打印4：提交优先任务
// 打印4：提交优先任务执行结束
// 打印1：yangchong , age 30
// 打印1：等待了100ms
// 打印2：tangxinyi , age 29
// 打印3：提交普通任务
// 1
// Result 2: 2
// Result 3: 打印3：提交普通任务执行结束
// 42
// Result 4: 100
