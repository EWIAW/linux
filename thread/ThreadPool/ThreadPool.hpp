#pragma once

#include <vector>
#include <queue>

#include <pthread.h>
#include <unistd.h>

#include "Thread.hpp"
#include "LockGuard.hpp"

template <class T>
class ThreadPool
{
    static const int ThreadNum = 5; // 线程池中线程的默认个数

public:
    // 构造函数，创建一批线程
    ThreadPool(const int num = ThreadNum)
        : _num(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        for (int i = 0; i < num; i++)
        {
            Thread *t = new Thread();
            _vecThread.push_back(t);
        }
    }

    // 往任务队列里插入任务
    void push(const T &in)
    {
        // pthread_mutex_lock(&_mutex);
        // _taskQueue.push(in);
        // pthread_cond_signal(&_cond);
        // pthread_mutex_unlock(&_mutex);
        {
            LockGuard lock(&_mutex);
            _taskQueue.push(in);
            pthread_cond_signal(&_cond);
        }
    }

    // 从队列中拿任务
    // 在pop函数中，不需要再加锁，因为在调用HandlerTask方法时，在里面已经加锁了
    T pop()
    {
        T t = _taskQueue.front();
        _taskQueue.pop();
        return t;
    }

    // 让所有线程跑起来
    void run()
    {
        for (int i = 0; i < _num; i++)
        {
            _vecThread[i]->start(HandlerTask, this);
        }
    }

    static void *HandlerTask(void *args)
    {
        ThreadPool<T> *threadpool = (ThreadPool<T> *)args;
        while (true)
        {
            pthread_mutex_lock(&threadpool->_mutex);

            while (threadpool->_taskQueue.empty())
            {
                pthread_cond_wait(&threadpool->_cond, &threadpool->_mutex);
            }
            T task = threadpool->pop();
            pthread_mutex_unlock(&threadpool->_mutex);
            task(); // 执行任务
        }
        return nullptr;
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
        // 释放线程资源
        for (int i = 0; i < _num; i++)
        {
            delete _vecThread[i];
        }
    }

private:
    int _num;                         // 线程池中线程的个数
    std::vector<Thread *> _vecThread; // 存放线程的数组
    std::queue<T> _taskQueue;         // 存放任务的队列
    pthread_mutex_t _mutex;           // 锁
    pthread_cond_t _cond;             // 条件变量
};