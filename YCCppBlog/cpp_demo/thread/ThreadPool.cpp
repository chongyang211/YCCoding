//
// Created by 杨充 on 2025/8/26.
//

#include "ThreadPool.h"
#include <algorithm>
#include <iostream>

// the constructor just launches some amount of workers
// 作用：初始化线程池，并指定线程池的名称和最大线程数。
// 原理：emplace_back 直接构造线程对象，避免了额外的拷贝操作。taskLoop 是线程的主循环函数，负责从任务队列中取出任务并执行。

ThreadPool::ThreadPool(const std::string &name, size_t maxNumberOfThreads) : mStop(false) {
    // 初始化 mStop 为 false，表示线程池处于运行状态。
    for (size_t i = 0; i < maxNumberOfThreads; ++i) {
        // 使用 emplace_back 创建 maxNumberOfThreads 个线程，每个线程执行 taskLoop 函数。
        // 将线程池的名称传递给 taskLoop，用于设置线程名称（在支持的系统上）。
        mThreads.emplace_back(&ThreadPool::taskLoop, this, name);
    }
}

// 作用：初始化线程池，使用默认名称 "yt_thread_pool"。
// 功能：与带名称的构造函数类似，只是线程名称固定为 "yt_thread_pool"。
ThreadPool::ThreadPool(size_t maxNumberOfThreads) : mStop(false) {
    for (size_t i = 0; i < maxNumberOfThreads; ++i) {
        mThreads.emplace_back(&ThreadPool::taskLoop, this, "yt_thread_pool");
    }
}

// 作用：销毁线程池，确保所有线程安全退出。
// 原理：mStop 是一个标志位，用于通知线程退出主循环。mCv.notify_all() 确保所有线程都能检测到 mStop 的变化。join 确保线程池销毁前所有线程都已完成任务。
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        // 设置 mStop 为 true，通知所有线程停止运行。
        mStop = true;
    }
    // 使用 mCv.notify_all() 唤醒所有等待的线程。
    mCv.notify_all();
    // 使用 std::for_each 和 std::thread::join 等待所有线程执行完毕。
    // std::for_each(mThreads.begin(), mThreads.end(), std::bind(&std::thread::join, std::placeholders::_1));
    // 等待所有线程结束
    for (auto &thread : mThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

// 清理普通任务
void ThreadPool::clearTasks() {
    // 作用：清空普通任务队列 mTasks。
    // 功能：使用互斥锁保护队列，逐个移除任务。
    std::unique_lock<std::mutex> lock(this->mQueueMutex);
    while (!this->mTasks.empty()) {
        mTasks.pop();
    }
}

// 清理优先任务
// 作用：清空优先任务队列 mPrepTasks。
// 功能：与 clearTasks 类似，但操作的是优先任务队列。
void ThreadPool::clearPreTasks() {
    std::unique_lock<std::mutex> lock(this->mQueueMutex);
    while (!this->mPrepTasks.empty()) {
        mPrepTasks.pop();
    }
}

// 作用：清空所有任务队列（包括普通任务和优先任务）。
// 功能：依次调用 clearTasks 和 clearPreTasks。
void ThreadPool::clearAllTasks() {
    clearTasks();
    clearPreTasks();
}

// 作用：线程的主循环函数，负责从任务队列中取出任务并执行。
// 原理：mCv.wait 使用条件变量实现线程的等待和唤醒。优先任务队列的任务优先执行，确保高优先级任务及时处理。
void ThreadPool::taskLoop(const std::string &name) {
    // 调用 onThreadAttached()，通知线程已启动。
    onThreadAttached();
    while (true) {
        std::shared_ptr<std::packaged_task<int()> > preTask;
        std::shared_ptr<std::packaged_task<int()> > task;
        {
            // 等待条件变量：停止信号或任务可用
            std::unique_lock<std::mutex> lock(this->mQueueMutex);
            //使用 mCv.wait 等待任务队列非空或线程池停止。
            mCv.wait(lock,[this] {
                return this->mStop || !this->mTasks.empty() || !this->mPrepTasks.empty();
            });
            // 检查停止信号
            if (this->mStop) {
                // 如果 mStop 为 true，调用 onThreadDetached() 并退出循环。
                onThreadDetached();
                return;
            }
            // 优先执行优先任务队列 mPrepTasks 中的任务。
            if (!this->mPrepTasks.empty()) {
                preTask = this->mPrepTasks.front();
                this->mPrepTasks.pop();
            }
            //执行普通任务队列 mTasks 中的任务。
            if (!this->mTasks.empty()) {
                task = this->mTasks.front();
                this->mTasks.pop();
            }
        }
        // 执行任务（在锁外执行，避免阻塞其他线程）
        if (preTask) {
            //std::cout << "preTask begin" << std::endl;
            (*preTask)();
            //std::cout << "preTask end" << std::endl;
        }
        if (task) {
            //std::cout << "task begin" << std::endl;
            (*task)();
            //std::cout << "task end" << std::endl;
        }
    }
}

// 作用：提交一个打包任务到普通任务队列。
// 原理：std::packaged_task 用于封装任务，支持异步获取任务结果。mCv.notify_one() 确保有线程及时处理新任务。
void ThreadPool::postPackagedTask(const std::shared_ptr<std::packaged_task<int()> > &task) {
    {
        // 使用互斥锁保护任务队列。
        std::unique_lock<std::mutex> lock(mQueueMutex);
        // 如果线程池已停止，抛出异常。
        // don't allow enqueueing after stopping the pool
        if (mStop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        // 将任务添加到普通任务队列 mTasks。
        mTasks.push(task);
    }
    // 使用 mCv.notify_one() 唤醒一个等待的线程。
    mCv.notify_one();
}

std::size_t ThreadPool::taskSize() {
    std::unique_lock<std::mutex> lock(mQueueMutex);
    return mTasks.size();
}

std::size_t ThreadPool::preTaskSize() {
    std::unique_lock<std::mutex> lock(mQueueMutex);
    return mPrepTasks.size();
}

std::size_t ThreadPool::allTaskSize() {
    return taskSize() + preTaskSize();
}


