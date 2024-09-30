#include <memory>

#include "UdpServer.hpp"

// 程序运行提示
void Usage(string proc)
{
    cout << "Usage:" << proc << "local ip, local prot" << endl;
}

// ./UdpServer port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
    }
    uint16_t port = atoi(argv[1]);
    unique_ptr<UdpServer> usvr(new UdpServer(port));

    usvr->init();
    usvr->start();
    return 0;
}