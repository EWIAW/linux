#pragma once

#include <vector>
#include <queue>

#include "Thread.hpp"

template <class T>
class ThreadPool
{
    const int ThreadNum = 5;//线程池中线程的个数

public:
    // 构造函数
    ThreadPool(const int num = ThreadNum)
    {
        for(int i = 0; i < num; i++)
        {
            Thread* t = new Thread;
            _vecThread.push_back(t);
        }
    }

    ~ThreadPool()
    {
    }

private:
    std::vector<Thread *> _vecThread; // 存放线程的数组
    std::queue<T> _taskQueue;         // 存放任务的队列
};