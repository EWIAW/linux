#pragma once

#include <cstring>

#include <string>
#include <vector>
#include <sstream>

#include "Util.hpp"

const std::string sep = "\r\n"; // 分隔符
const std::string default_root = "wwwroot";
const std::string home_page = "index.html";

// 请求
class HttpRequest
{
public:
    HttpRequest() {}

    ~HttpRequest() {}

    // 解析请求
    void parse()
    {
        // 1.从inbuffer中拿到第一行，分隔符\r\n
        std::string line = Util::getOneLine(inbuffer, sep);
        if (line.empty())
            return;

        // 2.从请求行中提取三个字段
        std::cout << "line:" << line << std::endl;

        std::stringstream ss(line);
        ss >> method >> url >> httpversion;
        std::cout << "method:" << method << std::endl;
        std::cout << "url:" << method << std::endl;
        std::cout << "httpserver:" << method << std::endl;

        // 3.添加服务器web默认路径
        std::string path = default_root; // ./wwwroot
        path += url;                     // ./wwwroot/a/b/c.html
        if (path[path.size() - 1] == '/')
        {
            path += home_page;
        }
    }

public:
    std::string inbuffer; // 缓冲区
    // std::string reqline;                // 请求行
    // std::vector<std::string> reqhander; // 请求报头
    // std::string reqtext;                // 请求正文

    std::string method;      // 请求方法
    std::string url;         // 请求路径
    std::string httpversion; // 请求http版本
    std::string path;        // web默认路径
};

// 响应
class HttpResponse
{
public:
    HttpResponse() {}

    ~HttpResponse() {}

public:
    std::string outbuffer;
};