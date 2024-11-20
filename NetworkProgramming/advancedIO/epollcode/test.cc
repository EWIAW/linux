#include <memory>

#include "epollserver.hpp"

std::string Handler(std::string str)
{
    std::string tmp = "server reponse : ";
    return tmp + str;
}

int main()
{
    std::unique_ptr<epollserver> svr(new epollserver(Handler));
    svr->init_Server();
    svr->run();
    return 0;
}