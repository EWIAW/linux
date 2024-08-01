#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>

#define FILE_NAME "log.txt"

int main()
{
    printf("printf :hello linux\n");
    fprintf(stdout,"fprintf:hello linux\n");
    fputs("fputs  :hello linux\n",stdout);

    const char arr[100] = "write  :hello linux\n";
    write(1,arr,strlen(arr));

    fork();

    //int fd = open(FILE_NAME,O_WRONLY | O_CREAT | O_TRUNC,0666);
    //assert(fd != -1);
    //dup2(fd,1);//将文件log.txt的文件描述符拷贝给1号文件描述符

    //printf("hello linux\n");

    //close(fd);//关闭文件

    //close(0);
    //int fd = open(FILE_NAME,O_RDONLY);
    //assert(fd != -1);

    //char arr[100];
    //scanf("%s",&arr);
    //puts(arr);
   // close(1);//把stdout关掉

   // int fd = open(FILE_NAME,O_WRONLY | O_CREAT | O_TRUNC,0666);//以写的形式打开文件，并且每次打开文件时都清空内容
   // assert(fd != -1);

   // printf("fd:%d\n",fd);

   // fflush(stdout);

   // close(fd);

    //close(0);//在打开文件之前，把stdin文件关闭掉

    //int fd = open(FILE_NAME,O_RDONLY);//已读的形式打开文件
    //assert(fd != -1);

    //printf("fd:%d\n",fd);

    //close(fd);
    //int fd1 = open(FILE_NAME(1), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    //int fd2 = open(FILE_NAME(2), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    //int fd3 = open(FILE_NAME(3), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    //int fd4 = open(FILE_NAME(4), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    //int fd5 = open(FILE_NAME(5), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666

    //printf("%d\n",fd1);
    //printf("%d\n",fd2);
    //printf("%d\n",fd3);
    //printf("%d\n",fd4);
    //printf("%d\n",fd5);

    //char arr[100];
    //read(fd,arr,sizeof(arr)-1);
    //arr[strlen(arr)] = 0;
    //printf("%s\n",arr);

    //close(fd);


    //write操作
   // char arr[100];
   // int cnt = 5;
   // while(cnt)
   // {
   //     sprintf(arr,"%s:%d\n","aaaaa",cnt--);
   //     write(fd,arr,strlen(arr));
   // }

   // close(fd);//关闭文件

    //  FILE* fp = fopen(FILE_NAME,"r"); /  if(fp == NULL) /  {
  //      perror("fopen");
  //  }

 //  char arr[100];
 //  while(fgets(arr,sizeof(arr)-1,fp) != NULL)
 //  {
 //      arr[strlen(arr)-1] = 0;
  //      puts(arr);
 //  }

   // int cnt = 5;
   // while(cnt)
   // {
   //     fprintf(fp,"%s:%d\n","hello linux",cnt--);
   // }

   // fclose(fp);

    return 0;
}
