#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define NUM 100

char linemanned[NUM];//定义数组存储我们输入的命令
char* arr[NUM];//定义一个指针数组存储分割后的字符串

int lastcode = 0;//存储退出码
int lastsign = 0;//存储信号
int status = 0;//存储子进程的退出信息

int main()
{
    while(1)
    {
        //输出提示符
        printf("[用户名@主机名 当前路径]");
        fflush(stdout);//刷新缓冲区

        //将我们在控制台输入的命令  输入到  linemanned数组中
        char* in = fgets(linemanned,sizeof(linemanned)-1,stdin);
        linemanned[strlen(linemanned)-1] = 0;//去掉末尾\n
        
        //ls -a -l  假设我们输入的命令是这样的，所以要进行字符串分割
        int i=0;
        arr[i++]=strtok(linemanned," ");//使用strtok函数来完成分割
        
        if(strcmp(arr[0],"ls") == 0)//如果此时的命令是ls命令，则我们给它带上颜色显示
        {
            arr[i++]="--color=auto";
        }

        //继续进行字符串分割
        while(arr[i-1]!=NULL)
        {
            arr[i++]=strtok(NULL," ");
        }
        arr[i]=NULL;

        //进行判断是否cd命令
        if(arr[0]!=NULL && strcmp(arr[0],"cd") == 0)
        {
            if(arr[1]!=NULL)
            {
                chdir(arr[1]);
            }
            continue;
        }
        
        //创建子进程执行我们的命令
        int id = fork();
        if(id == 0)//说明是子进程
        {
            execvp(arr[0],arr);
            exit(1);//如果子进程创建失败，则用exit退出，且退出码为1
        }
        
        //子进程运行完成后，要进行一个进程的等待来获取子进程的运行结果
        int ret = waitpid(id,&status,0);
        lastcode = (status>>8) & 0xff;
        lastsign = status & 0x7f;
    }
    return 0;
}
