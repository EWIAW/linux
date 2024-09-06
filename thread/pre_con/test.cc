#include <time.h>

#include <iostream>

#include <unistd.h>

#include "blockqueue.hpp"

//生产线程
void* Preduce(void* args)
{
    BlockQueue<int>* tmp  = (BlockQueue<int>*)args;
    while(true)
    {
        //生产活动
        int input = rand()%10 + 1;
        tmp->push(input);
        std::cout<<"生产数据:"<<input<<std::endl;
        // sleep(1);
    }
    return nullptr;
}

//消费线程
void* Consume(void* args)
{
    BlockQueue<int>* tmp  = (BlockQueue<int>*)args;
    while(true)
    {
        //消费活动
        int output;
        tmp->pop(&output);
        std::cout<<"消费数据:"<<output<<std::endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    srand(time(nullptr));

    BlockQueue<int>* bq = new BlockQueue<int>(5);//创建阻塞队列
    pthread_t ptid,ctid;

    //创建生产者线程
    //创建消费者线程
    pthread_create(&ptid,nullptr,Preduce,(void*)bq);
    pthread_create(&ctid,nullptr,Consume,(void*)bq);


    //等待线程退出
    pthread_join(ptid,nullptr);
    pthread_join(ctid,nullptr);

    delete bq;

    return 0;
}