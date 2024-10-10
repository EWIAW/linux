#include <iostream>
#include <string>
#include <memory>

#include "tcpserver.hpp"

using namespace std;

void Usage(string proc)
{
    cout << "Usage:" << proc << "local port" << endl;
}

// ./tcpserver 8080
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    uint16_t port = atoi(argv[1]);

    unique_ptr<tcpserver> utcp(new tcpserver(port));

    utcp->init();
    utcp->start();

    return 0;
}