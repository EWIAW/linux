#pragma once

#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include "socket.hpp"

class server
{
    static const uint16_t default_port = 3489;
    static const int fdnum = sizeof(fd_set) * 8;
    static const int default_fd = -1;

public:
    server(const uint16_t port = default_port)
        : _port(port), _listen_socket(-1), _arr(nullptr)
    {
    }

    void initServer()
    {
        _listen_socket = Sock::Socket();
        Sock::Bind(_listen_socket, _port);
        Sock::Listen(_listen_socket);
        _arr = new int[fdnum];
        for (int i = 0; i < fdnum; i++)
        {
            _arr[i] = default_fd;
        }
        _arr[0] = _listen_socket;
    }

    void Print()
    {
        std::cout << "fd list : ";
        for (int i = 0; i < fdnum; i++)
        {
            if (_arr[i] != -1)
            {
                std::cout << _arr[i] << " ";
            }
        }
        std::cout << std::endl;
    }

    void Handler_Event(fd_set rfds)
    {
        if (FD_ISSET(_listen_socket, &rfds))
        {
            std::string clientip;
            uint16_t clientport;
            int fd = Sock::Accept(_listen_socket, &clientip, &clientport);
            if (fd < 0)
            {
                Log_Message(ERROR, "accept socket failed!!!");
                return;
            }
            Log_Message(DUBUG, "accpet socket success , client : %s , port : %d", clientip.c_str(), clientport);
            // 将新的sock托管给select
            int i = 0;
            for (; i < fdnum; i++)
            {
                if (_arr[i] != default_fd)
                    continue;
                else
                    break;
            }
            if (i == fdnum)
            {
                Log_Message(ERROR, "server is full ,please wait!!!");
                close(fd);
            }
            else
            {
                _arr[i] = fd;
            }

            Print();
        }
    }

    void run()
    {
        for (;;)
        {
            int maxfd = 0;
            fd_set rfds;
            FD_ZERO(&rfds);
            struct timeval timeout = {1, 0};
            for (int i = 0; i < fdnum; i++)
            {
                if (_arr[i] == default_fd)
                    continue;

                FD_SET(_arr[i], &rfds); // 设置所有需要监听的文件描述符

                if (_arr[i] > maxfd)
                    maxfd = _arr[i];
            }
            // int n = select(_listen_socket + 1, &rfds, nullptr, nullptr, &timeout);
            int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr);
            switch (n)
            {
            case 0: // 说明没有时间就绪
                Log_Message(DUBUG, "timeout ...");
                break;
            case -1: // 说明发生错误
                Log_Message(FATAL, "select error,code : %d , err string : %s", errno, strerror(errno));
            default: // 说明有事件就绪了
                Log_Message(DUBUG, "get a new link ...");
                Handler_Event(rfds); // 处理事件
                break;
            }
        }
    }

    ~server()
    {
        if (_listen_socket < 0)
        {
            close(_listen_socket);
        }
    }

private:
    int _listen_socket;
    uint16_t _port;
    int *_arr;
};
