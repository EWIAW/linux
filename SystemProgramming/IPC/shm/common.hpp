#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <iostream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define PATHNAME "."
#define PROJ_ID  1
#define SIZE     4096

//获取key值
key_t GetKey()
{
    key_t id = ftok(PATHNAME,PROJ_ID);
    if(id == -1)
    {
        std::cerr<<"生成key值失败: "<<strerror(errno)<<std::endl;
        exit(1);
    }
    return id;
}

//创建共享内存
int CreatShm(key_t key)
{
    int ret = shmget(key,SIZE,IPC_CREAT | IPC_EXCL | 0666);
    if(ret == -1)
    {
        std::cerr<<"创建共享内存失败: "<<strerror(errno)<<std::endl;
        exit(2);
    }
    return ret;
}

//获取共享内存
int GetShm(key_t key)
{
    int ret = shmget(key,SIZE,IPC_CREAT);
    if(ret == -1)
    {
        std::cerr<<"获取共享内存失败: "<<strerror(errno)<<std::endl;
        exit(3);
    }
    return ret;
}

//连接共享内存
void* AttachShm(int shmid)
{
    void* ret = shmat(shmid,nullptr,0);
    if((long long)ret == -1)
    {
        std::cerr<<"连接共享内存失败: "<<strerror(errno)<<std::endl;
        exit(4);
    }
    return ret;
}

//断开连接共享内存
void DisattchShm(void* start)
{
    int ret = shmdt(start);
    if(ret == -1)
    {
        std::cerr<<"断开共享内存失败: "<<strerror(errno)<<std::endl;
        exit(5);
    }
}

//删除共享内存
void DeleteShm(int shmid)
{
    int ret = shmctl(shmid,IPC_RMID,nullptr);
    if(ret == -1)
    {
        std::cerr<<"删除共享内存失败: "<<strerror(errno)<<std::endl;
        exit(6);
    }
}
