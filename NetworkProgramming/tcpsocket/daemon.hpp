#pragma once

#include <cstdlib>
#include <cassert>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEV "/dev/null"

// 将进程守护进程化
void daemonSelf()
{
    // 1. 让调用进程忽略掉异常的信号
    signal(SIGPIPE, SIG_IGN);

    // 2. 如何让自己不是组长，setsid
    pid_t pid = fork();
    if (pid > 0)
    {
        exit(0);
    }

    // 子进程 -- 守护进程，精灵进程，本质就是孤儿进程的一种！
    pid_t n = setsid();
    assert(n != -1);

    // 3. 守护进程是脱离终端的，关闭或者重定向以前进程默认打开的文件
    int fd = open(DEV, O_RDWR);
    if (fd >= 0)
    {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);

        // 上面三个dup2操作完成后，0，1，2文件描述符均往文件黑洞里写，所以可以将fd关掉
        close(fd);
    }
    else
    {
        close(0);
        close(1);
        close(2);
    }
}