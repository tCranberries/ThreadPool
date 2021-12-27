//
// Created by wzw on 2021/12/24.
//

#include "test.h"
#include <mutex>

using namespace std;

int g = 0;
mutex gMutex;

void func() {
    lock_guard<mutex> guard(gMutex);
    g++;

    this_thread::sleep_for(chrono::seconds(1));
}

int main_test() {
    thread t1(func);
    thread t2 (func);

    t1.join();
    t2.join();

    cout << "g: " << g << endl;
    return 0;
}