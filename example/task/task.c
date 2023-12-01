#include <run.h>
#include <task.h>
#include <async.h>

#include <stdio.h>

void* test() { printf("Hello Test\n"); return 1; }

run()                       {
    task  a = async(test, 0);
          a = async(test, 0); 
    printf("Hello World (RET : %d)\n", await(a));
}