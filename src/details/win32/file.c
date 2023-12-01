#include "file.h"

#include "curr.h"
#include "thd.h"

#include "io_sched.h"
#include "io_task.h"

#include "sched.h"
#include "task.h"

obj_trait __file_trait      = {
    .on_new   = &__file_new   ,
    .on_clone = &__file_clone ,
    .on_ref   =             0 ,
    .on_del   = &__file_del   ,
    .size     = sizeof(__file)
};

bool_t
    __file_new
        (__file* par_file, u32_t par_count, va_list par) {
            par_file->io_sched = (par_count == 0) ? ref(io_sched_curr()) : ref(va_arg(par, __io_sched*));
            if (!par_file->io_sched)
                return false_t;

            par_file->file = INVALID_HANDLE_VALUE;
            return true_t;
}

bool_t 
    __file_clone
        (__file* par, __file* par_clone) {
            return false_t;
}

void   
    __file_del
        (__file* par)                      {
            CloseHandle(par->file)         ;
            CloseHandle(par->file_io_sched);
            del        (par->io_sched)     ;
}

bool_t
    __file_open
        (__file* par, const char* par_name)                     {
            if(par->file != INVALID_HANDLE_VALUE) return false_t;
            par->file = CreateFile                             (
                par_name                                       ,
                GENERIC_READ | GENERIC_WRITE | FILE_APPEND_DATA,
                0                                              ,
                0                                              ,
                OPEN_EXISTING                                  ,
                FILE_FLAG_OVERLAPPED                           ,
                0
            );

            if (par->file == INVALID_HANDLE_VALUE) return false_t;
            par->file_io_sched = CreateIoCompletionPort (
                par->file              ,
                par->io_sched->io_sched,
                par->io_sched          ,
                0
            );

            if(!par->file_io_sched)   {
                CloseHandle(par->file);
                return false_t        ;
            }

            return true_t;
}

bool_t 
    __file_create
        (__file* par, const char* par_name)                     {
            if(par->file != INVALID_HANDLE_VALUE) return false_t;
            par->file = CreateFile                             (
                par_name                                       ,
                GENERIC_READ | GENERIC_WRITE | FILE_APPEND_DATA,
                0                                              ,
                0                                              ,
                CREATE_NEW                                     ,
                FILE_FLAG_OVERLAPPED                           ,
                0
            );

            if (par->file == INVALID_HANDLE_VALUE) return false_t;
            par->file_io_sched = CreateIoCompletionPort (
                par->file              ,
                par->io_sched->io_sched,
                par->io_sched          ,
                0
            );

            if(!par->file_io_sched)   {
                CloseHandle(par->file);
                return false_t        ;
            }

            return true_t;
}

void
    __file_close
        (__file* par)                             {
            if(par->file != INVALID_HANDLE_VALUE) {
                CloseHandle(par->file)    ;
                CloseHandle(par->io_sched);
                par->file = INVALID_HANDLE_VALUE;
            }
}

struct __io_task*
    __file_read
        (__file* par, u8_t* par_buf, u64_t par_len)            {
            __io_task *ret = __io_sched_dispatch(par->io_sched);
            if (!ret) return 0;

            ret->state              = __io_task_state_pend;
            ret->io_task.Offset     = -1                  ;
            ret->io_task.OffsetHigh = -1                  ;
            bool_t res = ReadFile (
                par->file    ,
                par_buf      ,
                par_len      , 
                0            ,
                &ret->io_task
            );

            if (!res && GetLastError() != ERROR_IO_PENDING)     {
                ret->state = __io_task_state_none               ;
                obj_list_push_back(&par->io_sched->io_task, ret);
                return 0;
            }

            return ret;
}

struct __io_task*
    __file_write
        (__file* par, u8_t* par_buf, u64_t par_len)            {
            __io_task *ret = __io_sched_dispatch(par->io_sched); if (!ret) return 0;

            ret->state              = __io_task_state_pend;
            ret->io_task.Offset     = -1                  ;
            ret->io_task.OffsetHigh = -1                  ;
            bool_t res = WriteFile (
                par->file    ,
                par_buf      ,
                par_len      ,
                0            ,
                &ret->io_task
            );

            if (!res && GetLastError() != ERROR_IO_PENDING)     {
                ret->state = __io_task_state_none               ;
                obj_list_push_back(&par->io_sched->io_task, ret);
                return 0;
            }

            return ret;
}