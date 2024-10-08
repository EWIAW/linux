#include <stdio.h>

#include <memory>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include <signal.h>

#include "UdpServer.hpp"
#include "OnlineUser.hpp"

using namespace std;

string File = "dict.dat";             // 存储翻译信息的文件名
unordered_map<string, string> umdict; // 全局哈希字典
static const int buffernum = 1024;
OnlineUser OU; // 用户存储demo3的用户信息的哈希表

// 程序运行提示
void Usage(string proc)
{
    cout << "Usage:" << proc << "local ip, local prot" << endl;
}

// 分割字符串
bool CutString(const string &line, string &key, string &value)
{
    char identifier = ':';
    const auto &it = line.find(identifier, 0);
    if (it == string::npos)
    {
        return false;
    }
    else
    {
        key = line.substr(0, it);
        value = line.substr(it + 1, string::npos);
    }
    return true;
}

// 初始化字典
void InitDict()
{
    ifstream in(File, ios::binary); // 打开文件
    if (!in.is_open())              // 说明文件打开失败
    {
        cerr << "open file " << File << "errno" << endl;
        exit(OPEN_ERR);
    }

    string line;
    string key, value;
    while (getline(in, line))
    {
        if (CutString(line, key, value))
        {
            umdict.insert(make_pair(key, value));
        }
    }

    in.close(); // 关闭文件

    // 测试初始化字典是否成功
    // auto it = umdict.begin();
    // while (it != umdict.end())
    // {
    //     cout << it->first << ":" << it->second << endl;
    //     it++;
    // }
}

// 捕捉2号信号，使其可以进行热加载
void ReLoad(int signo)
{
    InitDict();
}

// demo1 翻译器：客户端发一个单词，服务端解析单词给出中文
void Translation(int sockfd, const string &ip, const uint16_t &port, string message)
{
    InitDict();                       // 初始化哈希字典
    string respone_string = "unknow"; // 返回信息
    // 查找字典中是否存储该单词
    auto it = umdict.find(message);
    if (it != umdict.end())
    {
        respone_string = it->second;
    }

    // 将结果发送回去
    struct sockaddr_in client;
    client.sin_addr.s_addr = inet_addr(ip.c_str());
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    sendto(sockfd, respone_string.c_str(), respone_string.size(), 0, (struct sockaddr *)&client, sizeof(client));
}

// demo2 远程命令行
void RemoteCommandLine(int sockfd, const string &ip, const uint16_t &port, string cmd)
{
    string response; // 需要返回的字符串内容]

    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == nullptr)
    {
        cerr << "popen errno" << endl;
        exit(POPEN_ERR);
    }

    char buffer[buffernum];
    struct sockaddr_in client;
    client.sin_addr.s_addr = inet_addr(ip.c_str());
    client.sin_family = AF_INET;
    client.sin_port = htons(port);

    while (fgets(buffer, sizeof(buffer), fp))
    {
        response += buffer;
    }
    sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr *)&client, sizeof(client));

    int n = pclose(fp);
    if (n == -1)
    {
        cerr << "pclose errno" << endl;
        exit(PCLOSE_ERR);
    }
}

// demo3 群聊功能
void GroupChat(int sockfd, const string &ip, const uint16_t &port, string message)
{
    // 上线
    if (message == "online")
    {
        OU.AddUser(ip, port);
        return;
    }
    // 下线
    if (message == "unonline")
    {
        OU.DeleteUser(ip, port);
        return;
    }

    if (OU.Is_Online(ip, port) == true) // 说明已经在线
    {
        OU.SendAll(sockfd, ip, port, message); // 消息路由
    }
    else
    {
        struct sockaddr_in client;
        client.sin_addr.s_addr = inet_addr(ip.c_str());
        client.sin_family = AF_INET;
        client.sin_port = htons(port);
        string response = "你还没有上线，请输出online上线";
        sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr *)&client, sizeof(client));
    }
}

// ./UdpServer port
int main(int argc, char *argv[])
{
    // 捕捉2号信号，使其可以进行热加载
    signal(2, ReLoad);

    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    // unique_ptr<UdpServer> usvr(new UdpServer(Translation, port));
    // unique_ptr<UdpServer> usvr(new UdpServer(RemoteCommandLine, port));
    unique_ptr<UdpServer> usvr(new UdpServer(GroupChat, port));

    usvr->init();
    usvr->start();
    return 0;
}