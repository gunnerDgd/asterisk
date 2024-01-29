#include <this.h>
#include <stdio.h>

void* 
    test()                    {
        static int count = 0;
        printf("Hello Test\n"); 
        return ++count; 
}

void async_main()                      {
    fut* a = 0; 
    a = async(test, 0); printf("Hello World (RET : %d)\n", await(a));
    a = async(test, 0); printf("Hello World (RET : %d)\n", await(a));
    a = async(test, 0); printf("Hello World (RET : %d)\n", await(a));
    a = async(test, 0); printf("Hello World (RET : %d)\n", await(a));
    a = async(test, 0); printf("Hello World (RET : %d)\n", await(a));
    a = async(test, 0); printf("Hello World (RET : %d)\n", await(a));
}