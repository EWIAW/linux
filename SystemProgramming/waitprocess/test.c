#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#define NUM 10
void Task1()
{
    printf("running Task1\n");
    return;
}

void Task2()
{
    printf("running Task2\n");
    return;
}

void Task3()
{
    printf("running Task3\n");
    return;
}

typedef void(*pfun)();//重定义函数指针


pfun arr[NUM];

void LoadTask()//加载任务到函数指针数组中，以便给父进程执行
{
    memset(arr,0,sizeof(arr));
    arr[0]=Task1;
    arr[1]=Task2;
    arr[2]=Task3;   
}

int main()
{
    pid_t id = fork();
    if(id==0)//说明是子进程
    {
        int cnt=5;
        while(cnt--)
        {
            //printf("我是子进程，我的pid是：%d，我的ppid：%d\n",getpid(),getppid());
            sleep(1);
        }
        //int* p;
        //*p=100;
        exit(10);
    }
    
    
    //下面代码写的是阻塞等待
    //if(id>0)//说明是父进程
    //{
    //    int status = 0;
    //    pid_t ret = waitpid(id,&status,0);
    //    //printf("子进程的id是：%d， 子进程的退出码是：%d，子进程收到的信号是：%d\n",ret,(status>>8)&0xFF,status&0x7F);
    //    //WIFEXITED     如果子进程正常退出，则返回真
    //    //WEXITSTATUS   提取子进程退出码
    //    //printf("子进程的退出码：%d\n",WEXITSTATUS(status));
    //    if(WIFEXITED(status))
    //    {
    //        printf("子进程正常退出\n");
    //    }
    //    else 
    //    {
    //        printf("子进程异常退出，子进程的id：%d，子进程的退出码：%d\n",id,WEXITSTATUS(status));
    //    }
    //}
    


    //下面代码写的是非阻塞等待
    //说明是父进程
    
    LoadTask();//导入任务

    int status=0;
    while(1)
    {
        pid_t ret =  waitpid(id,&status,WNOHANG);//非阻塞等待
        if(ret==0)//父进程等待成功，子进程没有退出
        {
            //在子进程没有退出期间，父进程可以执行其他任务；
            printf("wait done, but child running......\n");
            for(int i=0;arr[i]!=NULL;i++)
            {
                arr[i]();
            }
        }
        else if(ret>0)//父进程等待成功，子进程退出
        {
            printf("等待成功，子进程的退出码为：%d，子进程收到的信号为：%d\n",(status>>8)&0xFF,status&0x7F);
            break;
        }
        else//父进程等待错误，例如子进程id传错 
        {
            printf("wait false\n");
            break;
        }
        sleep(1);
    }
    
    return 0;
}
