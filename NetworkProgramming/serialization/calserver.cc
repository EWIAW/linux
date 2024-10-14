#include <iostream>
#include <string>
#include <memory>

#include "calserver.hpp"
#include "protocol.hpp"

using namespace std;

enum
{
    OK = 0,
    DIV_ZERO,
    MOD_ZERO,
    OP_ERROR
};

void Usage(string proc)
{
    cout << "Usage:" << proc << "local port" << endl;
}

void Cal(Request &req, Response &res)
{
    int result;
    int exitcode;
    char op = req.getOp();
    int x = req.getX();
    int y = req.getY();
    switch (op)
    {
    case '+':
        result = x + y;
        break;
    case '-':
        result = x - y;
        break;
    case '*':
        result = x * y;
        break;
    case '/':
        result = x / y;
        break;
    case '%':
        result = x % y;
        break;
    default:
        exitcode = 1;
        break;
    }
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
    utcp->start(Cal);

    return 0;
}