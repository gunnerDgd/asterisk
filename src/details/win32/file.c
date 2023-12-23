#include "file.h"

#include "curr.h"
#include "thd.h"

#include "io_sched.h"
#include "io_task.h"

#include "sched.h"
#include "task.h"

obj_trait file_trait     = {
    .on_new   = &file_new  ,
    .on_clone = &file_clone,
    .on_ref   =           0,
    .on_del   = &file_del  ,
    .size     = sizeof(file)
};

obj_trait* file_t = &file_trait;

bool_t
    file_new
        (file* par_file, u32_t par_count, va_list par) {
            par_file->file     = INVALID_HANDLE_VALUE                                           ;
            par_file->io_sched = (par_count == 0) ? &curr_thd->io_sched : va_arg(par, io_sched*);

            return par_file->io_sched != NULL;
}

bool_t 
    file_clone
        (file* par, file* par_clone) {
            return false_t;
}

void   
    file_del
        (file* par)                      {
            CloseHandle(par->file)         ;
            CloseHandle(par->file_io_sched);
            del        (par->io_sched)     ;
}

bool_t
    file_open
        (file* par, const char* par_name)                        {
            if (!par)                              return false_t;
            if (trait_of(par) != file_t)           return false_t;
            if (par->file != INVALID_HANDLE_VALUE) return false_t;

            par->file = CreateFile          (
                par_name                    ,
                GENERIC_READ | GENERIC_WRITE,
                0                           ,
                0                           ,
                OPEN_EXISTING               ,
                FILE_FLAG_OVERLAPPED        ,
                0
            );

            if (par->file == INVALID_HANDLE_VALUE) return false_t;
            par->file_io_sched = CreateIoCompletionPort (
                par->file              ,
                par->io_sched->io_sched,
                par->io_sched          ,
                0
            );

            if(!par->file_io_sched)             {
                CloseHandle(par->file)          ;
                par->file = INVALID_HANDLE_VALUE;
                return false_t                  ;
            }

            return true_t;
}

bool_t 
    file_create
        (file* par, const char* par_name)                        {
            if (!par)                              return false_t;
            if (trait_of(par) != file_t)           return false_t;
            if (par->file != INVALID_HANDLE_VALUE) return false_t;

            par->file = CreateFile          (
                par_name                    ,
                GENERIC_READ | GENERIC_WRITE,
                0                           ,
                0                           ,
                CREATE_NEW                  ,
                FILE_FLAG_OVERLAPPED        ,
                0
            );

            if (par->file == INVALID_HANDLE_VALUE) return false_t;
            par->file_io_sched = CreateIoCompletionPort (
                par->file              ,
                par->io_sched->io_sched,
                par->io_sched          ,
                0
            );

            if(!par->file_io_sched)             {
                CloseHandle(par->file)          ;
                par->file = INVALID_HANDLE_VALUE;
                return false_t                  ;
            }

            return true_t;
}

void
    file_close
        (file* par)                               {
            if(par->file != INVALID_HANDLE_VALUE) {
                CloseHandle(par->file)    ;
                CloseHandle(par->io_sched);

                par->file = INVALID_HANDLE_VALUE;
            }
}

struct io_task*
    file_read
        (file* par, u8_t* par_buf, u64_t par_len)          {
            if (!par)                              return 0;
            if (!par_buf)                          return 0;
            if (par->file == INVALID_HANDLE_VALUE) return 0;
            if (trait_of(par) != file_t)           return 0; io_task *ret = io_sched_dispatch(par->io_sched);
            if (!ret)                              return 0;

            ret->state              = io_task_exec;
            ret->io_task.Offset     = 0xFFFFFFFF  ;
            ret->io_task.OffsetHigh = 0xFFFFFFFF  ;
            bool_t res = ReadFile (
                par->file    ,
                par_buf      ,
                par_len      , 
                0            ,
                &ret->io_task
            );

            if (!res && GetLastError() != ERROR_IO_PENDING) {
                ret->state = io_task_free                ;
                list_push_back(&par->io_sched->none, ret);
                return 0;
            }

            return ret;
}

struct io_task*
    file_write
        (file* par, u8_t* par_buf, u64_t par_len)          {
            if (!par)                              return 0;
            if (!par_buf)                          return 0;
            if (par->file == INVALID_HANDLE_VALUE) return 0;
            if (trait_of(par) != file_t)           return 0; io_task *ret = io_sched_dispatch(par->io_sched);
            if (!ret)                              return 0;

            ret->state              = io_task_exec;
            ret->io_task.Offset     = 0xFFFFFFFF  ;
            ret->io_task.OffsetHigh = 0xFFFFFFFF  ;
            bool_t res = WriteFile (
                par->file    ,
                par_buf      ,
                par_len      ,
                0            ,
                &ret->io_task
            );

            if (!res && GetLastError() != ERROR_IO_PENDING) {
                ret->state = io_task_free                ;
                list_push_back(&par->io_sched->none, ret);
                return 0;
            }

            return ret;
}