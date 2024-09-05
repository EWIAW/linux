#pragma once

#include <pthread.h>

class Mutex
{
public:
    //构造函数
    Mutex(pthread_mutex_t* lock = nullptr)
    :_lock(lock)
    {
        //什么都不做
    }

    //加锁
    void lock()
    {
        if(_lock != nullptr)
            pthread_mutex_lock(_lock);
    }

    //解锁
    void unlock()
    {
        if(_lock != nullptr)
            pthread_mutex_unlock(_lock);
    }

    //析构函数
    ~Mutex()
    {
        //什么都不做
    }
private:
    pthread_mutex_t* _lock;
};

class LockGuard
{
public:
    //构造函数
    LockGuard(pthread_mutex_t* mutex)
    :_mutex(mutex)
    {
        _mutex.lock();
    }
    //析构函数
    ~LockGuard()
    {
        _mutex.unlock();
    }
private:
    Mutex _mutex;
};