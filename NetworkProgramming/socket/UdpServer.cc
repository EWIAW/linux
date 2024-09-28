#include <memory>

#include "UdpServer.hpp"

int main()
{
    unique_ptr<UdpServer> usvr(new UdpServer(8080));
    return 0;
}