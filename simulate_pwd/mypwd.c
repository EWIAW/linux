#include<stdio.h>
#include<stdlib.h>

int main()
{
    char* mypwd=getenv("PWD");
    printf("%s\n",mypwd);
    return 0;
}
