// 对原生线程库的线程函数进行封装
#pragma once

#include <cstdio>
#include <cassert>

#include <string>
#include <functional>

#include <pthread.h>

class Thread
{
    const int size = 1024;
    typedef std::function<void *(void *)> func_t;

private:
    pthread_t _tid;        // 线程id
    std::string _name;     // 线程名
    static int _threadnum; // 线程序号
    func_t _func;          // 线程需要执行的函数
    void *_args;           // 线程传递的参数

public:
    // 构造函数时，给线程创建一个名字
    Thread()
    {
        char buffer[size];
        snprintf(buffer, sizeof(buffer), "thread-%d", _threadnum++);
        _name = buffer;
    }

    // 调用start函数时才是真正的创建线程
    void start(func_t func, void *args = nullptr)
    {
        _func = func;
        _args = args;
        int n = pthread_create(&_tid, nullptr, start_routine, this);
        assert(n == 0);
    }

    // 线程退出
    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(n == 0);
    }

    // 获取线程名
    std::string getname()
    {
        return _name;
    }

    // 析构函数
    ~Thread()
    {
    }

private:
    static void *start_routine(void *args)
    {
        Thread *_this = (Thread *)args;
        return _this->callback();
    }

    // 创建线程后，该线程需要调用的函数
    void *callback()
    {
        return _func(_args);
    }
};
int Thread::_threadnum = 0;
