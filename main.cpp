//
// Created by wzw on 2021/12/23.
//

#include "_ThreadPool.h"

using namespace std;

void multiply(const int a, const int b) {
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
}

void multiplyRef(int& t, const int a, const int b) {
    t = a * b;
    std::cout << a << " * " << b << " = " << t << std::endl;
}

int main_main() {

    ThreadPool pool(4);
    pool.init();

//    cout << "main: " << this_thread::get_id() << endl;
//    int a = 2;
//    int b = 3;
//    int res;
//    // 引用的话必须加入 ref() 进行包装
//    auto future = pool.summit(multiplyRef, ref(res), a, b);
//
//    future.get();
//    cout << "main res: " << res << endl;

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            pool.summit(multiply, i ,j);
        }
    }

    pool.shutDown();

    return 0;
}