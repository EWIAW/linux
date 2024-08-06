#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main()
{
    while(true)
    {
        cout<<"hello linux: "<<getpid()<<endl;
        sleep(1);
        // raise(9);
    }
    return 0;
}