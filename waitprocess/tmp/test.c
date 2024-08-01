#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

typedef void (*pfunc)();//typedef函数指针

pfunc arr[4];//定义函数指针数组

void Task1()//任务1
{
    printf("正在执行任务1\n");
    return;
}

void Task2()//任务2
{
    printf("正在执行任务2\n");
    return;
}

void Task3()//任务3
{
    printf("正在执行任务3\n");
    return;
}

void LoadTask()//加载任务
{
    arr[0]=Task1;
    arr[1]=Task2;
    arr[2]=Task3;
    arr[3]=NULL;
}

int main()
{
    LoadTask();//加载任务
    pid_t id = fork();//创建子进程
    if(id == 0)//说明是子进程
    {
        int cnt1=9;
        while(cnt1--)
        {
            printf("子进程正在运行\n");
            sleep(1);
        }
    }
    else if(id > 0)//说明是父进程 
    {
        int status = 0;
        while(1)
        {
            pid_t ret = waitpid(id,&status,WNOHANG);//WHOHANG代表非阻塞等待
            if(ret == 0)
            {
                //说明等待成功，但是子进程没有退出
                printf("等待子进程成功，子进程没有退出，还在运行\n");
                int i=0;
                for(i=0;i<3;i++)//遍历任务
                {
                    arr[i]();
                }
            }
            else if(ret > 0)
            {
                //说明等待成功，子进程退出了
                printf("等待子进程成功，子进程退出\n");
                break;
            }
            else 
            {
                //说明等待失败
                printf("等待子进程失败\n");
                break;
            }
            sleep(3);
        }
    }
    return 0;
}
