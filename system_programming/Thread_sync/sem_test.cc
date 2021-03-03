#include "Semaphore.h"
#include <queue>
#include <thread>
#include <vector>
#include <unistd.h>
/*
Producer product 0!
-------Consumer consumed product 0!  
Producer product 1!
Producer product 2!
Producer product 3!
Producer product 4!
Producer product 5!               blank资源用完，等待消费（体现调度性）
-------Consumer consumed product 1!
Producer product 6!
-------Consumer consumed product 2!
Producer product 7!
-------Consumer consumed product 3!  按顺序进行消费
Producer product 8!
*/

#define maxSize 5 
Semaphore blank(maxSize);  // 一开始5个空格
Semaphore product(0);       // 一开始0个产品

Semaphore mutex(1);     // 初始化为1，相当于互斥锁
std::queue<int> queue;

void producer()
{
    for(int i = 0;i < 50;++i)
    {
        blank.wait();   // 试图获取空格资源
        mutex.wait();      // 试图获取进入临界区权限资源，体现互斥性

        queue.push(i);
        printf("Producer product %d!\n", i);
        mutex.notify_one();
        product.notify_one();   // 提醒有产品资源
        usleep(50000);
    }
}

void consumer()
{
    for(int i = 0;i < 50;++i)
    {
        product.wait();   // 试图获取产品资源
        mutex.wait();       

        int cur = queue.front();
        queue.pop();
        printf("-------Consumer consumed product %d!\n", cur);
        mutex.notify_one();
        blank.notify_one();  // 提醒有空格资源
        usleep(500000);
    }
}

int main()
{
    std::vector<std::thread> thrds;
    thrds.reserve(2);

    thrds.emplace_back(std::thread(producer));
    thrds.emplace_back(std::thread(consumer));

    for(auto& thrd : thrds)
    {
        thrd.join();
    }
    return 0;
}