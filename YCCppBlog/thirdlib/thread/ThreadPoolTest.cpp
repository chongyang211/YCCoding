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
}

int test2(std::string name, int age) {
    std::cout << "打印2：" << name << " , age " << age << std::endl;
}

//g++ -std=c++11 ThreadPoolTest.cpp
int main() {
    TaskThreadPtr m_callback_thread;
    m_callback_thread = std::make_shared<TaskThread>();
    m_callback_thread->postTask(&test1, "yangchong", 30);
    m_callback_thread->postTask(&test2, "tangxinyi", 29);
    // 提交普通任务
    auto future1 = m_callback_thread->postTask([]() {
        std::cout << "打印3：提交普通任务" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "打印3：提交普通任务执行结束" << std::endl;
        return 42;
    });
    // 提交优先任务
    auto future2 = m_callback_thread->postPreTask([]() {
        std::cout << "打印4：提交优先任务" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "打印4：提交优先任务执行结束" << std::endl;
        return 100;
    });
    // 获取任务结果
    std::cout << "Result 3: " << future1.get() << std::endl;
    std::cout << "Result 4: " << future2.get() << std::endl;
    return 0;
}
