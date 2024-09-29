#include <stdio.h>

#include <iostream>
#include <vector>

#include <unistd.h>

#define NUM 4

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int ticket = 1000;

void* start_routine(void* args)
{
    while(true)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond,&lock);

        std::cout<<(char*)args<<"正在抢票:"<<ticket<<std::endl;
        ticket--;
        
        pthread_mutex_unlock(&lock);
        // sleep(1);
    }
}

int main()
{
    std::vector<pthread_t> vtid;
    //创建一批线程
    for(int i = 0; i < NUM; i++)
    {
        char* name = new char[64];
        snprintf(name,sizeof(name),"thread%d",i+1);
        pthread_t tmp;
        pthread_create(&tmp,nullptr,start_routine,(void*)name);
        vtid.push_back(tmp);
    }

    while(true)
    {
        // pthread_cond_signal(&cond);
        pthread_cond_broadcast(&cond);
        // std::cout<<"主线程唤醒一个线程"<<std::endl;
        sleep(1);
    }

    //线程等待退出
    for(int i = 0; i < NUM; i++)
    {
        pthread_join(vtid[i],nullptr);
        std::cout<<"全部子线程已退出"<<std::endl;
    }

    return 0;
}