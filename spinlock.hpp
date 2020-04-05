#pragma once

#include <atomic>

class spinlock {
public:
    spinlock() noexcept : locked(false) {};
    spinlock(const spinlock& other) = delete;

    void lock() {
        while (locked.exchange(true));
    }

    void unlock() {
        locked.exchange(false);
    }

    bool try_lock() {
        return !locked.exchange(true);
    }

private:
    std::atomic_bool locked;
};