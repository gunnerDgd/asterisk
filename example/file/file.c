#include "file.h"
#include "io_sched.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

io_sched_main(par_task, par) {
    file* file_test = make(file_t) from(2, par, "test.txt");
    mem   buf       = mem_init(0, 32) ; if (!buf) return;
    ptr   ptr       = mem_ptr (buf, 0);

    if (!file_create(file_test)) {
    if (!file_open  (file_test))
        return false_t;
    }

    ptr_write(ptr, "Hello World\n", 12);
    task *task_1 = file_write(file_test, ptr, 12),
         *task_2 = file_write(file_test, ptr, 12),
         *task_3 = file_write(file_test, ptr, 12);

    printf   ("Written %lld bytes\n", (u64_t)await(task_1));
    printf   ("Written %lld bytes\n", (u64_t)await(task_2));
    printf   ("Written %lld bytes\n", (u64_t)await(task_3));

    del(task_1);
    del(task_2);
    del(task_3);

    return 0;
}