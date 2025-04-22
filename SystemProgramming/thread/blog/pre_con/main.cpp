#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>

#include "block_queue.hpp"
#include "task.hpp"

// 生产者运行的函数
void *preducer(void *args)
{
    block_queue<Task> *bq = (block_queue<Task> *)args;

    while (true)
    {
        int x = rand() % 1000;
        int y = rand() % 1000;
        Task t(x, y);
        printf("生产者生产一个任务：%d + %d = ？？？\n", x, y);
        bq->push(t);
        sleep(1);
    }
    return nullptr;
}

void *consumer(void *args)
{
    block_queue<Task> *bq = (block_queue<Task> *)args;

    while (true)
    {
        Task t;
        bq->pop(t);
        int result = t();
        printf("消费者者消费一个任务：%d + %d = %d\n", t._x_, t._y_, result);
        sleep(1);
    }

    return nullptr;
}

int main()
{
    srand(time(nullptr));
    block_queue<Task> bq(10);

    std::vector<pthread_t> pre_vpt(1);
    for (int i = 0; i < pre_vpt.size(); i++)
    {
        pthread_create(&pre_vpt[i], nullptr, preducer, &bq);
    }

    std::vector<pthread_t> con_vpt(10);
    for (int i = 0; i < con_vpt.size(); i++)
    {
        pthread_create(&con_vpt[i], nullptr, consumer, &bq);
    }

    // 线程等待
    for (int i = 0; i < pre_vpt.size(); i++)
    {
        pthread_join(pre_vpt[i], nullptr);
    }

    for (int i = 0; i < con_vpt.size(); i++)
    {
        pthread_join(con_vpt[i], nullptr);
    }
    return 0;
}