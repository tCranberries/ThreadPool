//
// Created by wzw on 2021/12/27.
//

#include "ThreadSafeStack.h"
#include <functional>

using namespace std;

int main() {

    ThreadSafeStack<int> stack{};
    stack.push(1);

    function<void(int)> func = [&](int timeout) {
        try {
            if (!stack.empty()) {
                this_thread::sleep_for(chrono::milliseconds(timeout));
                auto tmp = stack.pop();
                cout << this_thread::get_id() << " pop: " << *tmp << endl;
            }
        } catch (empty_stack& e) {
            e.what();
        }
    };

    thread t1(func, 2);
    thread t2(func, 3);

    t1.join();
    t2.join();

    return 0;
}