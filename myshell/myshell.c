#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define NUM 100

#define NONE_REDIR   0//没有重定向
#define OUTPUT_REDIR 1//输出重定向
#define APPEND_REDIR 2//追加重定向
#define INPUT_REDIR  3//输入重定向

char linemanned[NUM];//定义数组存储我们输入的命令
char* arr[NUM];//定义一个指针数组存储分割后的字符串

int lastcode = 0;//存储退出码
int lastsign = 0;//存储信号
int status   = 0;//存储子进程的退出信息

int REDIR    = NONE_REDIR;
char* start2 = NULL;//如果出现重定向，则指向后半段内容

//ls -a -l >  log.txt
//ls -a -l >> log.txt
//ls -a -l <  log.txt
//分离函数，将命令分为左右两部分
void linemannedCheck(char* linemanned)
{
   int begin = 0;
   int end = strlen(linemanned);
   while(begin < end)
   {
       if(linemanned[begin] == '>')
       {
            linemanned[begin] = '\0';
            begin++;
            if(linemanned[begin] == '>')
            {
                begin++;
                //下面这个while循环用来跳过空格
                while(linemanned[begin] == ' ')
                {
                    begin++;
                }
                start2 = &linemanned[begin];
                REDIR = APPEND_REDIR;
                break;
            }
            //跳过空格
            while(linemanned[begin] == ' ')
            {
                begin++;
            }
            start2 = &linemanned[begin];
            REDIR = OUTPUT_REDIR;
            break;
       }
       else if(linemanned[begin] == '<')
       {
            linemanned[begin] = '\0';
            begin++;
            //跳过空格
            while(linemanned[begin] == ' ')
            {
                begin++;
            }
            start2 = &linemanned[begin];
            REDIR = INPUT_REDIR;
            break;
       }
       else 
       {
           begin++;
       }
   }
}

int main()
{
    while(1)
    {
        REDIR = NONE_REDIR;
        start2 = NULL;

        //输出提示符
        printf("[用户名@主机名 当前路径]");
        fflush(stdout);//刷新缓冲区

        //将我们在控制台输入的命令  输入到  linemanned数组中
        char* in = fgets(linemanned,sizeof(linemanned)-1,stdin);
        linemanned[strlen(linemanned)-1] = 0;//去掉末尾\n
        
        linemannedCheck(linemanned);//对输入的命令进行解释，判断是否需要重定向

        //ls -a -l  假设我们输入的命令是这样的，所以要进行字符串分割
        int i=0;
        arr[i++]=strtok(linemanned," ");//使用strtok函数来完成分割
        
        //if(strcmp(arr[0],"ls") == 0)//如果此时的命令是ls命令，则我们给它带上颜色显示
        //{
        //    arr[i++]="--color=auto";
        //}

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
            switch(REDIR)
            {
                case NONE_REDIR: 
                    break;
                case OUTPUT_REDIR:
                case APPEND_REDIR:
                    {
                        int flags = O_WRONLY | O_CREAT; 
                        if(REDIR == OUTPUT_REDIR)
                        {
                            flags |= O_TRUNC;
                        }
                        else 
                        {
                            flags |= O_APPEND;
                        }
                        int fd = open(start2,flags,0666);
                        dup2(fd,1);
                    }
                    break;
                case INPUT_REDIR:
                    {
                        int flags = O_RDONLY;   
                        int fd =open(start2,flags);
                        dup2(fd,0);
                    }
                    break;
            }
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
