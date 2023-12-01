#include <file.h>
#include <run.h>
#include <async.h>

#include <stdio.h>

run()                                 {
    file *file = make(file_t) from (0);
    if (!file_create (file, "test2.txt")) {
    if (!file_open   (file, "test2.txt"))
        return false_t;
    }

    printf("Written %d Bytes\n", await(file_write(file, "Hello World\n", 12)));
    printf("Written %d Bytes\n", await(file_write(file, "Hello World\n", 12)));
    printf("Written %d Bytes\n", await(file_write(file, "Hello World\n", 12)));

    return 0;
}