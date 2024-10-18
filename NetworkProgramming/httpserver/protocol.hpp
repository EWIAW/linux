#pragma once

#include <cstring>

#include <string>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Util.hpp"

const std::string sep = "\r\n";                  // 分隔符
const std::string default_root = "wwwroot";      // 网页默认根目录
const std::string home_page = "index.html";      // 默认网页页面
const std::string html_404 = "wwwroot/404.html"; // 资源不存在页面

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
        std::string line = Util::getOneLine(_inbuffer, sep);
        if (line.empty())
            return;

        // 2.从请求行中提取三个字段
        std::stringstream ss(line);
        ss >> _method >> _url >> _httpversion;

        // 3.添加服务器web默认路径
        _path += default_root;              // /wwwroot
        _path += _url;                      // /wwwroot/
        if (_path[_path.size() - 1] == '/') // /wwwroot/
        {
            _path += home_page; // /wwwroot/index.html      默认界面
        }

        // 4.获取请求资源的后缀
        ssize_t n = _path.rfind(".");
        if (n == std::string::npos)
        {
            _suffix = "html";
        }
        else
        {
            _suffix = _path.substr(n);
        }

        // 5.得到资源的大小
        struct stat st;
        int pos = stat(_path.c_str(), &st);
        if (pos == 0)
        {
            _size = st.st_size;
        }
    }

public:
    std::string _inbuffer; // 缓冲区
    // std::string reqline;                // 请求行
    // std::vector<std::string> reqhander; // 请求报头
    // std::string reqtext;                // 请求正文

    std::string _method;      // 请求方法
    std::string _url;         // 请求路径
    std::string _httpversion; // 请求http版本
    std::string _path;        // web默认路径
    std::string _suffix;      // 请求资源的后缀
    int _size;        // 资源大小
};

// 响应
class HttpResponse
{
public:
    HttpResponse() {}

    ~HttpResponse() {}

public:
    std::string _outbuffer;
};