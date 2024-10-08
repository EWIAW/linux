#pragma once
#include <cstring>

#include <iostream>
#include <string>
#include <functional>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

typedef function<void(int, string, uint16_t, string)> func;

static const string DefaultIp = "0.0.0.0"; // 默认ip地址
static const int buffersize = 1024;        // 缓冲区的大小

// 枚举进程退出码
enum
{
    SOCKET_ERR = 1,
    BIND_ERR,
    OPEN_ERR,
    POPEN_ERR,
    PCLOSE_ERR,
};

// 编写udp服务端
class UdpServer
{
public:
    UdpServer(const func &callback, const uint16_t &port, const string &ip = DefaultIp)
        : _callback(callback), _sockfd(-1), _ip(ip), _port(port)
    {
    }

    // 服务端初始化
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
        bzero(&local, sizeof(local));                   // 对local变量全部置0
        local.sin_addr.s_addr = inet_addr(_ip.c_str()); // 指定其他client只能往这个ip发送信息
        local.sin_addr.s_addr = htonl(INADDR_ANY);      // 指定其他client能往这个服务端上的任何ip发送信息，服务器的真实写法
        local.sin_family = AF_INET;                     // 说明是网络通信
        local.sin_port = htons(_port);                  // 输入端口号

        int ret = bind(_sockfd, (struct sockaddr *)&local, sizeof(local)); // 绑定套接字
        if (ret == -1)
        {
            cerr << "socket bind:" << errno << ":" << strerror(errno) << endl;
            exit(BIND_ERR);
        }
        // UDP预备工作完成
    }

    // 服务端启动
    void start()
    {
        char buffer[buffersize];
        for (;;)
        {
            // 1.接收客户端传来的信息
            struct sockaddr_in peer; // 用来存储客户端的信息ip和port
            socklen_t len = sizeof(peer);
            ssize_t s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len); // 接受客户端的消息
            if (s > 0)
            {
                buffer[s] = 0;                              // 因为接收数据是以C语言字符串的形式接收，所以要将末尾添加/0
                string clientip = inet_ntoa(peer.sin_addr); // 获取客户端ip
                uint16_t clientport = ntohs(peer.sin_port); // 获取客户端port
                string message = buffer;                    // 获取客户端传来的信息
                cout << clientip << "[" << clientport << "]" << message << endl;

                // 2.处理客户端传过来的信息
                _callback(_sockfd, clientip, clientport, message);
            }
        }
    }

    ~UdpServer()
    {
    }

private:
    int _sockfd;    // 创建一个套接字后，会返回一个文件描述符
    string _ip;     // 服务端的ip地址
    uint16_t _port; // 服务端的端口号
    func _callback; // 回调方法，用来接收客户端的信息后进行处理
};