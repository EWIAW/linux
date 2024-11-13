#pragma once
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

class Util
{
public:
    // 将文件描述符设置为非阻塞状态
    static void setNonBlock(int fd)
    {
        int ret = fcntl(fd, F_GETFL); // 获取文件描述符状态
        if (ret < 0)
        {
            cerr << "获取文件描述符状态失败:" << strerror(errno) << endl;
            return;
        }
        fcntl(fd, F_SETFL, ret | O_NONBLOCK);
        return;
    }
};