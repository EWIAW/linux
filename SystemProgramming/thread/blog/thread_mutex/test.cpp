#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include "Thread.hpp"

using namespace std;

int ticket = 10000; // 火车票的票数

// 顶一个存放线程数据的结构体，里面包含锁和线程名
struct ThreadData
{
    ThreadData(pthread_mutex_t *lock, const string threadname)
        : _lock(lock), _threadname(threadname)
    {
    }

    pthread_mutex_t *_lock;
    string _threadname;
};

void *start_routine(void *args)
{
    ThreadData *td = (ThreadData *)args;
    while (true)
    {
        pthread_mutex_lock(td->_lock); // 加锁
        if (ticket > 0)
        {
            usleep(1000); // 让打印慢一点，方便我们观察
            cout << "我是" << td->_threadname.c_str() << "：我正在抢票：" << ticket-- << endl;
            pthread_mutex_unlock(td->_lock); // 解锁
        }
        else
        {
            pthread_mutex_unlock(td->_lock); // 解锁
            break;
        }
        usleep(500);
    }
    return nullptr;
}

int main()
{
    vector<Thread *> vt; // 定义容器存放线程Thread的地址
    pthread_mutex_t mtx; // 定义一把共用的互斥锁
    // 创建五个线程去抢票
    for (int i = 0; i < 5; i++)
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "thread:%d", i);
        ThreadData *td = new ThreadData(&mtx, buffer);
        Thread *t = new Thread();
        t->start(start_routine, td);
        vt.push_back(t);
    }

    // 等待线程退出
    for (int i = 0; i < 5; i++)
    {
        vt[i]->join();
    }
    return 0;
}