#include <iostream>
#include <string>
#include <memory>

#include "calclient.hpp"

using namespace std;

void Usage(string proc)
{
    cout << "Usage:" << proc << "local port" << endl;
}

// ./tcpserver ip port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    unique_ptr<tcpclient> utcp(new tcpclient(ip,port));
    
    utcp->init();
    utcp->start();

    return 0;
}