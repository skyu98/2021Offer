#include <thread>
#include <mutex>
#include <iostream>
#include <unistd.h>

// 此例中屏幕为共享资源，我们需要两个线程有序输出
std::mutex mutex;

void threadFunc() {
    for(int i = 0;i < 50;++i) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << "hello ";
            std::cout << "world" << std::endl;
        }
        sleep(1); // 确保其他线程获得锁
    }
}

int main() {
    std::thread lowerThread(threadFunc);
    lowerThread.detach();
    for(int i = 0;i < 50;++i) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << "HELLO ";
            std::cout << "WORLD" << std::endl;
        }
        sleep(1);
    }
    return 0;
}