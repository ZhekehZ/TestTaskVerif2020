#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// SPINLOCK
typedef atomic_flag spinlock;

void unlock(spinlock* sl) {
    atomic_flag_clear(sl);
}

void lock(spinlock* sl) {
    while (atomic_flag_test_and_set(sl));
}

bool try_lock(spinlock* sl) {
    return !atomic_flag_test_and_set(sl);
}

spinlock *create_spinlock(spinlock* sl) {
    if (sl == NULL) {
        sl = (spinlock*) malloc(sizeof *sl);
        if (sl == NULL) {
            return NULL;
        }
    }
    unlock(sl);
    return sl;
}
// \SPINLOCK


void test_single_thread() {
    spinlock sl;
    create_spinlock(&sl);
    lock(&sl);
    assert(!try_lock(&sl));
    unlock(&sl);
    assert(try_lock(&sl));
}



int result = 0;

void *thread_fun_1(void* arg) {
    spinlock *sl = arg;
    for (int i = 0; i < 6; ++i) {
        if (try_lock(sl)) {
            result = 1234;
            unlock(sl);
        }
    }
    return NULL;
}

void *thread_fun_2(void* arg) {
    spinlock *sl = arg;
    for (int i = 0; i < 6; ++i) {
        if (try_lock(sl)) {
            result = 45678;
            unlock(sl);
        }
    }
    return NULL;
}

void test_two_threads() {
    spinlock sl;
    create_spinlock(&sl);

    pthread_t t1, t2;

    assert(!pthread_create(&t1, NULL, thread_fun_1, &sl));
    assert(!pthread_create(&t2, NULL, thread_fun_2, &sl));

    void* unused;
    pthread_join(t1, &unused);
    pthread_join(t2, &unused);

    assert(result == 1234 || result == 45678);
}

int main() {
    test_single_thread();
    test_two_threads();

    return 0;
}