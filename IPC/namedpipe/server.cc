#include"common.hpp"

int main()
{
    CreatPipe();//创建命名管道

    int fd = open(NAMED_PIPE,O_RDONLY);
    assert(fd != -1);

    char buffer[1024];
    while(true)
    {
        ssize_t ret = read(fd,buffer,sizeof(buffer)-1);
        buffer[ret] = '\0';
        if(ret > 0)
        {
            std::cout<<"client -> server #"<<buffer<<std::endl;
        }
        else if(ret == 0)
        {
            std::cout<<"client quit, me too"<<std::endl;
            break;
        }
    }

    close(fd);

    RemotePipe();//删除命名管道
    return 0;
}