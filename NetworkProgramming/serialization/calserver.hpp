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

#include "log.hpp"
#include "protocol.hpp"

using namespace std;

typedef function<void(Request &req, Response &res)> func_t;

static const int gbacklog = 5; // 给listen监听函数使用
static const int buffernum = 1024;

// 枚举exit退出码
enum
{
    SOCKET_ERR = 1, // 创建socket失败
    BIND_ERR,       // 绑定socket失败
    LISTEN_ERR,     // 监听失败
};

void HandlerEnter(const int &sockfd, const func_t func)
{
    // inbuffer给readData函数做缓冲区
    std::string inbuffer;
    while (true)
    {
        std::string text; // text用来存放一条完整的请求，如：“content_len”/r/n“x op y”/r/n
        std::string str;  // str用来存放去掉报头后的请求，如：“x op y”
        // 1.读取数据
        bool ret = readData(sockfd, inbuffer, text); // 将网络中的完整的一条请求读到text中
        if (ret == false)
            return;

        // 走到这里，此时text中，是一个完整的报文数据    “content_len”/r/n“x op y”/r/n
        deLength(text, str); // 去掉报头

        // 2.将字符串反序列化
        Request req;
        req.deSerialization(str);

        // 3.处理任务
        Response res;
        func(req, res);

        // 4.将结构化数据序列化
        std::string out;
        res.Serialization(out);

        // 添加报头
        std::string send_str = enLength(out);

        // 5.发送数据
        send(sockfd, send_str.c_str(), send_str.size(), 0);
    }
}

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
        logmessage(NORMAL, "create socket success : listenSockfd:%d", _listenSockfd);

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

    void start(const func_t &func)
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
            logmessage(NORMAL, "accept socket success : sockfd:%d", sockfd);
            // cout << "sock:" << sockfd << endl;

            pid_t pid = fork();
            if (pid == 0)
            {
                // 子进程
                close(_listenSockfd);
                pid_t pid = fork();
                if (pid == 0)
                {
                    // 孙子进程
                    // serviceIO(sockfd);
                    HandlerEnter(sockfd, func);
                    close(sockfd);
                    exit(0);
                }
                exit(0);
            }
            // 父进程
            pid_t ret = waitpid(pid, nullptr, 0);
            close(sockfd); // 关闭不再使用的文件描述符
            if (ret > 0)
            {
                cout << "wait pid success" << endl;
            }
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