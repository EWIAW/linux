#pragma once

#include <cstdio>
#include <cstdarg>
#include <ctime>

#include <iostream>
#include <string>

#include <sys/types.h>
#include <unistd.h>

using namespace std;

#define DEBUG 0
#define NORMAL 1
#define WARRING 2
#define ERROR 3
#define FATAL 4 // 致命错误

#define LOG_NORMAL "log_normal.txt"
#define LOG_ERROR "log_error.txt"

// 将日志等级从数字转换为字符串
const char *to_levelstr(const int &level)
{
    switch (level)
    {
    case 0:
        return "DEBUG";
        break;
    case 1:
        return "NORMAL";
        break;
    case 2:
        return "WARRING";
        break;
    case 3:
        return "ERROR";
        break;
    case 4:
        return "FATAL";
        break;
    default:
        return nullptr;
        break;
    }
}

// 获取当前时间
string get_nowtime()
{
    time_t current_time;
    struct tm *time_ptr;
    char buffer[80];

    time(&current_time);                 // 获取当前时间
    time_ptr = localtime(&current_time); // 转换为当地时间

    // 格式化时间
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", time_ptr);
    return buffer;
}

// 日志函数
// logmessage(NORMAL,"create socket error:%d",_listensockfd)
void logmessage(const int &level, const char *format, ...)
{
    const int num = 1024;
    //[日志等级][时间][pid][message]

    char logprev[num]; // 获取前三个信息
    snprintf(logprev, sizeof(logprev) - 1, "[%s][%s][%d]", to_levelstr(level), get_nowtime().c_str(), getpid());

    // 获取后一个信息
    char logmessage[num];
    va_list args;
    va_start(args, format);
    vsnprintf(logmessage, sizeof(logmessage) - 1, format, args);
    va_end(args);

    cout << logprev << logmessage << endl;
}