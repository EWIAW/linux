#pragma once

#include <cstring>

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "log.hpp"

using namespace std;

static const int gbacklog = 5; // 给listen监听函数使用
static const int buffernum = 1024;

// 枚举错误类型
enum
{
    SOCKET_ERR = 1, // 创建socket失败
    BIND_ERR,       // 绑定socket失败
    LISTEN_ERR,     // 监听失败
};

class tcpserver
{
public:
    tcpserver(const uint16_t &port)
        : _listenSockfd(-1), _port(port)
    {
    }

    void init()
    {
        // 1.创建套接字
        _listenSockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenSockfd == -1)
        {
            logmessage(FATAL, "create socket error");
            exit(SOCKET_ERR);
        }
        logmessage(NORMAL, "create socket success");

        // 2.bind套接字
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        int ret = bind(_listenSockfd, (struct sockaddr *)&local, sizeof(local));
        if (ret == -1)
        {
            logmessage(FATAL, "bind socket error");
            exit(BIND_ERR);
        }
        logmessage(NORMAL, "bind socket success");

        // 3.listen监听
        ret = listen(_listenSockfd, gbacklog);
        if (ret == -1)
        {
            logmessage(FATAL, "listen socket error");
            exit(LISTEN_ERR);
        }
        logmessage(NORMAL, "listen socket success");
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
                logmessage(ERROR, "accept socket error");
                continue;
            }
            logmessage(NORMAL, "accept socket success");
            cout << "sock:" << sockfd << endl;

            serviceIO(sockfd); // 连接成功则进行IO通信

            // 一个客户端的连接结束后，需要关闭文件描述符
            close(sockfd);
        }
    }

    ~tcpserver()
    {
    }

private:
    void serviceIO(const int &sockfd)
    {
        char buffer[buffernum];
        while (true)
        {
            ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                buffer[n] = 0;
            }
            else
            {
                cout << "客户端已经关闭" << endl;
                break; // 走到这说明客户端已经关闭了
            }
            cout << "[服务器收到的信息]：" << buffer << endl;

            // 服务器收到信息后，再把信息发送回去
            n = write(sockfd, buffer, strlen(buffer));
        }
    }

private:
    int _listenSockfd;
    uint16_t _port;
};