#include <io.h>
#include <fs.h>
#include <core.h>

#include <stdio.h>

void run_async()                                               {
    io_sched *io_sched_1 = make (io_sched) from (0)            ;
    file     *file_1     = make (file)     from (1, io_sched_1);
    if (!file_create_cstr(file_1, "./test2.txt")) {
    if (!file_open_cstr  (file_1, "./test2.txt")) {
        printf ("Failed to Create File (%d)\n");
        return;
    }
    }

    u8_t *buf0 = new (u8_t[14]);
    u8_t *buf1 = new (u8_t[14]);
    u8_t *buf2 = new (u8_t[14]);
    fut  *fut1 = file_read(file_1, buf0, 13);
    fut  *fut2 = file_read(file_1, buf1, 13);
    fut  *fut3 = file_read(file_1, buf2, 13);
    await(fut1);
    await(fut2);
    await(fut3);

    printf("File Read0 : %d Bytes - %s\n", fut_ret(fut1), buf0);
    printf("File Read1 : %d Bytes - %s\n", fut_ret(fut2), buf1);
    printf("File Read2 : %d Bytes - %s\n", fut_ret(fut3), buf2);
    drop(buf0) ;
    drop(buf1) ;
    drop(buf2) ;

    del (io_sched_1);
    del (file_1)    ;
}