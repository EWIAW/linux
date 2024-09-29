#include "common.hpp"

int main()
{
    //1.获取共享内存
    key_t key = GetKey();
    int id = GetShm(key);

    //1.连接共享内存
    void* start = AttachShm(id);

    //2.使用
    char message[1024] = "我正在给你发消息";
    int cnt = 0;
    int count = 10;
    while(count--)
    {
        snprintf((char*)start,SIZE,"%s:%d",message,cnt++);
        sleep(1);
    }

    //3.断开连接共享内存
    DisattchShm(start);

    return 0;
}