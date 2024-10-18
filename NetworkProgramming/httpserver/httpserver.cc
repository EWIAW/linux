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
    std::cout << "---------------处理过的request请求---------------" << std::endl;
    std::cout << "method:" << request._method << std::endl;
    std::cout << "url:" << request._url << std::endl;
    std::cout << "path:" << request._path << std::endl;
    std::cout << "httpversion:" << request._httpversion << std::endl;
    std::cout << "suffix:" << request._suffix << std::endl;
    std::cout << "size:" << request._size << std::endl;
    std::cout << "---------------处理过的request请求---------------" << std::endl;

    std::string responeline = "HTTP/1.1 200 OK\r\n";
    std::string responeheader = Util::suffixToDesc(request._suffix); // 获取内容类型，例如html文件，jpg图片等等
    responeheader += "Content-Length: ";                             // 获取正文长度
    responeheader += std::to_string(request._size);
    responeheader += "\r\n";
    std::string blank = "\r\n";
    // std::string body = "<!DOCTYPE html><html lang=\" en \"><head><meta charset=\"UTF-8\"><title>my test</title></head><body>各位黑神话的大佬们好呀，就在刚刚，黑神话主创杨奇深夜发文，感慨一路走来艰辛的同时，还表示重新上路！同时在他们内部音乐会结束时，冯骥杨奇等主创在演讲时也表示，在他们休息一个月后，又要强度工作了，杨奇开玩笑说，要请假的赶紧，后面不好请了！可以说游科员工们的假期正式结束，准备开工，优先做DLC还是做新项目有待确认，大概率会是双开工，也就是同时做DLC和钟馗或姜子牙！</body></html>";

    std::string body;
    if (!Util::readFile(request._path, body)) // 读取要获取的正文内容，即html页面
    {
        Util::readFile(html_404, body);
    };
    reponse._outbuffer = responeline + responeheader + blank + body;

    return true;
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
    utcp->start();

    return 0;
}