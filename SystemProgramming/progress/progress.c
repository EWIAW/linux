#include"progress.h"

void Progress()
{
    int cnt=0;
    char arr[101]={0};
    char tmp[4]={'|','/','~','\\'};

    while(cnt<100)
    {
        arr[cnt]='#';
        printf("[%-100s][%d%%][%c]\r",arr,cnt+1,tmp[cnt%4]);
        fflush(stdout);
        cnt++;
        usleep(50000);
    }
    printf("\n");
}
