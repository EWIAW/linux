#pragma once

enum
{
    // 套接字错误
    SOCKET_ERR = 1,
    BIND_ERR,
    LISTEN_ERR,

    // epoll模型错误
    CREATE_EPOLL_ERR,
};