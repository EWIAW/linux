#include"MyStdio.h"

#define FILE_NAME "log.txt"

int main()
{
    FILE_* fp = fopen_(FILE_NAME,"a");
    
    const char* arr = "hello linux\n";

    fwrite_(arr,strlen(arr),fp);

    fclose_(fp);
    return 0;
}
