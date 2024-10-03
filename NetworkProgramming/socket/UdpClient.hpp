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
        // 对于客户端来说，它的ip地址和port端口号需要bind，但是不需要我们主动bind，给操作系统bind即可，因为客户端的端口号我们不能确定
    }

    void start()
    {
        struct sockaddr_in server; // 用来存储服务端的ip和port
        bzero(&server, sizeof(server));
        server.sin_addr.s_addr = inet_addr(_ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(_port);

        char buffer[1024];
        while (!_quit)
        {
            string message;
            cin >> message;

            sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server));

            struct sockaddr_in temp;
            socklen_t len = sizeof(temp);
            ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&temp, &len);
            if (n > 0)
            {
                buffer[n] = 0;
            }
            cout << "翻译结果为：" << buffer << endl;
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