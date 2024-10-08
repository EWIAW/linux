#pragma once
#include <cstring>
#include <cstdio>

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

static const int buffernum = 1024;

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
        // 对于客户端来说，它的ip地址和port端口号需要bind，但是不需要我们主动bind，给操作系统bind即可，因为客户端的端口号我们不能确定
    }

    // 客户端从线程函数 static修饰的成员函数没有this指针
    static void *start_routine(void *args)
    {
        int sockfd = *(int *)args;
        char buffer[buffernum];
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        while (true)
        {
            ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&temp, &len);
            if (n > 0)
            {
                buffer[n] = 0;
            }
            // cout << "服务器返回的结果为：" << endl;
            cout << buffer << endl;
        }
    }

    // 客户端     主线程向服务端发消息，从线程从服务端接收消息
    void start()
    {
        pthread_create(&_tid, nullptr, start_routine, &_sockfd); // 创建从线程接收服务端的消息
        pthread_detach(_tid);                                    // 线程分离

        // 用来存储服务端的ip和port
        struct sockaddr_in server;
        bzero(&server, sizeof(server));
        server.sin_addr.s_addr = inet_addr(_ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(_port);

        char buffer[buffernum];
        while (!_quit)
        {
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strlen(buffer) - 1] = '\0'; // 去除\n
            sendto(_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, sizeof(server));
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
    pthread_t _tid; // 从线程id
};