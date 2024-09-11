#include <iostream>

#include <unistd.h>

#include "Thread.hpp"

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
    Thread t1;

    t1.start(test);

    t1.join();

    return 0;
}