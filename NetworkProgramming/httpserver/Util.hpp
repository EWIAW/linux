#pragma once
#include <iostream>
#include <fstream>

class Util
{
public:
    // 读取一行信息
    static std::string getOneLine(std::string &buffer, const std::string &sep)
    {
        ssize_t n = buffer.find(sep);
        if (n == std::string::npos)
            return "";

        std::string sub = buffer.substr(0, n);
        buffer.erase(0, sub.size() + sep.size());
        return sub;
    }

    // 读取正文数据，即html文件
    static bool readFile(const std::string &path, std::string &body)
    {
        std::ifstream in(path);
        if (!in.is_open())
        {
            return false; // 资源不存在
        }
        std::string line;
        while (std::getline(in, line))
        {
            body += line;
        }

        in.close();
        return true;
    }

    //获取请求文件的后缀类型
    static std::string suffixToDesc(const std::string& suffix)
    {
        std::string ret="Content-Type: ";
        if(suffix==".html")
        {
            ret+="text/html";
        }
        else if(suffix==".jpg")
        {
            ret+="applicatiopn/x-jpg";
        }
        ret+="\r\n";
        return ret;
    }
};