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
    // 初始化线程池，默认线程数为 CPU 核心数加 1。
    ThreadPool(size_t maxNumberOfThreads = (std::thread::hardware_concurrency() + 1));
    // 带名称的构造函数。
    ThreadPool(const std::string &name, size_t maxNumberOfThreads = (std::thread::hardware_concurrency() + 1));
    // 提交普通任务。
    template<class F, class... Args>
    auto postTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    // 提交优先任务。
    template<class F, class... Args>
    auto postPreTask(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    // 析构函数
    virtual ~ThreadPool();
    void postPackagedTask(const std::shared_ptr<std::packaged_task<int()> > &task);
    // 清空普通任务队列。
    void clearTasks();
    // 清空所有任务队列。
    void clearAllTasks();
    // 清空优先任务队列。
    void clearPreTasks();
    // 返回普通任务队列的大小。
    std::size_t taskSize() { return mTasks.size(); }
    // 返回优先任务队列的大小。
    std::size_t preTaskSize() { return mPrepTasks.size(); }
    // 返回所有任务队列的大小。
    std::size_t allTaskSize() { return taskSize() + preTaskSize(); }
protected:
    // 线程启动时的回调（空实现）。
    virtual void onThreadAttached() {};
    // 线程退出时的回调（空实现）。
    virtual void onThreadDetached() {}
private:
    void taskLoop(const std::string &name);

    ThreadPool(const ThreadPool &) = delete;

    ThreadPool(ThreadPool &&) = delete;

    ThreadPool &operator=(const ThreadPool &) = delete;

    ThreadPool &operator=(ThreadPool &&) = delete;

private:
    // need to keep track of threads so we can join them
    // 存储线程池中的线程。
    std::vector<std::thread> mThreads;
    // 普通任务队列。
    std::queue<std::shared_ptr<std::packaged_task<int()> > > mTasks;
    // 优先处理的队列
    std::queue<std::shared_ptr<std::packaged_task<int()> > > mPrepTasks;

    // 保护任务队列的互斥锁。
    std::mutex mQueueMutex;
    // 条件变量，用于线程间的同步。
    std::condition_variable mCv;
    // 标志位，指示线程池是否停止。
    volatile bool mStop;
    // 空闲线程的互斥锁（未在代码中使用）。
    std::mutex mIdleMutex;
};

// 功能：提交普通任务。
// 将任务封装为 std::packaged_task，并推送到普通任务队列。返回一个 std::future，用于获取任务的执行结果。
// 使用模板和可变参数模板，支持任意类型的任务。返回一个 std::future，用于获取任务的执行结果。
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
    TaskThread() : ThreadPool(1) {}
    TaskThread(const std::string &name) : ThreadPool(name, 1) {}
};

typedef std::shared_ptr<TaskThread> TaskThreadPtr;
// ====================================================


#endif //THREADPOOL_H
