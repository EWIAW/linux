#pragma once
#include <cstring>

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// 枚举进程退出码
enum
{
    SOCKET_ERR = 1,
    BIND_ERR,
};

class UdpClient
{
public:
    UdpClient(const string &ip, const uint16_t &port)
        : _ip(ip), _port(port), _quit(false)
    {
    }

    // 客户端初始化
    void init()
    {
        // 1.创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd == -1)
        {
            cerr << "socket error:" << errno << ":" << strerror(errno) << endl;
            exit(SOCKET_ERR);
        }
        cout << "socket success:" << _sockfd << endl;
        // 2.bing套接字
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_addr.s_addr = inet_addr(_ip.c_str());
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
    }

    void start()
    {
        struct sockaddr_in server;
        bzero(&server, sizeof(server));
        server.sin_addr.s_addr = inet_addr(_ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(_port);

        while (!_quit)
        {
            string message;
            cin >> message;

            sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server));
        }
    }

    ~UdpClient()
    {
    }

private:
    int _sockfd;    // 文件描述符
    string _ip;     // ip地址
    uint16_t _port; // 端口号
    bool _quit;
};