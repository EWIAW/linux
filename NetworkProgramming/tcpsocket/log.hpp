#pragma once

#include <iostream>

using namespace std;

#define DEBUG 0
#define NORMAL 1
#define WARRING 2
#define ERROR 3
#define FATAL 4 // 致命错误

// 日志函数
void logmessage(int level, string message)
{
    cout << message << endl;
}