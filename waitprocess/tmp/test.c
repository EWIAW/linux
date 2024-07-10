#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t id=fork();//创建子进程
    
    if(id==0)//说明是子进程
    {
        int cnt=5;
        while(cnt--)
        {
            printf("我是子进程，我的pid是：%d，我的ppid是：%d\n",getpid(),getppid());
            sleep(1);
        }
        exit(10);//进程退出
    }
    else if(id>0)//说明是父进程
    {
        int status=0;
        pid_t ret=waitpid(id,&status,0);//0就是阻塞等待的意思
        printf("父进程等待成功，等待进程的id是：%d，其退出状态是：%d，收到的信号是：%d\n",ret,(status>>8)&0xff,status&0x7f);
    }

    return 0;
}
