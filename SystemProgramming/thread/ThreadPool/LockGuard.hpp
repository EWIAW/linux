#pragma once

#include <pthread.h>

class Mutex
{
public:
    // 构造函数
    Mutex(pthread_mutex_t *lock = nullptr)
        : _lock(lock)
    {
    }

    // 加锁
    void lock()
    {
        if (_lock != nullptr)
            pthread_mutex_lock(_lock);
    }

    // 解锁
    void unlock()
    {
        if (_lock != nullptr)
            pthread_mutex_unlock(_lock);
    }

    ~Mutex()
    {
    }

private:
    pthread_mutex_t *_lock;
};

// RAII方式的锁
class LockGuard
{
public:
    LockGuard(pthread_mutex_t *mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~LockGuard()
    {
        _mutex.unlock();
    }

private:
    Mutex _mutex;
};
