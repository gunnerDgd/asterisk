#include <core.h>
#include <thread.h>
#include <io.h>
#include <fs.h>

use            (
    dep (thread)
    dep (io)
)

run_async()                               {
    file *file_1 = new_file ("./test.txt");

    fut  *fut1 = file_write(file_1, "Hello World1\n", 14);
    fut  *fut2 = file_write(file_1, "Hello World2\n", 14);
    fut  *fut3 = file_write(file_1, "Hello World3\n", 14);

    println("File Write0 : %d Bytes", await(fut1));
    println("File Write1 : %d Bytes", await(fut2));
    println("File Write2 : %d Bytes", await(fut3));

    u8_t *buf0 = new (u8_t[14]);
    u8_t *buf1 = new (u8_t[14]);
    u8_t *buf2 = new (u8_t[14]);

    fut1 = file_read(file_1, buf0, 14);
    fut2 = file_read(file_1, buf1, 14);
    fut3 = file_read(file_1, buf2, 14);

    println("File Read0 : %d Bytes - %s", await(fut1), buf0);
    println("File Read1 : %d Bytes - %s", await(fut2), buf1);
    println("File Read2 : %d Bytes - %s", await(fut3), buf2);
    drop(buf0)  ;
    drop(buf1)  ;
    drop(buf2)  ;
    del (file_1);
}