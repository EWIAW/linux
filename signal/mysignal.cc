#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

void state()
{
    cout<<"请正确输入命令: 程序 数字 pid"<<endl;
}


// ./mysignal -9 pid
int main(int argc,char* argv[])
{
    //模拟kill命令
    // if(argc != 3)
    // {
    //     state();
    // }
    // else
    // {
    //     int pid = atoi(argv[2]);
    //     int sign = atoi(argv[1]);
    //     int ret = kill(pid,sign);
    // }

    while(true)
    {
        cout<<"hello linux: "<<getpid()<<endl;
        sleep(1);
        abort();
    }
    return 0;
}