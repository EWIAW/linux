#pragma once

#include <queue>
#include <pthread.h>

//阻塞队列
template<class T>
class BlockQueue
{
public:
    BlockQueue(const int maxcap)
    :_maxcap(maxcap)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_p_cond,nullptr);
        pthread_cond_init(&_c_cond,nullptr);
    }

    void push(const T& in)
    {
        pthread_mutex_lock(&_mutex);
        //1.判断，如果阻塞队列已满，则生产者停止生产

        //细节2：这里判断一定要用while，不能用if。
        //因为如果只有一个消费者 而 有十个生产者 当pop函数用broadcast将十个生产者都唤醒的时候，而此时阻塞队列里面只有一个空位，那么这个十个生产者产生十个数据进行插入会出现问题
        while(isfull() == true)
        {
            //细节1：
            pthread_cond_wait(&_p_cond,&_mutex);
        }

        //2.生产者进行生产
        _queue.push(in);

        //3.走到这里，阻塞队列里面一定有东西，所以消费者进行消费
        //细节3：signal可以放在临界区外，也可以凡在临界区内
        pthread_cond_signal(&_c_cond);

        pthread_mutex_unlock(&_mutex);
    }

    void pop(T* out)
    {
        pthread_mutex_lock(&_mutex);

        //1.如果队列为空，则消费者停止消费
        while(isempty() == true)
        {
            pthread_cond_wait(&_c_cond,&_mutex);
        }

        //2.如果队列不为空，则消费者进行消费
        *out = _queue.front();
        _queue.pop();

        //3.走到这里，队列至少有一个空位，所以生产者可以进行生产
        pthread_cond_signal(&_p_cond);

        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_p_cond);
        pthread_cond_destroy(&_c_cond);
    }

private:
    //判断队列是否为空
    bool isempty()
    {
        return _queue.empty();
    }

    //判断队列是否已满
    bool isfull()
    {
        if(_queue.size() == _maxcap)
            return true;
        else
            return false;
    }

private:
    std::queue<T> _queue;//队列
    int _maxcap;//队列的最大容量
    pthread_mutex_t _mutex;//锁
    pthread_cond_t _p_cond;//生产者条件变量
    pthread_cond_t _c_cond;//消费者条件变量
};