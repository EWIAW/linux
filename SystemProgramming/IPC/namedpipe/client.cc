#include"common.hpp"

int main()
{
    int fd = open(NAMED_PIPE,O_WRONLY);
    assert(fd != -1);

    char buffer[1024];
    while(true)
    {
        std::cout<<"# please input:";
        fgets(buffer,sizeof(buffer)-1,stdin);
        buffer[strlen(buffer)-1] = '\0';
        ssize_t ret = write(fd,buffer,sizeof(buffer)-1);
        if(ret == -1)
        {
            std::cout<<"errno: "<<errno<<" "<<strerror(errno)<<std::endl;
        }
    }

    close(fd);
    return 0;
}