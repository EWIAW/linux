// #include <iostream>
// #include <vector>

// #include <pthread.h>
// #include <unistd.h>

// using namespace std;

// #define NUM 10

// int g_val = 0;

// //线程类
// class Thread
// {
// public:
//     int number;
//     pthread_t tid;
//     char namebuffer[64];
// };

// string func()
// {
//     const char* str = "hello linux";
//     return str;
// }

// //新线程运行的函数
// void* thread(void* newThread)
// {
//     Thread* tmp = (Thread*)newThread;
//     int cnt = 5;
//     while(cnt--)
//     {
//         cout<<"我是新线程:"<<((Thread*)newThread)->namebuffer<<endl;
//         sleep(1);
//     }
//     pthread_exit((void*)tmp->number);
//     // return (void*)tmp->number;
//     // return (void*)100;
// }

// int main()
// {
//     vector<Thread*> vtid;
//     for(int i = 0; i < NUM; i++)//创建一大批线程
//     {
//         Thread* newThread = new Thread();
//         newThread->number = i;
//         snprintf(newThread->namebuffer,sizeof(newThread->namebuffer),"线程:%d",i);
//         pthread_create(&newThread->tid,nullptr,thread,newThread);
//         vtid.push_back(newThread);
//     }

//     // for(int i = 0; i < NUM/2; i++)
//     // {
//     //     pthread_cancel(vtid[i]->tid);
//     //     sleep(1);
//     // }

//     //等待线程退出
//     for(auto& thread : vtid)
//     {
//         void* ret = nullptr;
//         pthread_join(thread->tid,&ret);
//         cout<<(long long)ret<<endl;
//         delete thread;
//     }

//     // for(int i = 0; i < NUM; i++)
//     // {
//     //     pthread_t tid;
//     //     char namebuffer[64];
//     //     snprintf(namebuffer,sizeof(namebuffer),"thread:%d",i);
//     //     pthread_create(&tid,nullptr,thread,namebuffer);
//     //     sleep(1);
//     // }

//     // pthread_t tid;
//     // pthread_create(&tid,nullptr,thread,(void*)"thread one");//创建新线程

//     //主线程
//     while(true)
//     {
//         cout<<"我是主线程:"<<endl;
//         sleep(1);
//     }

//     return 0;
// }

#include <stdio.h>

#include <iostream>
#include <vector>

#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include "Thread.hpp" 

#define NUM 10

int ticket = 10000;

class thread
{
public:
    pthread_t tid;//线程id
    char namebuffer[64];//线程参数
    int number;//现在序号
};

void* start_routine(void* args)
{
    int cnt = 5;
    while(cnt--)
    {
        std::cout<<"我是新"<<((thread*)args)->namebuffer<<std::endl;
        sleep(1);
    }
    // return (void*)100;
    pthread_exit((void*)100);
}

void test1()
{
    std::vector<thread*> tv;

    //创建一批线程
    for(int i = 0; i < NUM; i++)
    {
        thread* newThread = new thread();
        newThread->number = i+1;
        snprintf(newThread->namebuffer,sizeof(newThread->namebuffer),"线程%d",i+1);
        int ret = pthread_create(&newThread->tid,nullptr,start_routine,newThread);
        assert(ret == 0);
        tv.push_back(newThread);
    }

    int cnt = 1;
    while(cnt--)
    {
        std::cout<<"**********我是主线程**********"<<std::endl;
        sleep(1);
    }

    //取消一批线程
    for(int i = 0; i < NUM/4; i++)
    {
        pthread_cancel(tv[i]->tid);//取消线程，即让线程退出
    }

    //分离一批线程
    for(int i = 2; i < NUM/2; i++)
    {
        pthread_detach(tv[i]->tid);//线程一旦分离后，就不能在join等待
    }

    //线程等待
    for(auto& tmp : tv)
    {
        void* ret = nullptr;
        int n = pthread_join(tmp->tid,&ret);
        // assert(n == 0);
        std::cout<<"线程"<<tmp->number<<"已退出"<<std::endl;
        // std::cout<<(long long)ret<<std::endl;
        delete tmp;//线程退出后要记得是否Thread对象空间
    }
}

void* run_routine(void* args)
{
    int cnt = 5;
    while(cnt--)
    {
        std::cout<<"我是一个新线程，我正在做"<<(char*)args<<std::endl;
        sleep(1);
    }
}

void* get_ticket(void* args)
{
    while(true)
    {
        if (ticket > 0)
        {
            usleep(1000);
            std::cout << (char *)args << ":正在抢票:" << ticket-- << std::endl;
        }
        else
        {
            break;
        }
    }
}

int main()
{
    Thread t1(get_ticket,(void*)"user1",1);
    Thread t2(get_ticket,(void*)"user2",2);
    Thread t3(get_ticket,(void*)"user3",3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}