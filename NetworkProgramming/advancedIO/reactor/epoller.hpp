// 封装epoll系统调用
#pragma once

#include <cstring>

#include <sys/epoll.h>
#include <unistd.h>

#include "log.hpp"
#include "err.hpp"

class epoller
{
    const static int default_num = -1;
    const static int default_size = 128;

public:
    epoller()
        : _epfd(default_num)
    {
    }
    ~epoller()
    {
        if (_epfd != default_num)
            close(_epfd);
    }

    // 创建epoll模型
    void Create()
    {
        _epfd = epoll_create(default_size);
        if (_epfd == -1)
        {
            Log_Message(FATAL, "create epoll fd is failed , errno : %d , reason : %s", errno, strerror(errno));
            exit(CREATE_EPOLL_ERR);
        }
        Log_Message(DUBUG, "create epoll fd success");
    }

    // 将连接添加到epoll模型中
    void AddEvent(int fd, int op)
    {
        struct epoll_event ee;
        ee.data.fd = fd;
        ee.events = op;
        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ee);
        if (ret == -1)
        {
            Log_Message(WARN, "join the epoll model failed ! ! !");
            close(fd);
            return;
        }
        Log_Message(DUBUG, "join the epoll model success ! ! !");
        return;
    }

    // 将连接从epoll模型中删除
    void Del()
    {
    }

    // 监听等待文件描述符
    void Wait()
    {
        // epoll_wait(_epfd, )
    }

private:
    int _epfd;
};