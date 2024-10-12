#pragma once

#include <cstring>

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include "log.hpp"
#include "./ThreadPool/ThreadPool.hpp"
#include "./ThreadPool/task.hpp"

using namespace std;

// #define VERSION1 // 最初单进程版
// #define VERSION2 // 多进程版
// #define VERSION3 // 多线程版
#define VERSION4 // 线程池版

static const int gbacklog = 5; // 给listen监听函数使用
static const int buffernum = 1024;

// 枚举错误类型
enum
{
    SOCKET_ERR = 1, // 创建socket失败
    BIND_ERR,       // 绑定socket失败
    LISTEN_ERR,     // 监听失败
};

// 声明
class tcpserver;

class ThreadData
{
public:
    ThreadData(tcpserver *ts, const int &sockfd)
        : _ts(ts), _sockfd(sockfd)
    {
    }

public:
    tcpserver *_ts;
    int _sockfd;
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

    void start()
    {
        myThreadPool::ThreadPool<Task>::GetInstance()->run(); // 启动线程池
        logmessage(NORMAL, "ThreadPool init success");
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
            logmessage(NORMAL, "accept socket success : sockfd:%d");
            // cout << "sock:" << sockfd << endl;

#ifdef VERSION1                // 单进程版，只能有一个客户端进行通信
            serviceIO(sockfd); // 连接成功则进行IO通信

            // 一个客户端的连接结束后，需要关闭文件描述符
            close(sockfd);
#endif

#ifdef VERSION2 // 多进程版，让孙子进程去解决通信的问题，所以父进程就可以阻塞的等待子进程退出
            pid_t pid = fork();
            if (pid == 0)
            {
                // 子进程
                close(_listenSockfd);
                pid_t pid = fork();
                if (pid == 0)
                {
                    // 孙子进程
                    serviceIO(sockfd);
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
#endif

#ifdef VERSION3 // 多线程版
            pthread_t tid;
            ThreadData *td = new ThreadData(this, sockfd);
            pthread_create(&tid, nullptr, start_routine, td);
#endif

#ifdef VERSION4 // 线程池版
            Task task(sockfd);
            myThreadPool::ThreadPool<Task>::GetInstance()->push(task);
#endif
        }
    }

    ~tcpserver()
    {
    }

private:
    static void *start_routine(void *args)
    {
        pthread_detach(pthread_self()); // 线程分离
        ThreadData *td = (ThreadData *)args;
        td->_ts->serviceIO(td->_sockfd);
        close(td->_sockfd); // 关闭文件描述符
        delete td;
        return nullptr;
    }

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