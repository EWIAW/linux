#include<stdio.h>
#include<unistd.h>

int main()
{
    pid_t id = fork();//创建子进程
    if(id == 0)//说明是子进程
    {
        //在子进程中进行进程的替换
        char* const  _argv[]={(char*)"./mybin",(char*)NULL};
        execv("./mybin",_argv);//将子进程替换成./mybin，并且也./mybin的方式执行    
    }
    else if(id > 0)//说明是父进程
    {

    }
    sleep(1);
    printf("进程替换结束\n");
    return 0;
}
