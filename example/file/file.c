#include <asterisk/core.h>
#include <asterisk/fs/file.h>

#include <stdio.h>

void async_main()                      {
    file *file = make (file_t) from (0);
    if (!file_create_cstr(file, "test2.txt")) {
    if (!file_open_cstr  (file, "test2.txt")) {
        printf ("Failed to Create File\n");
        return;
    }
    }

    u8_t* buf0 = new (u8_t[12]);
    u8_t* buf1 = new (u8_t[12]);
    u8_t* buf2 = new (u8_t[12]);
    fut* fut1 = file_read(file, buf0, 12);
    fut* fut2 = file_read(file, buf1, 12);
    fut* fut3 = file_read(file, buf2, 12);
    await_all(3, fut1, fut2, fut3);

    printf("File Read0 : %d Bytes (%s)\n", fut_ret(fut1), buf0);
    printf("File Read1 : %d Bytes (%s)\n", fut_ret(fut2), buf1);
    printf("File Read2 : %d Bytes (%s)\n", fut_ret(fut3), buf2);
    drop(buf0);
    drop(buf1);
    drop(buf2);
    del (file);
    return;
}