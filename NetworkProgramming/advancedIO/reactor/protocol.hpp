#pragma once

#include <cstring>

#include <string>

#include <jsoncpp/json/json.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SEP " "
#define SEP_LEN strlen(SEP)

#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

enum
{
    OK = 0,
    DIV_ZERO, // 除0
    MOD_ZERO, // 取模0
    OP_ERROR  // 运算符错误
};

//  添加报头
//  将“x op y"               ->      “content_len”/r/n“x op y”/r/n
//  将“exitcode result"      ->      “content_len”/r/n“exitcode result”/r/n
std::string enLength(const std::string &text)
{
    std::string ret;
    int content_len = text.size();
    ret += std::to_string(content_len);
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
        Json::Value root;
        root["first"] = _x;
        root["second"] = _y;
        root["operator"] = _op;

        Json::FastWriter writer;
        out = writer.write(root);
        return true;
    }

    // 反序列化 将字符串转换为结构化数据
    bool deSerialization(const std::string &in)
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);

        _x = root["first"].asInt();
        _y = root["second"].asInt();
        _op = root["operator"].asInt();
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
        Json::Value root;
        root["exitcode"] = _exitcode;
        root["result"] = _result;
        Json::FastWriter writer;
        out = writer.write(root);
        return true;
    }

    // 反序列化 将字符串转换为结构化数据
    bool deSerialization(const std::string &in)
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);
        _exitcode = root["exitcode"].asInt();
        _result = root["result"].asInt();
        return true;
    }

public:
    int _exitcode;
    int _result;
};

// 解析请求
//"total_len\r\nx op y\r\n"
bool Parse(std::string &inbuffer, std::string &text)
{
    std::cout << inbuffer << std::endl;

    text = "";
    auto pos = inbuffer.find("\r\n", 0);
    std::string total_len = inbuffer.substr(0, pos);
    // printf("%s",total_len.c_str());
    int len = std::stoi(total_len);
    int final_len = total_len.size() + 4 + len;
    if (inbuffer.size() < final_len)
    {
        Log_Message(DUBUG, "请求不完整");
        return false;
    }
    text = inbuffer.substr(0, final_len);
    std::cout << text << std::endl;
    inbuffer.erase(0, final_len);
    return true;
}