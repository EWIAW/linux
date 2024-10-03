#include <memory>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include "UdpServer.hpp"

using namespace std;

string File = "dict.dat";
unordered_map<string, string> umdict; // 全局哈希字典

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
    ifstream in(File, ios::binary);
    if (!in.is_open()) // 说明文件打开失败
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
    // 测试初始化字典是否成功
    // auto it = umdict.begin();
    // while (it != umdict.end())
    // {
    //     cout << it->first << ":" << it->second << endl;
    //     it++;
    // }
}

// demo1 翻译器
void Translation(int sockfd, const string &ip, const uint16_t &port, string message)
{
    InitDict(); // 初始化哈希字典
    string respone_string = "unknow";
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

// ./UdpServer port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    unique_ptr<UdpServer> usvr(new UdpServer(Translation, port));

    usvr->init();
    usvr->start();
    return 0;
}