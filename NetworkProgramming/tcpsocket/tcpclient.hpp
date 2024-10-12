#pragma once

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "log.hpp"

using namespace std;

static const int buffernum = 1024;

enum
{
    SOCKET_ERR = 1,
    CONNECT_ERR,
};

class tcpclient
{
public:
    tcpclient(const string &ip, const uint16_t &port)
        : _sockfd(-1), _ip(ip), _port(port)
    {
    }

    void init()
    {
        // 1.创建套接字
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd == -1)
        {
            logmessage(FATAL, "create socket error");
            exit(SOCKET_ERR);
        }
        logmessage(NORMAL, "create socket success");

        // 2.bind套接字
        // 3.listen监听
        // 4.accept连接
        // 以上操作client都不需要做或者操作系统帮助我们做
    }

    void start()
    {
        // 5.connect连接
        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(_ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(_port);
        int ret = connect(_sockfd, (struct sockaddr *)&server, sizeof(server));
        if (ret == -1)
        {
            logmessage(FATAL, "connect server error");
            exit(CONNECT_ERR);
        }
        logmessage(NORMAL, "connect server success");

        // 连接成功后，可以进行通信
        string message;
        while (true)
        {
            getline(cin, message);
            ssize_t n = write(_sockfd, message.c_str(), message.size());

            // 回显
            char buffer[buffernum];
            n = read(_sockfd, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                buffer[n] = 0;
                cout << "[server回显]" << buffer << endl;
            }
            else
            {
                break;
            }
        }
    }

    ~tcpclient()
    {
        if (_sockfd >= 0)
        {
            close(_sockfd);
        }
    }

private:
    int _sockfd;
    string _ip;
    uint16_t _port;
};