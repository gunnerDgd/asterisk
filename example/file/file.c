#include "file.h"
#include "io_sched.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void async_main(task* par_task, io_sched* par) {
    file* file_test = make(file_t) from(2, par, "D:\\test.txt");
    mem   buf       = mem_init(0, 32);
    if (!buf)
        return;
    ptr   buf_ptr   = mem_ptr (buf)  ;

    if (!file_create(file_test)) {
    if (!file_open  (file_test))
        return false_t;
    }

    strcpy(ptr_raw(buf_ptr), "Hello World")   ;
    u64_t res = await(file_write(file_test, buf_ptr, 11));
    printf("Written %d bytes\n", res);
}

int main() {
    io_sched *sched     = make(io_sched_t) from(0);
    task     *main_task = make(task_t)     from(2, async_main, sched);

    io_sched_dispatch(sched, main_task);
    io_sched_run     (sched)           ;
}