#pragma once

#include <stdio.h>
#include <time.h>

#include <iostream>
#include <string>

#define DEBUG printf("hello linux\n");

typedef int (*func_t)(int, int, char);

char GetOp()
{
    std::string str("+-*/%");
    int i = rand() % 5;
    char op = str[i];
    return op;
}

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

// 保存任务
void SaveData(std::string message)
{
    FILE *fp = fopen("./log.txt", "a+");
    if (fp == nullptr)
    {
        std::cerr << "打开文件失败!!!" << std::endl;
        return;
    }

    fputs(message.c_str(), fp);
    fputs("\n", fp);

    fclose(fp);
}

// 计算任务
class TaskCal
{
public:
    // 构造函数
    TaskCal()
    {
    }
    TaskCal(int x, int y, func_t cal = Cal)
        : _x(x), _y(y), _cal(Cal)
    {
        _op = GetOp();
    }

    std::string ToTaskString()
    {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "生成任务:%d %c %d = ？？？", _x, _op, _y);
        return buffer;
    }

    // 计算任务结果，并把结果返回
    std::string operator()()
    {
        int result = Cal(_x, _y, _op);
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "任务完成:%d %c %d = %d", _x, _op, _y, result);
        return buffer;
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

// 保存任务
class TaskSave
{
    typedef void (*func_t)(std::string);

public:
    TaskSave()
    {
    }
    TaskSave(std::string message, func_t save = SaveData)
        : _data(message)
    {
    }

    // 将结果保存到文件中
    void Save()
    {
        SaveData(_data);
        std::cout << "保存数据成功!!!" << std::endl;
    }

    ~TaskSave()
    {
    }

private:
    std::string _data; // 需要保存的数据
    func_t _save;      // 保存数据的函数
};

// 两个阻塞队列
// template <class C, class S>
// class TwoBlockQueue
// {
// public:
//     BlockQueue<C> *_c;
//     BlockQueue<S> *_s;
// };