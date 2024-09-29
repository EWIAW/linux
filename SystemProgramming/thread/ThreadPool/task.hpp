#pragma once

#include <cstdio>
#include <ctime>

#include <iostream>
#include <string>

typedef int (*func_t)(int, int, char);

// 进行运算
int Cal(int x, int y, char op)
{
    switch (op)
    {
    case '+':
        return x + y;
        break;
    case '-':
        return x - y;
        break;
    case '*':
        return x * y;
        break;
    case '/':
        if (y == 0)
        {
            std::cout << "除数为0，无法进行运算" << std::endl;
            return -1;
        }
        else
        {
            return x / y;
        }
        break;
    case '%':
        if (y == 0)
        {
            std::cout << "除数为0，无法进行运算" << std::endl;
            return -1;
        }
        else
        {
            return x % y;
        }
        break;

    default:
        std::cout << "出错！！！" << std::endl;
        return -1;
        break;
    }
}

// 计算任务
class TaskCal
{
public:
    // 构造函数
    TaskCal()
    {
    }
    TaskCal(int x, int y, char op, func_t cal = Cal)
        : _x(x), _y(y), _op(op), _cal(Cal)
    {
    }

    std::string ToTaskString()
    {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "生成任务:%d %c %d = ？？？", _x, _op, _y);
        return buffer;
    }

    // 计算任务结果，并把结果返回
    void operator()()
    {
        int result = Cal(_x, _y, _op);
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "任务完成:%d %c %d = %d", _x, _op, _y, result);
        // return buffer;
        std::cout<<buffer<<std::endl;
    }

    // 析构函数
    ~TaskCal()
    {
    }

private:
    int _x;      // 第一位数
    int _y;      // 第二位数
    char _op;    // 运算符
    func_t _cal; // 计算函数
};
