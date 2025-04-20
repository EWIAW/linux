#include <pthread.h>
#include <queue>

template <class T>
class block_queue
{
    static size_t defaultsize = 10; // 队列默认大小为10
private:
    std::queue<T> _queue_;     // 队列中存储的是任务，即T类型，其实一个个任务就是一个个类对象
    size_t _maxsize_;          // 队列缓冲区的大小
    pthread_mutex_t _mtx_;     // 互斥锁
    pthread_cond_t _pre_cond_; // 生产者的条件变量
    pthread_cond_t _con_cond_; // 消费者的条件变量

public:
    block_queue(const size_t maxsize = defaultsize)
        : _maxsize_(maxsize)
    {
        // 对锁和条件变量做一个初始化
        pthread_mutex_init(&_mtx_, nullptr);
        pthread_cond_init(&_pre_cond_, nullptr);
        pthread_cond_init(&_con_cond_, nullptr);
    }

    ~block_queue()
    {
        // 销毁锁和条件变量
        pthread_mutex_destroy(&_mtx_);
        pthread_cond_destroy(&_pre_cond_);
        pthread_cond_destroy(&_con_cond_);
    }

    void push(const T &in) // 插入一个任务
    {
        pthread_mutex_lock(&_mtx_);

        while (isFull() == true)
        {
            pthread_cond_wait(&_pre_cond_);
        }

        _queue_.push(in);

        pthread_cond_signal(&_con_cond_);

        pthread_mutex_unlock(&_mtx_);
    }

private:
    bool isFull() // 判断队列是否已满
    {
        if (_maxsize_ == _queue_.size())
            return true;
        else
            return false;
    }

    bool isEmpty() // 判断队列是否为空
    {
        if (_queue_.empty() == true)
            return true;
        else
            return false;
    }
};