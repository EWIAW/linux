#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

#define SIGNAL 2

// void state()
// {
//     cout<<"请正确输入命令: 程序 数字 pid"<<endl;
// }

//void CatchSign(int sign)
//{
//    cout<<"我捕捉了"<<sign<<"号信号"<<endl;
//}

void ShowPending(sigset_t& pending)
{
    for(int i = 31; i>0; i--)
    {
        if(sigismember(&pending,i))
        {
            cout<<"1";
        }
        else
        {
            cout<<"0";
        }
    }
    cout<<endl;
}

// ./mysignal -9 pid
int main(int argc,char* argv[])
{
    sigset_t block,oblock,pending;
    
    sigemptyset(&block);
    sigemptyset(&oblock);
    sigemptyset(&pending);

    sigaddset(&block,SIGNAL);

    sigprocmask(SIG_SETMASK,&block,&oblock);

    while(true)
    {
        sigemptyset(&pending);
        sigpending(&pending);
        ShowPending(pending);
        sleep(1);
    }

    //int a = 100;
    //int b = 0;
    //int c = 100/0;

    // for(int i = 1; i<=31 ;i++)
    // {
    //     signal(i,CatchSign);
    // }

    // while(true)
    // {
    //     cout<<"hello linux: "<<getpid()<<endl;
    //     sleep(1);
    // }

    // alarm(1);//设定一个闹钟，一秒后结束

    // while(true)
    // {
    //     ;
    // }

    // int a = 100;
    // int b = 0;
    // int c = a/b;

    // 模拟kill命令
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

    // while(true)
    // {
    //     cout<<"hello linux: "<<getpid()<<endl;
    //     sleep(1);
    //     // raise(9);
    //     abort();
    // }
    return 0;
}
