#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
    //printf("process running......\n");

    //execl("/usr/bin/ls","ls",NULL);
    //execlp("ls","ls",NULL);
    //char* const argv_[]={"ls","-l"}; 
    

    //execv("/usr/bin/ls",argv_);
    // execvp("ls",argv_);
    
    
    //execl("./mybin","./mybin",NULL);

    //char* const env_[]={"MYPATH=111222333",NULL};
    //execle("./mybin","./mybin",NULL,env_);
    
    printf("父进程running......\n");
    pid_t id = fork();

    if(id==0)//子进程
    {
        char* const env_[]={(char*)"MYPATH=111222333",NULL};
        execle("./mybin","./mybin",NULL,env_);
        exit(1);
    }

    //父进程要记录子进程的退出状态
    int status=0;
    pid_t ret = waitpid(id,&status,0);
    printf("子进程id:%d,子进程退出状态:%d,子进程退出码:%d\n",ret,(status>>8)&0xFF,(status)&0x7F);
    printf("process done!!!\n");
    return 0;
}
