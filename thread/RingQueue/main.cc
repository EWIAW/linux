#include <time.h>

#include <iostream>

#include <pthread.h>
#include <unistd.h>

#include "RingQueue.hpp"
#include "task.hpp"

// 生产者线程
void *Producer(void *args)
{
    RingQueue<TaskCal> *rq = (RingQueue<TaskCal> *)args;
    while (true)
    {
        // 生产数据
        // Verson 1
        // int data = rand() % 100;
        // rq->push(data);
        // std::cout << "生产完成，生产的数据是：" << data << std::endl;

        // Verson 2
        int x = rand() % 100;
        int y = rand() % 50;
        TaskCal task(x, y);
        rq->push(task);
        std::cout << "生产任务完成：" << task.ToTaskString() << std::endl;
        sleep(1);
    }
    return nullptr;
}

// 消费者线程
void *Consumer(void *args)
{
    RingQueue<TaskCal> *rq = (RingQueue<TaskCal> *)args;
    while (true)
    {
        // 消费数据
        // Verson 1
        // int data;
        // rq->pop(&data);
        // std::cout << "消费完成，消费的数据是：" << data << std::endl;

        // Verson
        TaskCal task;
        rq->pop(&task);
        std::string result = task();
        std::cout << "消费任务完成：" << result << std::endl;
    }
    return nullptr;
}

int main()
{
    srand(time(nullptr));

    RingQueue<TaskCal> *rq = new RingQueue<TaskCal>();

    pthread_t ptid[3], ctid[3];

    // 创建线程
    pthread_create(&ptid[0], nullptr, Producer, (void *)rq);
    pthread_create(&ptid[1], nullptr, Producer, (void *)rq);
    pthread_create(&ptid[2], nullptr, Producer, (void *)rq);

    pthread_create(&ctid[0], nullptr, Consumer, (void *)rq);
    pthread_create(&ctid[1], nullptr, Consumer, (void *)rq);
    pthread_create(&ctid[2], nullptr, Consumer, (void *)rq);

    // 线程等待
    pthread_join(ptid[0], nullptr);
    pthread_join(ptid[1], nullptr);
    pthread_join(ptid[2], nullptr);
    pthread_join(ctid[0], nullptr);
    pthread_join(ctid[1], nullptr);
    pthread_join(ctid[2], nullptr);

    delete rq;

    return 0;
}