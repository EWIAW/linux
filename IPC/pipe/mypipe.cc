#include<iostream>

#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#include<cassert>
#include<cstdlib>
#include<cstdio>
#include<cstring>

using namespace std;

int main()
{
    int fds[2] = {0,0};
    int ret = pipe(fds);//其中fds[0]为读，fds[1]为写

    pid_t id = fork();
    assert(id >= 0);

    //我想做的是，父进程读取管道，子进程写入管道

    //子进程
    if(id == 0)
    {
        //子进程关闭读取管道
        close(fds[0]);

        char buffer[1024];//缓冲区
        int cnt = 0;//计数器
        const char * str = "hello linux";
        while(1)
        {
            char buffer[1024];//缓冲区
            sprintf(buffer,"我是父进程，我在往管道写入数据：%s:%d",str,cnt++);//将内容写入缓冲区中
            write(fds[1],buffer,strlen(buffer));//将缓冲区中的数据写入到管道中
            sleep(1);
        }

        close(fds[1]);
        exit(0);
    }
    //父进程

    //父进程关闭写入管道
    close(fds[1]);

    char buffer[1024];//缓冲区
    int cnt = 5;
    while(cnt--)
    {
        int sz = read(fds[0],buffer,sizeof(buffer)-1);//sz为返回读到几个字节
        buffer[sz] = '\0';//按C语言的标准给末尾添加\0
        cout<<buffer<<endl;
        sleep(1);
    }
    close(fds[0]);
    int status = 0;//存储子进程的退出信息
    waitpid(id,&status,0);//阻塞等待子进程
    cout<<(status&0x7F)<<endl;  

    return 0;
}