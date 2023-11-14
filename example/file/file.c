#include "file.h"
#include "io_sched.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void async_main(task* par_task, const char* par) {
    file* file_test = make(file_t) from(2, par);
    mem   buf       = mem_init(0, 32);
    ptr   buf_ptr   = mem_ptr (buf)  ;

    strcpy(ptr_raw(buf_ptr), "Hello World")   ;
    await (file_write(file_test, buf_ptr, 11));
}

int main() {
    task     *main_task = make(task_t)     from(2, async_main, "test.txt");
    io_sched *sched     = make(io_sched_t) from(0);

    io_sched_dispatch(sched, main_task);
    io_sched_run     (sched)           ;
}