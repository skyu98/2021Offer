#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    explicit Semaphore(unsigned int init = 0) : count_(init) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_); 
        while(count_ == 0) { // 当前没有资源
            cv_.wait(lock);  // 阻塞等待资源
        }
        --count_;  // 有资源可以获取；获取资源，计数减一
    }

    void notify_one() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;  // 资源增加
        cv_.notify_one(); // 唤醒一个阻塞的线程 
    }

private:
    int count_;
    std::mutex mutex_;
    std::condition_variable cv_;
};
#endif // __SEMAPHORE_H__