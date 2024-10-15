#pragma once

#include <cstring>

#include <string>

#define SEP " "
#define SEP_LEN strlen(SEP)

#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

//  添加报头
//  将“x op y"               ->      “content_len”/r/n“x op y”/r/n
//  将“exitcode result"      ->      “content_len”/r/n“exitcode result”/r/n
std::string enLength(const std::string &text)
{
    std::string ret;
    int content_len = text.size();
    ret += to_string(content_len);
    ret += LINE_SEP;
    ret += text;
    ret += LINE_SEP;
    return ret;
}

// 去除报头
//  将  “content_len”/r/n“x op y”/r/n               ->      “x op y"
//  将  “content_len”/r/n“exitcode result”/r/n      ->      “exitcode result"
bool deLength(const std::string &text, std::string &out)
{
    ssize_t n = text.find(LINE_SEP);
    if (n == std::string::npos)
        return false;

    std::string content_len_string = text.substr(0, n);
    int content_len = stoi(content_len_string);

    out = text.substr(n + LINE_SEP_LEN, content_len);
    return true;
}

// 从sockfd中读取数据
// “content_len”/r/n“x op y”/r/n
// 将读取到的字节流先放到inbuffer缓冲中，再将完整的一条请求放到text中
bool readData(const int &sockfd, std::string &inbuffer, std::string &text)
{
    char buffer[1024];
    while (true)
    {
        ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        // cout << buffer << endl;
        if (n > 0)
        {
            buffer[n] = 0;
            inbuffer += buffer;
            ssize_t n = inbuffer.find(LINE_SEP);
            if (n == std::string::npos)
                continue;

            std::string text_len_string = inbuffer.substr(0, n);
            int text_len = stoi(text_len_string);
            int total_len = text_len_string.size() + LINE_SEP_LEN * 2 + text_len; // 求出数据报总长度

            if (inbuffer.size() < total_len)
            {
                continue;
            }

            // 走到这里，说明至少有一个完整的数据报
            text = inbuffer.substr(0, total_len);
            inbuffer.erase(0, total_len);
            break;
        }
        else if (n == 0)
        {
            // n==0说明客户端已经关闭
            return false;
        }
    }
    return true;
}

// 请求
class Request
{
public:
    Request()
        : _x(0), _y(0), _op(0)
    {
    }

    Request(const int &x, const int &y, const char &op)
        : _x(x), _y(y), _op(op)
    {
    }

    ~Request() {}

    // 序列化 将结构化数据转换为字符串
    bool Serialization(std::string &out)
    {
        out.clear();
        // 结构化 “x op y”
        std::string x_string = std::to_string(_x);
        std::string y_string = std::to_string(_y);
        out += x_string;
        out += SEP;
        out += _op;
        out += SEP;
        out += y_string;
        return true;
    }

    // 反序列化 将字符串转换为结构化数据
    bool deSerialization(const std::string &in)
    {
        // 将“x op y” 反序列化
        size_t left = in.find(SEP);
        size_t right = in.rfind(SEP);

        if (left == std::string::npos || right == std::string::npos)
            return false;
        if (left == right)
            return false;
        if (right - SEP_LEN - left != 1)
            return false;

        std::string x_string = in.substr(0, left);
        // std::string y_string = in.substr(right + SEP_LEN, in.size() - (right + SEP_LEN) - LINE_SEP_LEN);
        std::string y_string = in.substr(right + SEP_LEN);
        char op = in[left + SEP_LEN];

        if (x_string.empty())
            return false;
        if (y_string.empty())
            return false;

        _x = std::stoi(x_string);
        _y = std::stoi(y_string);
        _op = op;

        return true;
    }

public:
    int _x;
    int _y;
    char _op;
};

// 响应
class Response
{
public:
    Response()
        : _exitcode(0), _result(0)
    {
    }

    ~Response() {}

    // 序列化 将结构化数据转换为字符串
    bool Serialization(std::string &out)
    {
        //"0 100"
        out.clear();
        std::string ec_string = std::to_string(_exitcode);
        std::string rs_string = std::to_string(_result);
        out += ec_string;
        out += SEP;
        out += rs_string;
        return true;
    }

    // 反序列化 将字符串转换为结构化数据
    bool deSerialization(const std::string &in)
    {
        // 将“0 100”反序列化
        ssize_t mid = in.find(SEP);
        if (mid == std::string::npos)
            return false;

        int exitcode = stoi(in.substr(0, mid));
        int result = stoi(in.substr(mid + 1));

        _exitcode = exitcode;
        _result = result;
        return true;
    }

public:
    int _exitcode;
    int _result;
};