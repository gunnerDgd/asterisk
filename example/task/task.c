#include <this.h>
#include <stdio.h>

void* 
    test()                    { 
        printf("Hello Test\n"); 
        return 1; 
}

void async_main()                      {
    fut* a = async(test, 0);
    printf("Hello World (RET : %d)\n", await(a));
}