#include <unistd.h>
#include <thread>
#include <pthread.h> //rwlock
#include <functional>
#include <iostream>
#include <vector>
/*
=====Writer 1 before: cnt = 669 after: cnt = 670   后面几个线程到来时
=====Writer 0 before: cnt = 670 after: cnt = 671    写线程优先，独占
-------------Reader 3, cnt = 671                    读线程在后，可共享
-------------Reader 1, cnt = 671
-------------Reader 0, cnt = 671
-------------Reader 2, cnt = 671
*/

// RAII
class writeLockGuard {
public:
    writeLockGuard(::pthread_rwlock_t* rwlock)
        : rwlock_(rwlock) {
        pthread_rwlock_wrlock(rwlock_);
    }

    ~writeLockGuard() { pthread_rwlock_unlock(rwlock_);}
private:
    pthread_rwlock_t* rwlock_;
};

class readLockGuard {
public:
    readLockGuard(::pthread_rwlock_t* rwlock)
        : rwlock_(rwlock) {
        pthread_rwlock_rdlock(rwlock_);
    }

    ~readLockGuard() { pthread_rwlock_unlock(rwlock_); }
private:
    pthread_rwlock_t* rwlock_;
};

pthread_rwlock_t rwlock;
int global_counter = 0; // 共享资源

void writerThread(int id) {
    int save = 0;
    while(true) {
        writeLockGuard lock_guard(&rwlock);
        save = global_counter;
        usleep(1000); // 该时间段内，依旧独占写锁
        std::cout << "=====Writer " << id
                << " before: cnt = " <<  save
                << " after: cnt = " << ++global_counter << std::endl;
    }
    usleep(5000000);
    
}

void readerThread(int id)
{
    while(true) {
        {
            readLockGuard lock_guard(&rwlock);
            printf("-------------Reader %d, cnt = %d\n", id, global_counter);
        }
        usleep(1000);
    }
}

int main() {
    std::vector<std::thread> thrds;
    thrds.reserve(8);

    pthread_rwlock_init(&rwlock, NULL);
    // 3个写线程
    for(int i = 0;i < 2;++i) {
        std::thread thrd(std::bind(writerThread, i));
        thrds.emplace_back(std::move(thrd));
    }

    // 5个读线程
    for(int i = 0;i < 4;++i) {
        std::thread thrd(std::bind(readerThread, i));
        thrds.emplace_back(std::move(thrd));
    }

    for(auto& thrd : thrds) {
        thrd.join();
    }

    pthread_rwlock_destroy(&rwlock);
    return 0;
}


