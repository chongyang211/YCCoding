//
// Created by 杨充 on 2025/8/26.
//

#include "ThreadPool.h"
#include <algorithm>
#include <iostream>

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(const std::string &name, size_t maxNumberOfThreads) : mStop(false) {
    for (size_t i = 0; i < maxNumberOfThreads; ++i) {
        mThreads.emplace_back(&ThreadPool::taskLoop, this, name);
    }
}

ThreadPool::ThreadPool(size_t maxNumberOfThreads) : mStop(false) {
    for (size_t i = 0; i < maxNumberOfThreads; ++i) {
        mThreads.emplace_back(&ThreadPool::taskLoop, this, "yt_thread_pool");
    }
}

// the destructor joins all threads
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mStop = true;
    }
    mCv.notify_all();
    std::for_each(mThreads.begin(), mThreads.end(), std::bind(&std::thread::join, std::placeholders::_1));
}

void ThreadPool::clearTasks() {
    std::unique_lock<std::mutex> lock(this->mQueueMutex);
    while (!this->mTasks.empty()) {
        mTasks.pop();
    }
}

void ThreadPool::clearPreTasks() {
    std::unique_lock<std::mutex> lock(this->mQueueMutex);
    while (!this->mPrepTasks.empty()) {
        mPrepTasks.pop();
    }
}

void ThreadPool::clearAllTasks() {
    clearTasks();
    clearPreTasks();
}

void ThreadPool::taskLoop(const std::string &name) {
    onThreadAttached();
    for (;;) {
        std::shared_ptr<std::packaged_task<int()> > preTask;
        std::shared_ptr<std::packaged_task<int()> > task; {
            std::unique_lock<std::mutex> lock(this->mQueueMutex);
            this->mCv.wait(lock, [this] { return this->mStop || !this->mTasks.empty() || !this->mPrepTasks.empty(); });
            if (this->mStop) {
                onThreadDetached();
                return;
            }

            if (!this->mPrepTasks.empty()) {
                preTask = this->mPrepTasks.front();
                this->mPrepTasks.pop();
            }

            if (!this->mTasks.empty()) {
                task = this->mTasks.front();
                this->mTasks.pop();
            }
        }

        if (preTask) {
            std::cout << "preTask begin" << std::endl;
            (*preTask)();
            std::cout << "preTask end" << std::endl;
        }

        if (task) {
            std::cout << "task begin" << std::endl;
            (*task)();
            std::cout << "task end" << std::endl;
        }
    }
}

void ThreadPool::postPackagedTask(const std::shared_ptr<std::packaged_task<int()> > &task) { {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        // don't allow enqueueing after stopping the pool
        if (mStop) throw std::runtime_error("enqueue on stopped ThreadPool");

        mTasks.push(task);
    }
    mCv.notify_one();
}
