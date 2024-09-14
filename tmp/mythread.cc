#include <cstdio>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
using namespace std;
#define NUM 10

class ThreadData
{
public:
    pthread_t tid;
    char buffer[64];
};

void *start_routine(void *args)
{
    int tmp = 10;
    while (true)
    {
        cout << "我是" << (char *)args << "，我的tmp是" << tmp << "，tmp的地址是" << &tmp << endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    vector<ThreadData *> vtid;
    for (int i = 0; i < NUM; i++)
    {
        ThreadData *td = new ThreadData;
        snprintf(td->buffer, sizeof(td->buffer), "thread%d", i);
        pthread_create(&td->tid, nullptr, start_routine, (void *)td->buffer);
        vtid.push_back(td);
    }

    for (int i = 0; i < NUM; i++)
    {
        pthread_join(vtid[i]->tid, nullptr);
    }

    return 0;
}