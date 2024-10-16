#include <iostream>
#include <string>
#include <memory>

#include "calserver.hpp"
#include "protocol.hpp"

using namespace std;

enum
{
    OK = 0,
    DIV_ZERO, // 除0
    MOD_ZERO, // 取模0
    OP_ERROR  // 运算符错误
};

void Usage(string proc)
{
    cout << "Usage:" << proc << "local port" << endl;
}

void Cal(const Request &req, Response &res)
{
    int result = -1;
    int exitcode = OK;
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
        if (y == 0)
            exitcode = DIV_ZERO;
        else
            result = x / y;
        break;
    case '%':
        if (y == 0)
            exitcode = MOD_ZERO;
        else
            result = x % y;
        break;
    default:
        exitcode = OP_ERROR;
        break;
    }
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