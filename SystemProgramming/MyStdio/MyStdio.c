#include"MyStdio.h"

FILE_* fopen_(const char* pathname,const char* mode)
{
    //判断文件的打开方式
    int status = 0;
    if(strcmp(mode,"w") == 0)
    {
        status |= O_WRONLY | O_CREAT | O_TRUNC;
    }
    else if(strcmp(mode,"r") == 0)
    {
        status |= O_RDONLY;
    }
    else if(strcmp(mode,"a") == 0)
    {
        status |= O_WRONLY | O_CREAT | O_APPEND;
    }

    //判断打开文件时，是否需要创建文件
    int fd = 0;//存储文件描述符
    if((status | O_WRONLY) > 0)
    {
        fd = open(pathname,status,0666);
    }
    else 
    {
        fd = open(pathname,status);
    }
    
    //说明文件打开失败
    if(fd < 0)
    {
        const char* err = strerror(errno);
        printf("open:%s",err);
        return NULL;
    }

    FILE_* file = (FILE_*)malloc(sizeof(FILE_));//开辟文件FILE_结构体

    file->fileno =  fd;
    memset(file->buffer,0,sizeof(file->buffer));//将缓冲区数组中的内容全部置0
    file->pos = 0;
    return file;
}

void fflush_(FILE_* stream)
{
    if(stream->pos > 0)//说明缓冲区中有数据，将缓冲区中全部数据写入
    {
        write(stream->fileno,stream->buffer,stream->pos);
        fsync(stream->fileno);
    }
    stream->pos = 0;//将缓冲区末尾位置置0
}

void fwrite_(const void* ptr,int sz,FILE_* stream)
{
    strcpy((char*)stream->buffer + stream->pos,(char*)ptr);
    stream->pos += sz;

    //如果缓冲区中有\n，说明符合行缓冲，进程缓冲区的刷新
    if(stream->buffer[stream->pos-1] == '\n')
    {
        fflush_(stream);
    }
    
    //如果缓冲区满了，进行缓冲区刷新
    if(stream->pos > 100)
    {
        fflush_(stream);
    }

}

void fclose_(FILE_* fp)
{
    fflush_(fp);//文件关闭前，刷新缓冲区
    close(fp->fileno);
    free(fp);//关闭文件最后，释放内存
}
