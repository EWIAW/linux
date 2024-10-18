#pragma once

#include <cstring>

#include <iostream>
#include <functional>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include "protocol.hpp"

typedef std::function<void(const HttpRequest &request, HttpResponse &response)> func_t;

static const int gbacklog = 5;     // 给listen监听函数使用
static const int buffernum = 4096; // 缓冲区大小

// 枚举exit退出码
enum
{
    SOCKET_ERR = 1, // 创建socket失败
    BIND_ERR,       // 绑定socket失败
    LISTEN_ERR,     // 监听失败
};

void HandlerEnter(const int &sockfd, const func_t func)
{
    while (true)
    {
        char buffer[buffernum];
        HttpRequest request;
        HttpResponse respone;
        // 1.读取请求
        ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            // 2.反序列化请求
            request._inbuffer = buffer;

            std::cout << "---------------完整请求开始---------------" << std::endl;
            std::cout << request._inbuffer << std::endl;
            std::cout << "---------------完整请求结束---------------" << std::endl;

            // 3.处理请求并返回响应
            request.parse(); // 解析请求
            func(request, respone);
            // 4.序列化响应

            // 5.发送响应
            send(sockfd, respone._outbuffer.c_str(), respone._outbuffer.size(), 0);
        }
    }
}

class httpserver
{
public:
    httpserver(const uint16_t &port, const func_t &func)
        : _listenSockfd(-1), _port(port), _func(func)
    {
    }

    ~httpserver()
    {
    }

    void init()
    {
        // 1.创建套接字
        _listenSockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenSockfd == -1)
        {
            exit(SOCKET_ERR);
        }

        // 2.bind套接字
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        int ret = bind(_listenSockfd, (struct sockaddr *)&local, sizeof(local));
        if (ret == -1)
        {
            exit(BIND_ERR);
        }

        // 3.listen监听
        ret = listen(_listenSockfd, gbacklog);
        if (ret == -1)
        {
            exit(LISTEN_ERR);
        }
    }

    void start()
    {
        // 4.accept链接
        for (;;)
        {
            struct sockaddr_in client; // 所连接的客户端信息
            socklen_t len = sizeof(client);
            int sockfd = accept(_listenSockfd, (struct sockaddr *)&client, &len); // 这里的网络通信可以相当于文件操作
            if (sockfd == -1)
            {
                continue;
            }

            pid_t pid = fork();
            if (pid == 0)
            {
                // 子进程
                close(_listenSockfd);
                pid_t pid = fork();
                if (pid == 0)
                {
                    // 孙子进程
                    HandlerEnter(sockfd, _func);
                    close(sockfd);
                    exit(0);
                }
                exit(0);
            }
            // 父进程
            pid_t ret = waitpid(pid, nullptr, 0);
            close(sockfd); // 关闭不再使用的文件描述符
        }
    }

private:
    int _listenSockfd;
    uint16_t _port;
    func_t _func;
};