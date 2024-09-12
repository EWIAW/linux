#include <iostream>

#include <unistd.h>

#include "Thread.hpp"
#include "ThreadPool.hpp"

void *test(void *args)
{
    while(true)
    {
        std::cout<<"hello linux"<<std::endl;
        sleep(1);
    }
}

int main()
{
    ThreadPool<int> tp(10);
    tp.run();

    while(true)
    {
        ;
    }

    return 0;
}