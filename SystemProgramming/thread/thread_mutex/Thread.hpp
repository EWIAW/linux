#pragma once

#include <assert.h>

#include <string>

#include <pthread.h>

typedef void *(*func)(void *); // 重命名函数指针

class Thread
{
public:
    // 构造函数
    Thread(func start_routine, void *args, int number)
        : _start_routine(start_routine), _args(args), _number(number)
    {
        // 给线程起名字
        char namebuffer[64];
        snprintf(namebuffer, sizeof(namebuffer), "线程-%d", number);
        _name = namebuffer;

        // 创建线程并运行
        int n = pthread_create(&_tid, nullptr, _start_routine, _args); // 创建线程
        assert(n == 0);
    }

    // 线程等待
    int join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(n == 0);
    }

    ~Thread()
    {
        // 什么都不做
    }

private:
    std::string _name;   // 线程名称
    pthread_t _tid;      // 线程id
    int _number;         // 线程number
    func _start_routine; // 线程要执行的函数
    void *_args;         // 线程参数
};