#include <pthread.h>
#include <queue>

template <class T>
class block_queue
{
private:
    std::queue<T> _queue_;     // 队列中存储的是任务，即T类型，其实一个个任务就是一个个类对象
    size_t _maxsize_;          // 队列缓冲区的大小
    pthread_mutex_t _mtx_;     // 互斥锁
    pthread_cond_t _pre_cond_; // 生产者的条件变量
    pthread_cond_t _con_cond_; // 消费者的条件变量

public:
    block_queue(const size_t maxsize)
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

    void push(const T &in) // 生产者插入一个任务
    {
        pthread_mutex_lock(&_mtx_);

        // 如果此时缓冲区满了，则生产者不能再生产任务了，需要阻塞等待消费者消费任务
        while (isFull() == true)
        {
            pthread_cond_wait(&_pre_cond_, &_mtx_); // 使用条件变量阻塞生产者
        }

        _queue_.push(in); // 向缓冲区中插入任务

        // 当生产者向缓冲区插入任务后，就能保证缓冲区中至少有一个任务，那么这个时候需要唤醒消费者去消费任务
        pthread_cond_signal(&_con_cond_); // 唤醒消费者消费任务

        pthread_mutex_unlock(&_mtx_);
    }

    void pop(T &out) // 消费者拿取一个任务
    {
        pthread_mutex_lock(&_mtx_);

        // 如果此时缓冲区为空，则消费者不能再消费任务了，需要阻塞等待生产者生产任务
        while (isEmpty() == true)
        {
            pthread_cond_wait(&_con_cond_, &_mtx_); // 使用条件变量阻塞消费者
        }

        // 拿取任务
        out = _queue_.front();
        _queue_.pop();

        // 当消费者拿取一个任务后，就能保证缓冲区中至少有一个空位，那么这个时候需要唤醒生产者生产任务
        pthread_cond_signal(&_pre_cond_); // 缓存生产者生产任务

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