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
            Log_Message(WARN, "join the epoll model failed , errno : %d , reason : %s", errno, strerror(errno));
            close(fd);
            return;
        }
        Log_Message(DUBUG, "join the epoll model success ! ! !");
        return;
    }

    // 将连接从epoll模型中删除
    void Del(int sock)
    {
        int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
        if (ret == -1)
        {
            Log_Message(ERROR, "epoll_ctl to delete is failed , errno : %d , reason : %s", errno, strerror(errno));
            return;
        }
        Log_Message(DUBUG, "epoll_ctl delete is success");
        return;
    }

    // 监听等待文件描述符
    int Wait(struct epoll_event *events, int maxnum, int timeout)
    {
        int ret = epoll_wait(_epfd, events, maxnum, timeout);
        switch (ret)
        {
        case 0:
            Log_Message(DUBUG, "time out...");
            break;
        case -1:
            Log_Message(ERROR, "epoll_wait failed , errno : %d , reason : %s", errno, strerror(errno));
            break;
        default:
            Log_Message(DUBUG, "events ready!!!");
            break;
        }
        return ret;
    }

    // 控制事件
    bool Control(int sock, int op, int event)
    {
        struct epoll_event ee;
        ee.data.fd = sock;
        ee.events = event;
        int n = 0;
        if (op == EPOLL_CTL_DEL)
        {
            n = epoll_ctl(_epfd, op, sock, nullptr);
        }
        if (op == EPOLL_CTL_MOD)
        {
            n = epoll_ctl(_epfd, op, sock, &ee);
        }
        else
        {
            n = -1;
        }
        return n == 0;
    }

private:
    int _epfd;
};