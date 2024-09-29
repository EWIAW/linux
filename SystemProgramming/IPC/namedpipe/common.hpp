#pragma once

#include <cerrno>
#include <cstring>
#include <cassert>

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NAMED_PIPE "named_pipe"

//创建命名管道
bool CreatPipe()
{
    umask(0);
    int ret = mkfifo(NAMED_PIPE,0666);
    if(ret == 0)
        return true;
    else if(ret == -1)
    {
        std::cout<<"errno: "<<errno<<" "<<strerror(errno)<<std::endl;
        return false;
    }
}

//删除命名管道
bool RemotePipe()
{
    int ret = unlink(NAMED_PIPE);
    if(ret == 0)
        return true;
    else if(ret == -1)
    {
        std::cout<<"errno: "<<errno<<" "<<strerror(errno)<<std::endl;
        return false;
    }
}