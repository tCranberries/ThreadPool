//
// Created by wzw on 2021/12/24.
//

#ifndef THREAD_POOL_TEST_H
#define THREAD_POOL_TEST_H

#include <iostream>
#include <thread>

using namespace std;

class Task {
public:
    Task(int i, char* s) : id(i), name(s) {};
    ~Task() = default;;

    void operator()() {
        this_thread::sleep_for(chrono::seconds(3));
        cout << "pid: " << this_thread::get_id() << "  id: " << id << "  name: " << name << endl;
    }
private:
    int id;
    char* name;
};


#endif //THREAD_POOL_TEST_H
