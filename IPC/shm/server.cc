#include "common.hpp"

int main()
{
    //1.创建共享内存
    key_t key = GetKey();
    int id = CreatShm(key);

    //2.连接共享内存
    void* start = AttachShm(id);

    //3.使用
    int count = 10;
    while(count--)
    {
        printf("client say: ");
        printf("%s\n",start);
        struct shmid_ds ds;
        shmctl(id,IPC_STAT,&ds);
        printf("共享内存的大小：%d,最后一次连接时间：%d，key值：%d",ds.shm_segsz,ds.shm_atime,ds.shm_perm.__key);
        sleep(1);
    }

    //4.断开连接共享内存
    DisattchShm(start);

    //5.删除共享内存
    DeleteShm(id);

    return 0;
}