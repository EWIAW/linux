#include<stdio.h>

#define ONE (1<<0)
#define TWO (1<<1)
#define THREE (1<<2)
#define FOUR (1<<3)

void show(int status)
{
    if(status & ONE)
    {
        printf("one\n");
    }
    if(status & TWO)
    {
        printf("two\n");
    }
    if(status & THREE)
    {
        printf("three\n");
    }
    if(status & FOUR)
    {
        printf("four\n");
    }
}

int main()
{
    show(ONE);
    printf("----------\n");
    show(TWO);
    printf("----------\n");
    show(THREE);
    printf("----------\n");
    show(FOUR);
    printf("----------\n");
    show(ONE | TWO | THREE);
    return 0;
}
