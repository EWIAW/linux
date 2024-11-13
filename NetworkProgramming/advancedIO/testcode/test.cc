#include <iostream>

#include <unistd.h>

#include "util.hpp"

using namespace std;

void OtherTask()
{
    cout << "Down..." << endl;
}

int main()
{
    Util::setNonBlock(0); // 将标准输入流设置为非阻塞状态
    char buffer[32];
    while (true)
    {
        cout << ">>>";
        fflush(stdout);
        ssize_t n = read(0, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n - 1] = 0;
            cout << "#echo:" << buffer << endl;
        }
        else if (n == 0) // 说明读到末尾
        {
            cout << "read end!!!" << endl;
            break;
        }
        else // 走到这里说明，要么读取失败，要么非阻塞没读到，但是它们的read的返回值都是-1，那么我们怎么区分这两种情况呢
        {
            if (errno == EAGAIN)
            {
                cout << "没有发生错误，底层没有资源就绪！！！" << endl;
                OtherTask();
            }
            else if (errno == EINTR)
            {
                // 由于信号引发的读取中断，下次重新读取
                continue;
            }
            else
            {
                cout << "read error:" << strerror(errno) << endl;
                break;
            }
        }
        sleep(1);
    }

    return 0;
}