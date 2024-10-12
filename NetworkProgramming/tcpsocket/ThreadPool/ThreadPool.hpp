#pragma once

#include <vector>
#include <queue>
#include <mutex>

#include <pthread.h>
#include <unistd.h>

#include "Thread.hpp"
#include "LockGuard.hpp"
#include "../log.hpp"

namespace myThreadPool
{
    // 声明
    template <class T>
    class ThreadData;

    template <class T>
    class ThreadPool;

    // 存储线程数据
    template <class T>
    class ThreadData
    {
    public:
        ThreadData(const std::string &name, ThreadPool<T> *threadpool)
            : _name(name), _threadpool(threadpool)
        {
        }

    public:
        std::string _name;
        ThreadPool<T> *_threadpool;
    };

    template <class T>
    class ThreadPool
    {
        static const int ThreadNum = 5; // 线程池中线程的默认个数

    public:
        static ThreadPool<T> *GetInstance()
        {
            if (_tp == nullptr)
            {
                _cppmutex.lock();
                if (_tp == nullptr)
                {
                    _tp = new ThreadPool<T>();
                }
                _cppmutex.unlock();
            }
            return _tp;
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
                ThreadData<T> *td = new ThreadData<T>(_vecThread[i]->getname(), this);
                _vecThread[i]->start(HandlerTask, td);
                // std::cout << _vecThread[i]->getname() << "start..." << std::endl;
                logmessage(DEBUG, "%s start...", _vecThread[i]->getname().c_str());
            }
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
        // 构造函数，创建一批线程
        ThreadPool(const int num = ThreadNum)
            : _num(num)
        {
            pthread_mutex_init(&_mutex, nullptr);
            pthread_cond_init(&_cond, nullptr);
            for (int i = 0; i < _num; i++)
            {
                Thread *t = new Thread();
                _vecThread.push_back(t);
            }
        }

        void operator=(const ThreadPool &tp) = delete;
        ThreadPool(const ThreadPool &tp) = delete;

        static void *HandlerTask(void *args)
        {
            ThreadData<T> *td = (ThreadData<T> *)args;
            while (true)
            {
                pthread_mutex_lock(&td->_threadpool->_mutex);

                // 这里使用while而不用if的原因：防止多个线程同时结束等待，而此时任务队列中只有一个任务，而出现问题
                while (td->_threadpool->_taskQueue.empty())
                {
                    pthread_cond_wait(&td->_threadpool->_cond, &td->_threadpool->_mutex);
                }
                T task = td->_threadpool->pop();
                pthread_mutex_unlock(&td->_threadpool->_mutex);
                std::cout << td->_name << "获取了一个任务：" << std::endl;
                task(); // 执行任务
            }
            delete td;
            return nullptr;
        }

    private:
        int _num;                         // 线程池中线程的个数
        std::vector<Thread *> _vecThread; // 存放线程的数组
        std::queue<T> _taskQueue;         // 存放任务的队列
        pthread_mutex_t _mutex;           // 锁
        pthread_cond_t _cond;             // 条件变量

        static std::mutex _cppmutex; // 给创建单例的函数使用的锁
        static ThreadPool<T> *_tp;   // 单例静态指针
    };

    template <class T>
    ThreadPool<T> *ThreadPool<T>::_tp = nullptr;

    template <class T>
    std::mutex ThreadPool<T>::_cppmutex;
}
