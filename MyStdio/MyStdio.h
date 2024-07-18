#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

#define SIZE 1024

typedef struct FILE_ 
{
    int fileno;//文件描述符
    char buffer[SIZE];//缓冲区
    int pos;//记录缓冲区末尾的位置，指向\0的位置
}FILE_;

FILE_* fopen_(const char* pathname,const char* mode);//我自己的文件打开函数

void fwrite_(const void* ptr,int sz,FILE_* stream);//我自己的文件写入函数

void fflush_(FILE_* stream);//刷新缓冲区

void fclose_(FILE_* fp);//我自己的文件关闭函数
