#pragma once

#include <ctime>
#include <stdarg.h>
#include <cstdio>

#include <iostream>
#include <string>

#define INFO 0  // 正常
#define DUBUG 1 // 调试
#define WARN 2  // 警告
#define ERROR 3 // 错误
#define FATAL 4 // 致命

// 获取日志等级
std::string Get_Level(int level)
{
    switch (level)
    {
    case 0:
        return "INFO";
        break;
    case 1:
        return "DUBUG";
        break;
    case 2:
        return "WARN";
        break;
    case 3:
        return "ERROR";
        break;
    case 4:
        return "FATAL";
        break;
    default:
        return "NOTHING";
        break;
    }
}

// 打印日志信息
void Log_Message(int level, const char *format, ...)
{
    //[时间][日志等级][原因]
    time_t current_time = time(nullptr);
    struct tm *local_time = localtime(&current_time);
    char time_str[32] = {0};
    strftime(time_str, sizeof(time_str) - 1, "%Y-%m-%d %H:%M:%S", local_time);

    std::string level_str = Get_Level(level);

    char message[128];
    va_list arg;
    va_start(arg, format);
    vsnprintf(message, sizeof(message) - 1, format, arg);

    printf("[%s][%s][%s]\n", time_str, level_str.c_str(), message);
}