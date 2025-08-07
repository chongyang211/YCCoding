//
// Created by 杨充 on 2025/8/7.
//

#ifndef DEMOTEST_H
#define DEMOTEST_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class DemoTest {
public:
    void _DelayedTask();  // 延迟任务函数
    int _InitAndApp();           // 初始化并执行任务的函数
private:
    std::mutex mtx;              // 互斥锁
    std::condition_variable halCv; // 条件变量
    bool isHalReady = false;     // 任务是否准备完成的标志
};



#endif //DEMOTEST_H
