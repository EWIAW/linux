#include <cstring>

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const string DefaultIp = "0.0.0.0"; // 默认ip地址

// 枚举进程退出码
enum
{
    SOCKET_ERR = 1,
    BIND_ERR,
};

// 编写udp服务端
class UdpServer
{
public:
    UdpServer(const uint16_t &port, const string &ip = DefaultIp)
        : _ip(ip), _port(port)
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
        bzero(&local, sizeof(local));
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());
        local.sin_addr.s_addr = htonl(INADDR_ANY);
        local.sin_family = AF_INET;
        local.sin_port = _port;

        int ret = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (ret == -1)
        {
            cerr << "socket bind:" << errno << ":" << strerror(errno) << endl;
            exit(BIND_ERR);
        }
    }

    // 服务端启动
    void start()
    {
        for (;;)
        {
        }
    }

    ~UdpServer()
    {
    }

private:
    int _sockfd;    // 创建一个套接字后，会返回一个文件描述符
    string _ip;     // 服务端的ip地址
    uint16_t _port; // 服务端的端口号
};