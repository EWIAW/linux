#pragma once
#include <cstring>

#include <vector>
#include <string>
#include <unordered_map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// 用户User类
class User
{
public:
    User(const string &ip, const uint16_t &port)
        : _ip(ip), _port(port)
    {
        // 通过ip和端口号为用户生成唯一的用户名
        string name;
        name += _ip;
        name += ":";
        name += to_string(_port);
        _name = name;
    }

    // 获取用户id
    uint16_t GetPort()
    {
        return _port;
    }

    // 获取用户port
    string GetIp()
    {
        return _ip;
    }

    // 获取用户名
    string GetName()
    {
        return _name;
    }

    ~User()
    {
    }

private:
    string _ip;     // 用户id
    uint16_t _port; // 用户端口号
    string _name;   // 用户名
};

// 用户上线类，用来管理存储用户的类
class OnlineUser
{
public:
    OnlineUser()
    {
    }

    // 用户上线后，添加到um哈希表中
    void AddUser(const string &ip, const uint16_t &port)
    {
        User user(ip, port);                            // 构造一个用户
        umUser.insert(make_pair(user.GetName(), user)); // 将用户插入到哈希表中
    }

    // 用户下线后，从um哈希表中删除
    void DeleteUser(const string &ip, const uint16_t &port)
    {
        User user(ip, port);
        umUser.erase(user.GetName());
    }

    // 判断用户是否已经上线
    bool Is_Online(const string &ip, const uint16_t &port)
    {
        User user(ip, port);
        unordered_map<string, User>::iterator it = umUser.find(user.GetName());
        if (it == umUser.end())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    // 消息路由，消息路由时，也要给服务端发送一份
    void SendAll(const int &sockfd, const string &ip, const uint16_t &port, const string &message) // 这里的ip和port为发出消息的客户端
    {
        // 进行消息路由前，先封装一下要发送的信息，即在信息前面带上ip和port
        string respone;
        respone = "[" + ip + ":" + to_string(port) + "]" + message;

        struct sockaddr_in otherclient;
        User user(ip, port); // 发信息的客户端用户对象
        auto it = umUser.begin();
        while (it != umUser.end())
        {
            if (it != umUser.find(user.GetName()))
            {
                otherclient.sin_addr.s_addr = inet_addr(it->second.GetIp().c_str());
                otherclient.sin_family = AF_INET;
                otherclient.sin_port = htons(it->second.GetPort());
                sendto(sockfd, respone.c_str(), respone.size(), 0, (struct sockaddr *)&otherclient, sizeof(otherclient));
            }
            it++;
        }

        // 消息路由给服务端
    }

    ~OnlineUser()
    {
    }

private:
    unordered_map<string, User> umUser; // 用哈希表存储用户对象，其中string为用户的名称
};