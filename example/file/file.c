#include <file.h>
#include <this.h>
#include <io_sched.h>

#include <stdio.h>

void async_main()                                            {
    io_sched *io_sched = make (io_sched_t) from (0)          ;
    file     *file     = make (file_t)     from (1, io_sched);
    if (!file_create_cstr(file, "test2.txt")) {
    if (!file_open_cstr  (file, "test2.txt")) {
        printf ("Failed to Create File\n");
        return false_t;
    }
    }

    fut* fut0 = io_sched_fut(io_sched);
    fut* fut1 = file_write(file, "Hello World\n", 12);
    fut* fut2 = file_write(file, "Hello World\n", 12);
    fut* fut3 = file_write(file, "Hello World\n", 12);
    
    await(fut0);
    printf("File Write0 : %d\n", fut_ret(fut1));
    printf("File Write1 : %d\n", fut_ret(fut2));
    printf("File Write2 : %d\n", fut_ret(fut3));
    return 0;
}