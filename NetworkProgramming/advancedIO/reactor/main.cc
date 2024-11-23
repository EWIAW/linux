#include <memory>

#include "tcpserver.hpp"
#include "protocol.hpp"

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

void Handler_Task(Connection *conn)
{
    Log_Message(DUBUG, "进入Handler_Task函数");

    std::string text;
    bool ret = Parse(conn->inbuffer, text);
    if (ret == false)
    {
        return;
    }

    // 将请求的报头去掉
    std::string out;
    deLength(text, out);

    // std ::cout << out << std::endl;

    Request request;
    request.deSerialization(out);
    Response response;

    Cal(request, response);

    response.Serialization(out);
    conn->outbuffer += enLength(out);

    std::cout << "--------------------返回的数据是 : " << out << std::endl;

    // 将数据直接发送回去
    if (conn->_sender)
        conn->_sender(conn);

    Log_Message(DUBUG, "退出Handler_Task函数");
}

int main()
{
    std::unique_ptr<Tcpserver> svr(new Tcpserver());
    svr->init_Server(Handler_Task);
    svr->run();
    return 0;
}