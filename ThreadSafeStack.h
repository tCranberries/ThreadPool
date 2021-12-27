//
// Created by wzw on 2021/12/27.
//

#ifndef THREADPOOL_THREADSAFESTACK_H
#define THREADPOOL_THREADSAFESTACK_H

#include <iostream>
#include <mutex>
#include <thread>
#include <stack>
#include <exception>

struct empty_stack : std::exception {
    const char* what() const throw() override {
        return "empty stack!";
    };
};

template<class T>
class ThreadSafeStack {
public:
    ThreadSafeStack() = default;
    ThreadSafeStack(const ThreadSafeStack<T>& threadSafeStack) {
        std::lock_guard<std::mutex> guard(threadSafeStack.mutex);
        stack = threadSafeStack.stack;
    }

    ThreadSafeStack& operator=(const ThreadSafeStack<T>& threadSafeStack) = delete;

    void pop(T& value) {
        std::lock_guard<std::mutex> guard(mutex);
        if (stack.empty()) {
            throw empty_stack();
        }
        value = stack.top();
        stack.pop();
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> guard(mutex);
        if (stack.empty()) {
            throw empty_stack();
        }
        std::shared_ptr<T> const res(std::make_shared<T>(stack.top()));
        stack.pop();
        return res;
    }

    void push(T value) {
        std::lock_guard<std::mutex> guard(mutex);
        stack.push(value);
    }

    bool empty() {
        std::lock_guard<std::mutex> guard(mutex);
        return stack.empty();
    }

    size_t size() {
        std::lock_guard<std::mutex> guard(mutex);
        return stack.size();
    }

private:
    std::mutex mutex;
    std::stack<T> stack;
};


#endif //THREADPOOL_THREADSAFESTACK_H
