#pragma once

#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include "socket.hpp"

class server
{
    static const uint16_t default_port = 3489;
    static const int fdnum = sizeof(fd_set) * 8; // 1024
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

    void Handler_Event(fd_set rfds)
    {
        // 遍历_arr数组，处理rfds已经就绪的事件
        for (int i = 0; i < fdnum; i++)
        {
            if (_arr[i] == default_fd)
                continue;

            if (_arr[i] == _listen_socket && FD_ISSET(_listen_socket, &rfds))
            {
                // 处理获取新连接事件
                Handler_New_Link();
            }
            else if (FD_ISSET(_arr[i], &rfds))
            {
                // 处理连接成功后的事件
                Handler_Ready(_arr[i], i);
            }
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

                if (_arr[i] > maxfd) // 查找最大的文件描述符
                    maxfd = _arr[i];
            }
            // int n = select(_listen_socket + 1, &rfds, nullptr, nullptr, &timeout);
            int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr);
            switch (n)
            {
            case 0: // 说明没有事件就绪
                Log_Message(DUBUG, "timeout ...");
                break;
            case -1: // 说明发生错误
                Log_Message(FATAL, "select error,code : %d , err string : %s", errno, strerror(errno));
            default: // 说明有事件就绪了
                Log_Message(DUBUG, "Handler Even ...");
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
        delete[] _arr;
    }

private:
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

    // 处理获取新连接事件
    void Handler_New_Link()
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

    // 处理连接成功后的事件
    void Handler_Ready(int fd, int pos)
    {
        char buffer[1024];
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        buffer[n] = 0;
        if (n > 0)
        {
            std::cout << "client say# " << buffer << std::endl;
        }
        else if (n == 0)
        {
            Log_Message(DUBUG, "client is close,meet to");
            close(fd);
            // 将该客户端的文件描述符从_arr数组中删除
            _arr[pos] = default_fd;
        }
        else
        {
            Log_Message(ERROR, "read the fd is failed , errno : %d , reason : %s", errno, strerror(errno));
            close(fd);
            // 将该客户端的文件描述符从_arr数组中删除
            _arr[pos] = default_fd;
        }

        // 响应信息给客户端
        ssize_t ret = write(fd, buffer, strlen(buffer) - 1);
        return;
    }

private:
    int _listen_socket;
    uint16_t _port;
    int *_arr; // 用于管理文件描述符
};
