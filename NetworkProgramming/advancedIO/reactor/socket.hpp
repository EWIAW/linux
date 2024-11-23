#pragma once

#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "err.hpp"
#include "log.hpp"
#include "util.hpp"

class Sock
{
    const static int blognum = 32;
    const static int default_num = -1;
    const static uint16_t default_port = 3489;

public:
    Sock()
        : _listen_socket(default_num)
    {
    }

    ~Sock()
    {
        if (_listen_socket != default_num)
            close(_listen_socket);
    }

    // 1.创建socket
    void Socket()
    {
        _listen_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_listen_socket < 0)
        {
            Log_Message(FATAL, "create socket failed!!! , reason : %s", strerror(errno));
            exit(SOCKET_ERR);
        }
        Log_Message(DUBUG, "create socket success!!!");
        int opt = 1;
        setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)); // 使得可以立即重启服务器
        Util::setNonBlock(_listen_socket);                                                      // 将套接字设置为非阻塞状态
    }

    // 2.绑定socket
    void Bind(const uint16_t port = default_port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        int ret = bind(_listen_socket, (struct sockaddr *)&local, sizeof(local));
        if (ret < 0)
        {
            Log_Message(FATAL, "bind socket failed!!! , reson : %s", strerror(errno));
            exit(BIND_ERR);
        }
        Log_Message(DUBUG, "bind socket success");
        return;
    }

    // 3.开始监听
    void Listen()
    {
        int ret = listen(_listen_socket, blognum);
        if (ret < 0)
        {
            Log_Message(FATAL, "listen socket failed!!! , reason : %s", strerror(errno));
            exit(LISTEN_ERR);
        }
        Log_Message(DUBUG, "listen socket success");
        return;
    }

    // 接收新连接
    int Accept(std::string *clientip, uint16_t *clientport, int *err)
    {
        struct sockaddr_in client;
        memset(&client, 0, sizeof(client));
        socklen_t len = sizeof(client);
        int fd = accept(_listen_socket, (struct sockaddr *)&client, &len);
        if (fd < 0)
        {
            Log_Message(ERROR, "accept socket failed!!! , reason : %s", strerror(errno));
            *err = errno; // 将错误信息带回
        }
        else
        {
            *clientip = inet_ntoa(client.sin_addr);
            *clientport = ntohs(client.sin_port);
            Log_Message(DUBUG, "accept socket success!!! , get new socket : %d , clientip : %s , clientport : %d", fd, clientip->c_str(), *clientport);
            Util::setNonBlock(_listen_socket); // 将套接字设置为非阻塞状态
        }
        return fd;
    }

    int FD()
    {
        return _listen_socket;
    }

private:
    int _listen_socket;
};