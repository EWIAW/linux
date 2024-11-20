#pragma once

#include <string>
#include <functional>

#include <sys/epoll.h>
#include <unistd.h>

#include "socket.hpp"
#include "log.hpp"
#include "err.hpp"

class epollserver
{
    static const uint16_t default_port = 3489;
    static const int default_size = 128;
    static const int default_num = -1;

    using func_t = std::function<std::string(std::string)>;

public:
    epollserver(func_t func)
        : _listen_socket(default_num),
          _epfd(default_num),
          _port(default_port),
          _arr_ee(nullptr),
          _arr_ee_num(default_size),
          _func(func)
    {
    }

    ~epollserver()
    {
        if (_listen_socket != default_num)
            close(_listen_socket);
        if (_epfd != default_num)
            close(_epfd);
    }

    void init_Server()
    {
        // 1.创建套接字
        _listen_socket = Sock::Socket();
        Sock::Bind(_listen_socket, _port);
        Sock::Listen(_listen_socket);

        // 2.创建epoll模型
        _epfd = epoll_create(default_size);

        // 3.将_listen_socket套接字加入到epoll模型中
        struct epoll_event ee;
        ee.data.fd = _listen_socket;
        ee.events = EPOLLIN;
        epoll_ctl(_epfd, EPOLL_CTL_ADD, _listen_socket, &ee);

        // 4.创建存储事件的数组
        _arr_ee = new struct epoll_event[_arr_ee_num];
    }

    void run()
    {
        int timeout = -1;
        for (;;)
        {
            int ret = epoll_wait(_epfd, _arr_ee, default_size, timeout);
            switch (ret)
            {
            case 0:
                Log_Message(INFO, "time out...");
                break;
            case -1:
                Log_Message(FATAL, "epoll_wati failed!!!");
                break;
            default:
                Log_Message(INFO, "events ready");
                Handler_event(ret); // 处理事件
                break;
            }
        }
    }

    void Handler_event(int n)
    {
        for (int i = 0; i < n; i++)
        {
            if (_arr_ee[i].data.fd == _listen_socket && (_arr_ee[i].events & EPOLLIN))
            {
                //_listen_socket套接字就绪，将新套接字 插入 epoll模型中
                std::string ip;
                uint16_t port;
                int sock = Sock::Accept(_listen_socket, &ip, &port);
                struct epoll_event ee;
                ee.data.fd = sock;
                ee.events = EPOLLIN;
                epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ee);
            }
            else if (_arr_ee[i].events & EPOLLIN)
            {
                // 普通套接字就绪
                char buffer[1024];
                ssize_t n = read(_arr_ee->data.fd, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    Log_Message(INFO, "client say# : %s", buffer);

                    // 处理消息，将消息发送回去
                    std::string response = _func(buffer);
                    ssize_t n = write(_arr_ee->data.fd, response.c_str(), response.size());
                }
                else if (n == 0)
                {
                    // 客户端退出了，要将文件描述符从epoll模型中删除
                    Log_Message(INFO, "client quit!!!");
                    epoll_ctl(_epfd, EPOLL_CTL_DEL, _arr_ee->data.fd, nullptr); // 先移除再关闭，因为要确保fd是合法的
                    close(_arr_ee->data.fd);
                }
                else
                {
                    Log_Message(WARN, "read failed!!!");
                }
            }
            else
            {
                // todo
            }
        }
    }

private:
    int _listen_socket;
    int _epfd; // epoll模型的文件描述符
    uint16_t _port;
    struct epoll_event *_arr_ee;
    int _arr_ee_num;
    func_t _func;
};