#include <memory>

#include "socket.hpp"
#include "log.hpp"
#include "selectserver.hpp"

int main()
{
    std::unique_ptr<server> svr(new server);
    svr->initServer();
    svr->run();
    return 0;
}