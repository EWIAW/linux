#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include <unistd.h>

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
    static const int default_size = 1024;

public:
    Tcpserver()
        : _events(new struct epoll_event[default_size]),
          _max_num(default_size)
    {
    }

    ~Tcpserver()
    {
        delete[] _events;
    }

    void init_Server(func_t server)
    {
        // 1.创建套接字K
        _sock.Socket();
        _sock.Bind();
        _sock.Listen();

        // 2.创建epoll模型
        _epoll.Create();

        // 3.将listen套接字添加到epoll模型中
        AddConection(_sock.FD(), EPOLLIN | EPOLLET, std::bind(&Tcpserver::Accepter, this, std::placeholders::_1), nullptr, nullptr);

        _server = server;
    }

    void run()
    {
        int timeout = -1;
        for (;;)
        {
            int n = _epoll.Wait(_events, _max_num, timeout);
            // 循环遍历就绪队列中的事件
            for (int i = 0; i < n; i++)
            {
                int event = _events[i].events;
                int fd = _events[i].data.fd;

                // 将所有错误转换为读写错误
                if (event & EPOLLERR)
                    event |= EPOLLIN | EPOLLOUT;
                if (event & EPOLLHUP)
                    event |= EPOLLIN | EPOLLOUT;

                // 事件就绪
                if ((event & EPOLLIN) && Is_Connection_Exist(fd) && _connections[fd]->_recver) // 读事件就绪
                    _connections[fd]->_recver(_connections[fd]);
                if ((event & EPOLLOUT) && Is_Connection_Exist(fd) && _connections[fd]->_sender) // 写事件就绪
                    _connections[fd]->_sender(_connections[fd]);
            }
        }
    }

    // 判断连接是否还存在
    bool Is_Connection_Exist(int sock)
    {
        auto it = _connections.find(sock);
        return it != _connections.end();
    }

private:
    // 添加连接
    void AddConection(int sock, uint32_t events, func_t recver, func_t sender, func_t excepter)
    {
        // 为该sock创建connection对象
        Connection *conn = new Connection(sock);
        conn->Reg(recver, sender, excepter);

        // 将sock要关心的事件 注册 到epoll模型中
        _epoll.AddEvent(sock, events);

        // 将kv添加到connections对象中
        _connections.insert(std::make_pair(sock, conn));
        Log_Message(DUBUG, "add new sock : %d in epoll and unordered_map", sock);
    }

    // 用于给listen套接字获取新连接
    void Accepter(Connection *conn)
    {
        for (;;)
        {
            int err; /// 错误码
            std::string ip;
            uint16_t port;
            int sock = _sock.Accept(&ip, &port, &err);

            if (sock > 0)
            {
                // 为新的连接创建conneciton对象，并加入到epoll模型和哈希表中
                AddConection(sock, EPOLLIN | EPOLLET,
                             std::bind(&Tcpserver::Recv, this, std::placeholders::_1),
                             std::bind(&Tcpserver::Send, this, std::placeholders::_1),
                             std::bind(&Tcpserver::Err, this, std::placeholders::_1));
                Log_Message(DUBUG, "get a new link , clientip : %s , clientport : %d", ip.c_str(), port);
            }
            else
            {
                if (err == EAGAIN || err == EWOULDBLOCK) // 说明此时listensocket中没有新的连接
                    break;
                else if (err == EINTR) // 说明被信号中断，需要循环继续读
                    continue;
                else
                    break;
            }
        }
    }

    void EnbleReadWrite(Connection *conn, bool read, bool write)
    {
        int event = 0;
        if (read != false)
        {
            event |= EPOLLIN;
        }
        if (write != false)
        {
            event |= EPOLLOUT;
        }
        _epoll.Control(conn->_fd, EPOLL_CTL_MOD, event);
    }

    // 用于给普通套接字对象的读方法
    void Recv(Connection *conn)
    {
        char buffer[1024] = {0};
        for (;;)
        {
            ssize_t n = recv(conn->_fd, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[n] = 0;
                conn->inbuffer += buffer;
                // 处理收到的请求
                _server(conn);
            }
            else if (n == 0)
            {
                if (Is_Connection_Exist(conn->_fd) && conn->_excepter)
                {
                    conn->_excepter(conn);
                }
            }
            else
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                else if (errno == EINTR) // 说明被信号中断，需要循环继续读
                    continue;
                else
                {
                    if (Is_Connection_Exist(conn->_fd) && conn->_excepter)
                    {
                        conn->_excepter(conn);
                    }
                    break;
                }
            }
        }
    }

    // 用于给普通套接字的写方法
    void Send(Connection *conn)
    {
        Log_Message(DUBUG, "进入Send函数");
        int count = 0;

        // 发送数据的时候，需要检查数据有没有发完，即outbuffer中还有没有数据
        while (true)
        {
            sleep(1);
            printf("%d\n", count++);
            ssize_t n = send(conn->_fd, conn->outbuffer.c_str(), conn->outbuffer.size(), 0);
            if (n > 0)
            {
                std::cout<<"进入第一个if"<<std::endl;
                if (conn->outbuffer.empty() == true)
                {
                    break;
                }
                else
                {
                    conn->outbuffer.erase(0,n);
                    continue;
                }
            }
            else
            {
                std::cout<<"进入第二个if"<<std::endl;
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                else if (errno == EINTR) // 说明被信号中断，需要循环继续读
                    continue;
                else
                {
                    if (Is_Connection_Exist(conn->_fd) && conn->_excepter)
                    {
                        conn->_excepter(conn);
                    }
                    break;
                }
            }
        }

        // 说明发送缓冲区已经满了，无法再发送，需要将写入缓冲去设置为EPOLLOUT事件
        if (conn->outbuffer.empty() == false)
        {
            EnbleReadWrite(conn, true, true);
        }
        else
        {
            EnbleReadWrite(conn, true, false);
        }
        Log_Message(DUBUG, "退出Send函数");

        return;
    }

    // 用于给普通套接字的错误方法，对于所有的错误处理，统一将连接关闭
    void Err(Connection *conn)
    {
        // 从epoll模型中删除
        _epoll.Del(conn->_fd);

        // 从哈希表中删除
        _connections.erase(conn->_fd);

        // 关闭文件描述符
        close(conn->_fd);

        // 释放堆空间
        delete conn;

        Log_Message(DUBUG, "delete the connection success");
    }

private:
    Sock _sock;
    epoller _epoll;
    std::unordered_map<int, Connection *> _connections;
    struct epoll_event *_events;
    int _max_num;
    func_t _server; // 处理服务请求
};