#pragma once

#include <cstdio>
#include <ctime>

#include <iostream>
#include <string>

static const int buffernum1 = 1024;

typedef int (*func_t)(int, int, char);

// 计算任务
class Task
{
public:
    Task(const int sockfd)
        : _sockfd(sockfd)
    {
    }

    void operator()()
    {
        serviceio(_sockfd);
    }

    void serviceio(const int &sockfd)
    {
        char buffer[buffernum1];
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
        close(sockfd);
    }

    ~Task()
    {
    }

private:
    int _sockfd;
};
