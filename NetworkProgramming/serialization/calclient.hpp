#pragma once

#include <iostream>
#include <string>
#include <cctype>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "log.hpp"
#include "protocol.hpp"

using namespace std;

static const int buffernum = 1024;

enum
{
    SOCKET_ERR = 1,
    CONNECT_ERR,
};

class tcpclient
{
public:
    tcpclient(const string &ip, const uint16_t &port)
        : _sockfd(-1), _ip(ip), _port(port)
    {
    }

    void init()
    {
        // 1.创建套接字
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd == -1)
        {
            logmessage(FATAL, "create socket error");
            exit(SOCKET_ERR);
        }
        logmessage(NORMAL, "create socket success");

        // 2.bind套接字
        // 3.listen监听
        // 4.accept连接
        // 以上操作client都不需要做或者操作系统帮助我们做
    }

    void start()
    {
        // 5.connect连接
        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(_ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(_port);
        int ret = connect(_sockfd, (struct sockaddr *)&server, sizeof(server));
        if (ret == -1)
        {
            logmessage(FATAL, "connect server error");
            exit(CONNECT_ERR);
        }
        logmessage(NORMAL, "connect server success");

        // 连接成功后，可以进行通信
        string line;
        string inbuffer;
        while (true)
        {
            // 1.发送数据
            getline(std::cin, line);
            Request req = Parseline(line); // 解析输入的字符串

            string content;
            req.Serialization(content); // 将请求序列化
            // 添加报头
            string content_str = enLength(content);

            send(_sockfd, content_str.c_str(), content_str.size(), 0);

            // 2.读取数据
            string text;
            readData(_sockfd, inbuffer, text);
            // 去掉报头
            string out;
            deLength(text, out);

            // 将out反序列化
            Response res;
            res.deSerialization(out);
            cout << res._exitcode << ":" << res._result << endl;
        }
    }

    ~tcpclient()
    {
        if (_sockfd >= 0)
        {
            close(_sockfd);
        }
    }

private:
    // 解析字符串并返回一个请求
    // "123*456"
    Request Parseline(const string &line)
    {
        string left, right;
        int n = 0;
        char op;

        // 定义一个flag来帮助我们判断是否左操作数、右操作数或者运算符
        int flag = 0; // 如果为0，左操作数，为1，右操作数，为2，运算符

        while (n < line.size())
        {
            switch (flag)
            {
            case 0:
                left += line[n++];
                if (!isdigit(line[n]))
                    flag = 2;
                break;
            case 1:
                right += line[n++];
                break;
            case 2:
                op = line[n++];
                flag = 1;
                break;
            default:
                break;
            }
        }
        Request ret(stoi(left), stoi(right), op);
        return ret;
    }

private:
    int _sockfd;
    string _ip;
    uint16_t _port;
};