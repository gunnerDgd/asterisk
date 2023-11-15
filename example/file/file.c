#include "file.h"
#include "io_sched.h"
#include "box.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

io_sched_main(par_task, par) {
    file *file_test = make(file_t) from(2, par, "test.txt");
    box  *buf       = make(box_t)  from(1, 64)             ; 
    if  (!buf)        {
        del(file_test);
        return;
    }

    ptr ptr = box_ptr(buf, 0);

    if (!file_create(file_test)) {
    if (!file_open  (file_test))
        return false_t;
    }

    ptr_write(ptr, "Hello World\n", 12);
    task *task_1 = file_write(file_test, ptr, 12),
         *task_2 = file_write(file_test, ptr, 12),
         *task_3 = file_write(file_test, ptr, 12);

    printf("Written %lld bytes\n", await_as(task_1, u64_t));
    printf("Written %lld bytes\n", await_as(task_2, u64_t));
    printf("Written %lld bytes\n", await_as(task_3, u64_t));

    del(task_1);
    del(task_2);
    del(task_3);

    return 0;
}