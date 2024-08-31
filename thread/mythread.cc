#include <iostream>
#include <vector>

#include <pthread.h>
#include <unistd.h>

using namespace std;

#define NUM 10

int g_val = 0;

//线程类
class Thread
{
public:
    int number;
    pthread_t tid;
    char namebuffer[64];
};

string func()
{
    const char* str = "hello linux";
    return str;
}

//新线程运行的函数
void* thread(void* newThread)
{
    Thread* tmp = (Thread*)newThread;
    int cnt = 5;
    while(cnt--)
    {
        cout<<"我是新线程:"<<((Thread*)newThread)->namebuffer<<endl;
        sleep(1);
    }
    pthread_exit((void*)tmp->number);
    // return (void*)tmp->number;
    // return (void*)100;
}

int main()
{
    vector<Thread*> vtid;
    for(int i = 0; i < NUM; i++)//创建一大批线程
    {
        Thread* newThread = new Thread();
        newThread->number = i;
        snprintf(newThread->namebuffer,sizeof(newThread->namebuffer),"线程:%d",i);
        pthread_create(&newThread->tid,nullptr,thread,newThread);
        vtid.push_back(newThread);
    }

    // for(int i = 0; i < NUM/2; i++)
    // {
    //     pthread_cancel(vtid[i]->tid);
    //     sleep(1);
    // }

    //等待线程退出
    for(auto& thread : vtid)
    {
        void* ret = nullptr;
        pthread_join(thread->tid,&ret);
        cout<<(long long)ret<<endl;
        delete thread;
    }

    // for(int i = 0; i < NUM; i++)
    // {
    //     pthread_t tid;
    //     char namebuffer[64];
    //     snprintf(namebuffer,sizeof(namebuffer),"thread:%d",i);
    //     pthread_create(&tid,nullptr,thread,namebuffer);
    //     sleep(1);
    // }

    // pthread_t tid;
    // pthread_create(&tid,nullptr,thread,(void*)"thread one");//创建新线程

    //主线程
    while(true)
    {
        cout<<"我是主线程:"<<endl;
        sleep(1);
    }

    return 0;
}