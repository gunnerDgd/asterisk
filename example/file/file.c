#include <asterisk/core.h>
#include <asterisk/fs/file.h>

#include <stdio.h>

void async_main()                      {
    file *file = make (file_t) from (0);
    if (!file_create_cstr(file, "test2.txt")) {
    if (!file_open_cstr  (file, "test2.txt")) {
        printf ("Failed to Create File\n");
        return false_t;
    }
    }

    fut* fut1 = file_write(file, "Hello World\n", 12);
    fut* fut2 = file_write(file, "Hello World\n", 12);
    fut* fut3 = file_write(file, "Hello World\n", 12);
    await_all(3, fut1, fut2, fut3);

    printf("File Write0 : %d\n", fut_ret(fut1));
    printf("File Write1 : %d\n", fut_ret(fut2));
    printf("File Write2 : %d\n", fut_ret(fut3));

    del(file);
    return  0;
}