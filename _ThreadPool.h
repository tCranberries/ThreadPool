//
// Created by wzw on 2021/12/23.
//

#ifndef THREAD_POOL__THREAD_POOL_H
#define THREAD_POOL__THREAD_POOL_H

#include <iostream>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <stdexcept>

using namespace std;

class ThreadPool {
public:
    explicit ThreadPool(int num) : numThreads(num), threads(vector<thread>(num)), isShutDown(false) {};

    class Worker {
    private:
        ThreadPool* pool;
    public:
        explicit Worker(ThreadPool* ptr) : pool(ptr) {};

        void operator()() {
            function<void()> func;
            while (true) {
                {
                    unique_lock<mutex> locker(pool->queueLock);
                    while (!pool->isShutDown && pool->tasks.empty()) {
                        pool->conditionLock.wait(locker);
                    }
                    if (pool->isShutDown && pool->tasks.empty()) {
                        break;
                    }
                    func = move(pool->tasks.front());
                    pool->tasks.pop();
                }
                func();
            }
        }
    };


    void init() {
        for (int i = 0; i < numThreads; i++) {
            threads[i] = thread(Worker(this));
        }
    }

    void shutDown() {
        {
            unique_lock<mutex> lock(queueLock);
            isShutDown = true;
        }
        conditionLock.notify_all();

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        threads.clear();
    }


    template<typename F, typename ...Args>
    auto summit(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
        using returnType = typename result_of<F(Args...)>::type;
        auto task = make_shared<packaged_task<returnType()>>(bind(forward<F>(f), forward<Args>(args)...));
        future<returnType> res = task->get_future();
        function<void()> wrapperFunc = [task]() { (*task)(); };
        {
            unique_lock<mutex> lock(queueLock);
            if (isShutDown) {
                throw runtime_error("enqueue on stopped thread pool");
            }
            tasks.push(wrapperFunc);
        }
        conditionLock.notify_one();

        return res;
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

private:

    bool isShutDown;
    int numThreads;
    vector<thread> threads;
    queue<function<void()>> tasks;

    mutex queueLock;
    condition_variable conditionLock;

};


#endif //THREAD_POOL__THREAD_POOL_H
