#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "socket.hpp"
#include "epoller.hpp"

class Connection;

class Tcpserver;

using func_t = std::function<void(Connection *)>;

// 对每一个客户端的文件描述符 通过先描述再组织的方式 管理起来
class Connection
{
public:
    Connection(int sock)
        : _fd(sock)
    {
    }

    void Reg(func_t recver, func_t sender, func_t excepter)
    {
        _recver = recver;
        _sender = sender;
        _excepter = excepter;
    }

public:
    int _fd;
    std::string inbuffer;
    std::string outbuffer;

    func_t _recver;   // 从socket中读
    func_t _sender;   // 向socket中写
    func_t _excepter; // 处理socket上的异常IO事件

    Tcpserver *_tsp; // todo
};

class Tcpserver
{
public:
    Tcpserver()
    {
    }

    ~Tcpserver()
    {
    }

    void init_Server()
    {
        // 1.创建套接字K
        _sock.Socket();
        _sock.Bind();
        _sock.Listen();

        // 2.创建epoll模型
        _epoll.Create();

        // 3.将listen套接字添加到epoll模型中
        AddConection(_sock.FD(), EPOLLIN | EPOLLET, std::bind(&Tcpserver::r, this, std::placeholders::_1), nullptr, nullptr);
    }

    void r(Connection *conn)
    {
    }

    void AddConection(int sock, uint32_t events, func_t recver, func_t sender, func_t excepter)
    {
        // 为该sock创建connection对象
        Connection *conn = new Connection(sock);
        conn->Reg(recver, sender, excepter);

        // 将sock要关心的事件 注册 到epoll模型中
        _epoll.AddEvent(_sock.FD(), events);

        // 将kv添加到connections对象中
        _connections.insert(std::make_pair(sock, conn));
    }

    void run()
    {
        for (;;)
        {
        }
    }

private:
    Sock _sock;
    epoller _epoll;
    std::unordered_map<int, Connection *> _connections;
};