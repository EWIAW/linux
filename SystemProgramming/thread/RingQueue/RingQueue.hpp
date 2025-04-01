#pragma once

#include <assert.h>

#include <vector>

#include <pthread.h>
#include <semaphore.h>

const int gcap = 5;

// 基于信号量的环形队列
template <class T>
class RingQueue
{
private:
    std::vector<T> _queue;   // 环形队列
    int _capacity;           // 队列的最大容量
    sem_t _spacesem;         // 生产者的信号量，空间资源信号量，即有多少个空位
    sem_t _datasem;          // 消费者的信号量，数据资源信号量，即有多少个数据
    int _spacepos;           // 队列空间资源的下标
    int _datapos;            // 队列数据资源的下标
    pthread_mutex_t _pmutex; // 生产者的锁
    pthread_mutex_t _cmutex; // 消费者的锁

public:
    RingQueue(const int &capacity = gcap)
        : _queue(capacity), _capacity(capacity), _spacepos(0), _datapos(0)
    {
        int ret = sem_init(&_spacesem, 0, _capacity); // 对于空间资源，刚开始信号量为_capacity个
        assert(ret == 0);
        ret = sem_init(&_datasem, 0, 0); // 对于数据资源，刚开始信号量为0个
        assert(ret == 0);

        pthread_mutex_init(&_pmutex, nullptr);
        pthread_mutex_init(&_cmutex, nullptr);
    }

    void push(const T &in)
    {
        P(_spacesem);
        pthread_mutex_lock(&_pmutex);
        _queue[_spacepos++] = in;
        _spacepos %= _capacity;
        pthread_mutex_unlock(&_pmutex);
        V(_datasem);
    }

    void pop(T *out)
    {
        P(_datasem);
        pthread_mutex_lock(&_cmutex);
        *out = _queue[_datapos++];
        _datapos %= _capacity;
        pthread_mutex_unlock(&_cmutex);
        V(_spacesem);
    }

    ~RingQueue()
    {
        int ret = sem_destroy(&_spacesem);
        assert(ret == 0);
        ret = sem_destroy(&_datasem);
        assert(ret == 0);

        pthread_mutex_destroy(&_pmutex);
        pthread_mutex_destroy(&_cmutex);
    }

private:
    // P操作，对信号量进行--，申请信号量
    void P(sem_t &sem)
    {
        int ret = sem_wait(&sem);
        assert(ret == 0);
    }

    // V操作，对信号量进行++，归还信号量
    void V(sem_t &sem)
    {
        int ret = sem_post(&sem);
        assert(ret == 0);
    }
};
