//
// Created by 杨充 on 2025/8/26.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool(size_t maxNumberOfThreads = (std::thread::hardware_concurrency() + 1));

    ThreadPool(const std::string &name, size_t maxNumberOfThreads = (std::thread::hardware_concurrency() + 1));

    template<class F, class... Args>
    auto postTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    template<class F, class... Args>
    auto postPreTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    virtual ~ThreadPool();

    void postPackagedTask(const std::shared_ptr<std::packaged_task<int()> > &task);

    void clearTasks();

    void clearAllTasks();

    void clearPreTasks();

    std::size_t taskSize() { return mTasks.size(); }

    std::size_t preTaskSize() { return mPrepTasks.size(); }

    std::size_t allTaskSize() { return taskSize() + preTaskSize(); }

protected:
    virtual void onThreadAttached() {
    };

    virtual void onThreadDetached() {
    }

private:
    void taskLoop(const std::string &name);

    ThreadPool(const ThreadPool &) = delete;

    ThreadPool(ThreadPool &&) = delete;

    ThreadPool &operator=(const ThreadPool &) = delete;

    ThreadPool &operator=(ThreadPool &&) = delete;

private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> mThreads;
    // the task queue
    std::queue<std::shared_ptr<std::packaged_task<int()> > > mTasks;
    // 优先处理的队列
    std::queue<std::shared_ptr<std::packaged_task<int()> > > mPrepTasks;

    // synchronization
    std::mutex mQueueMutex;
    std::condition_variable mCv;
    volatile bool mStop;
    std::mutex mIdleMutex;
};

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::postTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::shared_ptr<std::packaged_task<return_type()> >(
        new std::packaged_task<return_type()>(std::bind(std::forward<F>(f), std::forward<Args>(args)...)));
    std::future<return_type> res = task->get_future(); {
        std::unique_lock<std::mutex> lock(mQueueMutex);

        // don't allow enqueueing after stopping the pool
        if (mStop) throw std::runtime_error("enqueue on stopped ThreadPool");

        mTasks.push(task);
    }
    mCv.notify_one();

    return res;
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::postPreTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::shared_ptr<std::packaged_task<return_type()> >(
        new std::packaged_task<return_type()>(std::bind(std::forward<F>(f), std::forward<Args>(args)...)));
    std::future<return_type> res = task->get_future(); {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        if (mPrepTasks.size() > 500) {
            std::future<int> empFuture = std::async(std::launch::async, []() -> int { return 0; });
            return empFuture;
        }

        // don't allow enqueueing after stopping the pool
        if (mStop) throw std::runtime_error("enqueue on stopped ThreadPool");

        mPrepTasks.push(task);
    }
    mCv.notify_one();

    return res;
}

// ====================================================
// a task executor
class TaskThread : public ThreadPool {
public:
    TaskThread() : ThreadPool(1) {
    }

    TaskThread(const std::string &name) : ThreadPool(name, 1) {
    }
};

typedef std::shared_ptr<TaskThread> TaskThreadPtr;
// ====================================================


#endif //THREADPOOL_H
