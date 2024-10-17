#include <iostream>
#include <string>
#include <memory>
#include <functional>

#include "httpserver.hpp"
#include "protocol.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage:\n"
              << proc << "local port" << std::endl;
}

bool ProcessTask(const HttpRequest &request, HttpResponse &reponse)
{
    std::cout << "---------------start---------------" << std::endl;
    std::cout << request.inbuffer << std::endl;
    std::cout << "----------------end----------------" << std::endl;

    std::string responeline = "HTTP/1.1 200 OK\r\n";
    std::string responeheader = "Content-Type: text/html\r\n";
    std::string blank = "\r\n";
    std::string body = "<!DOCTYPE html><html lang=\" en \"><head><meta charset=\"UTF-8\"><title>my test</title></head><body>各位黑神话的大佬们好呀，就在刚刚，黑神话主创杨奇深夜发文，感慨一路走来艰辛的同时，还表示重新上路！同时在他们内部音乐会结束时，冯骥杨奇等主创在演讲时也表示，在他们休息一个月后，又要强度工作了，杨奇开玩笑说，要请假的赶紧，后面不好请了！可以说游科员工们的假期正式结束，准备开工，优先做DLC还是做新项目有待确认，大概率会是双开工，也就是同时做DLC和钟馗或姜子牙！</body></html>";

    reponse.outbuffer = responeline + responeheader + blank + body;
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

    std::unique_ptr<httpserver> utcp(new httpserver(port, ProcessTask));

    utcp->init();
    utcp->start(ProcessTask);

    return 0;
}