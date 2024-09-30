#include <memory>

#include "UdpClient.hpp"

void Usage(string proc)
{
    cout << "Usage:" << proc << "server ip,server port" << endl;
}

// ./UdpClient ip port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);

    unique_ptr<UdpClient> usvr(new UdpClient(serverip, serverport));
    usvr->init();
    usvr->start();
    return 0;
}