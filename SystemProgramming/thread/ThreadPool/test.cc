#include <iostream>
#include <memory>

#include <unistd.h>

#include "Thread.hpp"
#include "ThreadPool.hpp"
#include "task.hpp"

using namespace std;

int main()
{
    ThreadPool<TaskCal>* tp=ThreadPool<TaskCal>::GetInstance();
    tp->run();

    int x, y;
    char op;

    while (true)
    {
        std::cout << "请输入数据1：";
        std::cin >> x;
        std::cout << "请输入数据2：";
        std::cin >> y;
        std::cout << "请输入运算符：";
        std::cin >> op;

        TaskCal task(x, y, op);
        std::cout << task.ToTaskString() << std::endl;
        tp->push(task);
        // sleep(1);
    }

    return 0;
}