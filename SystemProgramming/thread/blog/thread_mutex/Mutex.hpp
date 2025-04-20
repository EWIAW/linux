#include <pthread.h>

// 封装锁
class Mutex
{
private:
    pthread_mutex_t *_mtx;

public:
    Mutex(pthread_mutex_t *mtx)
        : _mtx(mtx)
    {
    }

    void lock()
    {
        pthread_mutex_lock(_mtx);
    }

    void unlock()
    {
        pthread_mutex_unlock(_mtx);
    }

    Mutex()
    {
    }
};

class LockGuard
{
private:
    Mutex _mtx;

public:
    LockGuard(pthread_mutex_t *mtx)
        : _mtx(mtx)
    {
        _mtx.lock();
    }
    ~LockGuard()
    {
        _mtx.unlock();
    }
};