#include <iostream>
#include <vector>
#include <unistd.h>
#include "Thread.hpp"

using namespace std;

int ticket = 10000; // 火车票的票数

// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *start_routine(void *args)
{
    while (true)
    {
        if (ticket > 0)
        {
            usleep(1000);//让打印慢一点，方便我们观察
            cout << "我是" << (char *)args << "：我正在抢票：" << ticket-- << endl;
        }
        else
        {
            break;
        }
    }
    return nullptr;
}

int main()
{
    vector<Thread *> vt; // 定义容器存放线程Thread的地址
    // 创建五个线程去抢票
    for (int i = 0; i < 5; i++)
    {
        Thread *t = new Thread;
        t->start(start_routine, (void *)t->getname().c_str());
        vt.push_back(t);
    }

    // 等待线程退出
    for (int i = 0; i < 5; i++)
    {
        vt[i]->join();
    }
    return 0;
}