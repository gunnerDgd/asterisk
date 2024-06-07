#include <io.h>
#include <fs.h>
#include <core.h>
#include <net.h>
#include <thread.h>

#include <stdio.h>

use            (
    dep (thread)
    dep (net)
    dep (io)
)

run_async()                            {
    file *file_1 = make (file) from (0);
    if (!file_create_cstr(file_1, "./test2.txt")) {
    if (!file_open_cstr  (file_1, "./test2.txt")) {
        printf ("Failed to Create File\n");
        return 0;
    }
    }

    fut  *fut1 = file_write(file_1, "Hello World1\n", 14);
    fut  *fut2 = file_write(file_1, "Hello World2\n", 14);
    fut  *fut3 = file_write(file_1, "Hello World3\n", 14);

    printf("File Write0 : %d Bytes\n", await(fut1));
    printf("File Write1 : %d Bytes\n", await(fut2));
    printf("File Write2 : %d Bytes\n", await(fut3));

    u8_t *buf0 = new (u8_t[14]);
    u8_t *buf1 = new (u8_t[14]);
    u8_t *buf2 = new (u8_t[14]);

    fut1 = file_read(file_1, buf0, 14);
    fut2 = file_read(file_1, buf1, 14);
    fut3 = file_read(file_1, buf2, 14);

    printf("File Read0 : %d Bytes - %s", await(fut1), buf0);
    printf("File Read1 : %d Bytes - %s", await(fut2), buf1);
    printf("File Read2 : %d Bytes - %s", await(fut3), buf2);
    drop(buf0)  ;
    drop(buf1)  ;
    drop(buf2)  ;
    del (file_1);
}