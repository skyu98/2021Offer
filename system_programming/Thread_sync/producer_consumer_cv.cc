#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <functional>
/*
consumer0 ---get: Product 0
consumer1 ---get: Product 1
producer: The queue is FULL!!!
consumer0 ---get: Product 2
consumer1 ---get: Product 3
producer: The queue is FULL!!!
consumer0 ---get: Product 4
......
consumer0 quit!
consumer1 quit!
*/

std::mutex mutex;
std::condition_variable notFull;
std::condition_variable notEmpty;
std::queue<std::string> queue;
const int maxSize = 5; // 最多存放maxSize个产品

void consumer(int id)
{
    while(true)
    {
        std::string product;
        {
            std::unique_lock<std::mutex> lock(mutex); // 先取得锁再判断
            while(queue.empty())  // 如果队列为空
            {
                // 解锁，阻塞等待条件满足
                notEmpty.wait(lock);
                // 当条件满足，该阻塞线程被唤醒；再次检查是否满足条件
            }
            // 条件满足，申请获得锁，取出产品
            product = std::move(queue.front());
            queue.pop();
        }
        // 释放锁(锁的粒度一定要小！)
        // 当前队列不满，通知生产者可以进行生产
        notFull.notify_one();
        if(product == "quit!")
        {
            printf("consumer%d quit!\n", id);
            break;
        }
        printf("consumer%d ---get: %s\n", id, product.c_str());
        sleep(1);
    }
}

void producer()
{
    for(int i = 0;i < 20;++i)
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(queue.size() >= maxSize)  // 如果队列满了
            {
                printf("producer: The queue is FULL!!!\n");
                notFull.wait(lock);
            }
            // 生产
            std::string product("Product " + std::to_string(i));
            queue.push(std::move(product));
        }
        // 通知消费者可以进行消费
        notEmpty.notify_one();
        usleep(5000);
    }

    for(int i = 0;i < 2;++i)
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(queue.size() >= maxSize)  // 如果队列满了
            {
                notFull.wait(lock);
            }
            // 提醒退出
            queue.push("quit!");
        }
        // 通知消费者可以进行消费
        notEmpty.notify_one();
    }


}

int main()
{
    std::vector<std::thread> thrds;
    thrds.reserve(3);

    thrds.emplace_back(std::thread(producer));

    for(int i = 0;i < 2;++i)
    {
        thrds.emplace_back(std::thread(std::bind(consumer, i)));
    } 

    for(auto& thrd : thrds)
    {
        thrd.join();
    }
    return 0;
}