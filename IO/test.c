#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define FILE_NAME(NUM) "log.txt"#NUM

int main()
{
    int fd1 = open(FILE_NAME(1), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    int fd2 = open(FILE_NAME(2), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    int fd3 = open(FILE_NAME(3), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    int fd4 = open(FILE_NAME(4), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666
    int fd5 = open(FILE_NAME(5), O_WRONLY | O_CREAT, 0666);//打开文件,0666指创建的文件默认权限为666

    printf("%d\n",fd1);
    printf("%d\n",fd2);
    printf("%d\n",fd3);
    printf("%d\n",fd4);
    printf("%d\n",fd5);

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
