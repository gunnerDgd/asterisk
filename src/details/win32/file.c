#include "file.h"
#include "thd.h"

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
            par_file->io_sched = ref(va_arg(par, __io_sched*));
            if (!make_at(&par_file->path, str_t) from (0)) return false_t;        
            if (!par_file->io_sched) {
                del(&par_file->path);
                return false_t      ;
            }
            
            const char* path     = va_arg(par, const char*)    ;
            u64_t       path_len = strlen(path)                ;
            str_push_back_cstr(&par_file->path, path, path_len);

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
        (__file* par)                 {
            CloseHandle(par->file)    ;
            CloseHandle(par->io_sched);
            del       (&par->path)    ;
}

bool_t 
    __file_open
        (__file* par)                            {
            if(par->file != INVALID_HANDLE_VALUE)
                return false_t;

            par->file = CreateFile          (
                str_ptr(&par->path)         ,
                GENERIC_READ | GENERIC_WRITE,
                0                           ,
                0                           ,
                OPEN_EXISTING               ,
                FILE_FLAG_OVERLAPPED        ,
                0
            );

            if (par->file == INVALID_HANDLE_VALUE) return false_t;
            par->io_sched  = CreateIoCompletionPort (
                par->file                      ,
                __thd_curr()->io_sched.io_sched,
               &__thd_curr()->io_sched         ,
                0
            );

            if(!par->io_sched)        {
                CloseHandle(par->file);
                return false_t        ;
            }

            return true_t;
}

bool_t 
    __file_create
        (__file* par)                            {
            if(par->file != INVALID_HANDLE_VALUE)
                return false_t;

            par->file = CreateFile          (
                ptr_raw(str_ptr(&par->path)),
                GENERIC_READ | GENERIC_WRITE,
                0                           ,
                0                           ,
                CREATE_NEW                  ,
                FILE_FLAG_OVERLAPPED        ,
                0
            );

            if (par->file == INVALID_HANDLE_VALUE) return false_t;
            par->io_sched = CreateIoCompletionPort (
                par->file                      ,
                __thd_curr()->io_sched.io_sched,
               &__thd_curr()->io_sched         ,
                0
            );

            if(!par->io_sched)        {
                CloseHandle(par->file);
                return false_t        ;
            }

            return true_t;
}

void 
    __file_close
        (__file* par) {
            if(par->file != INVALID_HANDLE_VALUE) {
                CloseHandle(par->file)    ;
                CloseHandle(par->io_sched);
                par->file = INVALID_HANDLE_VALUE;
            }
}

struct __task* 
    __file_read
        (__file* par, u8_t* par_buf, u64_t par_len)        {
            __io_task *ret = mem_new (0, sizeof(__io_task)); 
            if (!ret) return 0;

            bool_t res = ReadFile(par->file, par_buf, par_len, 0, ret);
            if (!res && GetLastError() != ERROR_IO_PENDING)
                goto read_failed;

            ret->task = __sched_exec(&__thd_curr()->sched, __io_task_run, ret);
            if (!ret->task) goto read_failed;

            return ret->task;
    read_failed:
            mem_del(0, ret);
            return  0;
}

struct __task*
    __file_write
        (__file* par, u8_t* par_buf, u64_t par_len) {
            __io_task *ret = mem_new (0, sizeof(__io_task)); 
            if (!ret) return 0;

            bool_t res = WriteFile (par->file, ptr_raw(par_buf), par_len, 0, ret);
            if (!res && GetLastError() != ERROR_IO_PENDING)
                goto write_failed;

            ret->task = __sched_exec(&__thd_curr()->sched, __io_task_run, ret);
            if (!ret->task) goto write_failed;

            return ret->task;
    write_failed:
            mem_del(0, ret);
            return  0;
}