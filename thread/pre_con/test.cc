#include <time.h>

#include <iostream>

#include <unistd.h>

#include "blockqueue.hpp"
#include "task.hpp"

///////////////////////////////////////////////////////////////////////////////////测试
// //生产线程
// void* Preduce(void* args)
// {
//     BlockQueue<int>* tmp  = (BlockQueue<int>*)args;
//     while(true)
//     {
//         //生产活动
//         int input = rand()%10 + 1;
//         tmp->push(input);
//         std::cout<<"生产数据:"<<input<<std::endl;
//         // sleep(1);
//     }
//     return nullptr;
// }

// //消费线程
// void* Consume(void* args)
// {
//     BlockQueue<int>* tmp  = (BlockQueue<int>*)args;
//     while(true)
//     {
//         //消费活动
//         int output;
//         tmp->pop(&output);
//         std::cout<<"消费数据:"<<output<<std::endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     srand(time(nullptr));

//     BlockQueue<int>* bq = new BlockQueue<int>(5);//创建阻塞队列
//     pthread_t ptid,ctid;

//     //创建生产者线程
//     //创建消费者线程
//     pthread_create(&ptid,nullptr,Preduce,(void*)bq);
//     pthread_create(&ctid,nullptr,Consume,(void*)bq);

//     //等待线程退出
//     pthread_join(ptid,nullptr);
//     pthread_join(ctid,nullptr);

//     delete bq;

//     return 0;
// }

///////////////////////////////////////////////////////////////////////////////////////

// 获取运算符号
char GetOp()
{
    std::string str("+-*/%");
    int i = rand() % 5;
    char op = str[i];
    return op;
}

// 生产者进行生产任务
void *produce(void *args)
{
    // 计算任务阻塞队列
    BlockQueue<TaskCal> *tmp = ((TwoBlockQueue<TaskCal, TaskSave> *)args)->_c;
    while (true)
    {
        // 生产数据
        int x = rand() % 10;
        int y = rand() % 5;
        char op = GetOp();
        TaskCal task(x, y, op);
        tmp->push(task);
        std::cout << task.ToTaskString() << std::endl;
        sleep(1);
    }
    return nullptr;
}

// 消费者进行处理任务
// 既是生产者也是消费者，消费数据，并将数据生成给保存任务线程
void *consume(void *args)
{
    BlockQueue<TaskCal> *tmp1 = ((TwoBlockQueue<TaskCal, TaskSave> *)args)->_c;  // 计算任务
    BlockQueue<TaskSave> *tmp2 = ((TwoBlockQueue<TaskCal, TaskSave> *)args)->_s; // 保存任务
    while (true)
    {
        // 消费数据
        TaskCal task;
        tmp1->pop(&task);
        std::cout << task() << std::endl;

        // 生产结果 给 保存线程
        TaskSave save(task());
        tmp2->push(save);
        std::cout << "发送数据......" << std::endl;
    }
    return nullptr;
}

// 消费结果，将结果保存起来
void *save(void *args)
{
    BlockQueue<TaskSave> *tmp = ((TwoBlockQueue<TaskCal, TaskSave> *)args)->_s;
    while (true)
    {
        TaskSave save;
        tmp->pop(&save);
        save.Save();
    }
    return nullptr;
}

int main()
{
    srand(time(nullptr));

    // 创建两个阻塞队列
    TwoBlockQueue<TaskCal, TaskSave> *tbq = new TwoBlockQueue<TaskCal, TaskSave>();
    tbq->_c = new BlockQueue<TaskCal>(5);
    tbq->_s = new BlockQueue<TaskSave>(5);

    // BlockQueue<TaskCal> *bq = new BlockQueue<TaskCal>(5);
    pthread_t ptid[3], pctid[3], ctid;

    pthread_create(&ptid[0], nullptr, produce, (void *)tbq);
    pthread_create(&ptid[1], nullptr, produce, (void *)tbq);
    pthread_create(&ptid[2], nullptr, produce, (void *)tbq);

    pthread_create(&pctid[0], nullptr, consume, (void *)tbq);
    pthread_create(&pctid[1], nullptr, consume, (void *)tbq);
    pthread_create(&pctid[2], nullptr, consume, (void *)tbq);

    pthread_create(&ctid, nullptr, save, (void *)tbq);

    pthread_join(ptid[0], nullptr);
    pthread_join(ptid[1], nullptr);
    pthread_join(ptid[2], nullptr);

    pthread_join(pctid[0], nullptr);
    pthread_join(pctid[1], nullptr);
    pthread_join(pctid[2], nullptr);

    pthread_join(ctid, nullptr);

    delete tbq->_c;
    delete tbq->_s;
    delete tbq;

    return 0;
}