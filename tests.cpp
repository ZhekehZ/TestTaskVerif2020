#include <thread>
#include <cassert>
#include <vector>
#include <iostream>

#include "spinlock.hpp"

void test_single_thread() {
    spinlock sl;
    sl.lock();
    assert(!sl.try_lock());
    sl.unlock();
    assert(sl.try_lock());
}

void test_two_threads() {
    spinlock sl;
    std::string msg;

    auto thread_fun = [&sl, &msg](int n_thread) {
        for (int i = 0; i < 1000000; ++i) {
            sl.lock();
            msg.clear();
            msg = "thread " + std::to_string(n_thread);
            sl.unlock();
        }
    };

    std::thread t1{thread_fun, 1}, t2{thread_fun, 2};
    t1.join();
    t2.join();

    assert(msg == "thread 1" || msg == "thread 2");
}

void test_many_threads() {
    spinlock sl1, sl2;
    int counter1 = 0, counter2 = 0;

    const int thread_n = 20;
    const int repeat_n = 100000;

    auto thread_func = [&]() {
        for (int i = 0; i < repeat_n; ++i) {
            if (sl1.try_lock()) {
                ++counter1;
                sl1.unlock();
            } else {
                sl2.lock();
                ++counter2;
                sl2.unlock();
            }
        }
    };

    std::vector<std::thread> threads(thread_n);
    for (auto &thread : threads) {
        thread = std::thread{thread_func};
    }
    for (auto &thread : threads) {
        thread.join();
    }

    assert(counter1 + counter2 == thread_n * repeat_n);
}

int main() {
    test_single_thread();
    test_two_threads();
    test_many_threads();

    return 0;
}