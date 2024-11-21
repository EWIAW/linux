#pragma once
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

class Util
{
public:
    // 将文件描述符设置为非阻塞状态
    static void setNonBlock(int fd)
    {
        int ret = fcntl(fd, F_GETFL); // 获取文件描述符状态
        if (ret < 0)
        {
            std::cerr << "获取文件描述符状态失败:" << strerror(errno) << std::endl;
            return;
        }
        fcntl(fd, F_SETFL, ret | O_NONBLOCK);
        return;
    }
};