#pragma once

#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "err.hpp"
#include "log.hpp"

class Sock
{
    const static int blognum = 32;

public:
    // 1.创建socket
    static int Socket()
    {
        int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_socket < 0)
        {
            Log_Message(FATAL, "create socket failed!!! , reason : %s", strerror(errno));
            exit(SOCKET_ERR);
        }
        Log_Message(DUBUG, "create socket success!!!");
        int opt = 1;
        setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)); // 使得可以立即重启服务器
        return listen_socket;
    }

    // 2.绑定socket
    static void Bind(int &listen_socket, uint16_t &port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        int ret = bind(listen_socket, (struct sockaddr *)&local, sizeof(local));
        if (ret < 0)
        {
            Log_Message(FATAL, "bind socket failed!!! , reson : %s", strerror(errno));
            exit(BIND_ERR);
        }
        Log_Message(DUBUG, "bind socket success");
        return;
    }

    // 3.开始监听
    static void Listen(int &listen_socket)
    {
        int ret = listen(listen_socket, blognum);
        if (ret < 0)
        {
            Log_Message(FATAL, "listen socket failed!!! , reason : %s", strerror(errno));
            exit(LISTEN_ERR);
        }
        Log_Message(DUBUG, "listen socket success");
        return;
    }

    // 接收新连接
    static int Accept(int &listen_socket, std::string *clientip, uint16_t *clientport)
    {
        struct sockaddr_in client;
        memset(&client, 0, sizeof(client));
        socklen_t len = sizeof(client);
        int fd = accept(listen_socket, (struct sockaddr *)&client, &len);
        if (fd < 0)
        {
            Log_Message(ERROR, "accept socket failed!!! , reason : %s", strerror(errno));
        }
        else
        {
            *clientip = inet_ntoa(client.sin_addr);
            *clientport = ntohs(client.sin_port);
            Log_Message(DUBUG, "accept socket success!!! , get new socket : %d , clientip : %s , clientport : %d", fd, clientip->c_str(), *clientport);
        }
        return fd;
    }

private:
};