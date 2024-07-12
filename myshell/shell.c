#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define NUM 100

int main()
{
    char USER[NUM];//存储用户名数组
    char HOSTNAME[NUM];//存储主机名数组
    char PWD[NUM];//存储当前路径数组

    char* ret1 = getenv("USER");//获取用户名
    char* ret2 = getenv("HOSTNAME");//获取主机名
    char* ret3 = getenv("PWD");//获取当前路径
    strcpy(USER,ret1);//将用户名拷贝给USER数组
    strcpy(HOSTNAME,ret2);//将主机名拷贝给HOSTNAME数组
    strcpy(PWD,ret3);//将当前路径拷贝给PWD数组

    //分离当前路径，将当前路径的最后一个名字取出来
    int pos = 0;
    int length = strlen(ret3);
    for(int i=length;i>=0;i--)
    {
        if(PWD[i]=='/')
        {
            pos=i+1;
            break;
        }
    }

    char pwd[NUM];
    int i=0;
    while(1)
    {
        if(pos <= length)
        {
            pwd[i]=PWD[pos];
            i++;
            pos++;
        }
        else 
        {
            break;
        }
    }

    while(1)
    {
        //输出
        printf("我的命令行解释器：[%s@%s %s]",USER,HOSTNAME,pwd);
        fflush(stdout);
        char linemanned[NUM];
        char* in = fgets(linemanned,sizeof(linemanned)-1,stdin);
        linemanned[strlen(linemanned)-1] = 0;//去掉\n
        
        //进行输入字符串的切割
        char* arr[NUM];
        int i=0;
        arr[i]=strtok(linemanned," ");
        i++;
        while(arr[i-1]!=NULL)
        {
            arr[i]=strtok(NULL," ");
            i++;
        }
        arr[i]=NULL;

        int id = fork();
        if(id == 0)//说明是子进程
        {
            execvp(arr[0],arr);
        }
        printf("\n");

        int ret = waitpid(id,NULL,0);
    }
    return 0;
}
