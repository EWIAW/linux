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

void Cal(const Request &req, Response &res)
{
    int result = -1;
    int exitcode;
    int x = req._x;
    int y = req._y;
    char op = req._op;
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
    exitcode = 0;
    res._exitcode = exitcode;
    res._result = result;
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