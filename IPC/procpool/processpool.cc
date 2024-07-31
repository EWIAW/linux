#include<cassert>
#include<ctime>
#include<cstring>
#include<cstdio>
#include<cstdbool>

#include<iostream>
#include<vector>
#include<string>

#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define PROGRESSNUM 5//子进程个数
#define TaskNum 10//任务次数

//定义一个类存储每个管道的写入提示符
class MessageFd
{
public:
    //构造函数
    MessageFd(int writefd, pid_t pid)
    :_writefd(writefd)
    ,_pid(pid)
    {
        char tmp[1024];
        sprintf(tmp,"detailed information: 我是第 %d 个进程, 我的进程id是: %d, 我的writefd是: %d",num++,_pid,_writefd);
        _information.append(tmp);
    }

    static int num;//记录子进程是第几个
    int _writefd;//写入文件描述符
    pid_t _pid;//子进程id
    std::string _information;//进程的详细信息
};
int MessageFd::num = 0;

//重定义函数指针
typedef void (*pfunc)();

void DownTask()
{
    std::cout<<"下载任务"<<std::endl;
    sleep(1);
}

void IOTask()
{
    std::cout<<"IO任务"<<std::endl;
    sleep(1);
}

void ExecuteTask()
{
    std::cout<<"执行任务"<<std::endl;
    sleep(1);
}

//加载任务到任务表中
void LoadFunc(std::vector<pfunc>& taskmap)
{
    taskmap.push_back(DownTask);
    taskmap.push_back(IOTask);
    taskmap.push_back(ExecuteTask);
}

//接收任务，返回任务吗
int ReceiveTask(int fd)
{
    int taskid = 0;
    ssize_t ret = read(fd,&taskid,sizeof(taskid));
    if(ret == sizeof(taskid))//说明读取成功
    {
        return taskid;
    }
    else if(ret == -1)//说明发生错误
    {
        return -1;
    }
    else
    {
        return -2;
    }
}

//创建子进程
void creatprogress(std::vector<MessageFd>& mfd, std::vector<pfunc>& taskmap)
{
    //循环创建子进程
    for(int i = 0; i < PROGRESSNUM; i++)
    {
        int fd[2];
        int ret = pipe(fd);//创建管道
        assert(ret == 0);

        pid_t id = fork();//创建子进程
        //子进程
        if(id == 0)
        {
            //子进程关闭 写 管道
            close(fd[1]);

            //不仅要关闭本次的 写 管道，还要关闭上一个创建子进程遗留下来的 写 管道
            std::vector<MessageFd>::iterator it = mfd.begin();
            while(it != mfd.end())
            {
                close((*it)._writefd);
                it++;
            }

            //循环读取管道中的内容
            while(true)
            {
                int taskid = ReceiveTask(fd[0]);//接收任务，获取任务码
                if(taskid >= 0)
                {
                    printf("我是子进程, 我的pid是: %d, 我正在执行的任务是：",getpid());
                    taskmap[taskid]();
                }
                else
                    break;
            }
            //printf("第%d个子进程\n",i);
            exit(0);
        }
        MessageFd tmp(fd[1],id);
        mfd.push_back(tmp);

        //sleep(1);

        //父进程关闭 读 管道
        close(fd[0]);

    }
}

void dispatchtash(std::vector<MessageFd>& mfd, std::vector<pfunc>& taskmap)
{
    srand(time(NULL));

    int cnt = TaskNum;//循环执行任务的次数
    while(cnt--)
    {
        //1.选一个子进程
        int fd = rand() % PROGRESSNUM;

        //2.选一个任务
        int taskid = rand() % taskmap.size();

        //3.发送任务
        write(mfd[fd]._writefd,&taskid,sizeof(taskid));//向管道写入内容

        sleep(1);
    }
    //所有任务派发成功后，关闭所有管道
    for(int i = 0; i < PROGRESSNUM; i++)
    {
        close(mfd[i]._writefd);
    }
}

//等待子进程退出
void WaitProcess(std::vector<MessageFd>& mfd)
{
    for(int i = 0; i < PROGRESSNUM; i++)
    {
        waitpid(mfd[i]._pid,NULL,0);
        printf("等待子进程成功,子进程id是%d\n",mfd[i]._pid);
    }
}

int main()
{

    //1.加载任务到任务表中
    std::vector<pfunc> taskmap;//任务表
    LoadFunc(taskmap);

    //2.创建子进程，每个子进程都对应一个管道
    std::vector<MessageFd> mfd;//进程信息表
    creatprogress(mfd,taskmap);

    //3.给五个子进程派发任务
    dispatchtash(mfd,taskmap);

    //4.进程等待，关闭子进程
    WaitProcess(mfd);

    return 0;
}