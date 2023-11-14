#include "file.h"
#include "io_res.h"

bool_t 
    __file_init
        (__file* par_file, u32_t par_count, va_list par) {
            if(!make_at(par_file->path, str_t) from (1, NULL))
                return false_t;

            par_file->io_sched = ref(va_arg(par, __io_sched*));
            if (!par_file->io_sched)
                return false_t;
            
            const char* path     = va_arg(par, const char*)    ;
            u64_t       path_len = strlen(path)                ;
            str_push_back_cstr(&par_file->path, path, path_len);

            par_file->file = INVALID_HANDLE_VALUE;
            return true_t;
}

bool_t 
    __file_init_as_clone
        (__file* par, __file* par_clone) {
            return false_t;
}

void   
    __file_deinit
        (__file* par)             {
            CloseHandle(par->file);
            del       (&par->path);
}

bool_t 
    __file_open
        (__file* par)              {
            if(par->file != INVALID_HANDLE_VALUE)
                return false_t;

            par->file = CreateFile  (
                par                 ,
                GENERIC_ALL         ,
                0                   ,
                0                   ,
                OPEN_EXISTING       ,
                FILE_FLAG_OVERLAPPED,
                0
            );

            return (par->file != INVALID_HANDLE_VALUE);
}

bool_t 
    __file_create
        (__file* par) {
            if(par->file != INVALID_HANDLE_VALUE)
                return false_t;

            par->file = CreateFile  (
                par                 ,
                GENERIC_ALL         ,
                0                   ,
                0                   ,
                CREATE_NEW          ,
                FILE_FLAG_OVERLAPPED,
                0
            );

            return (par->file != INVALID_HANDLE_VALUE);
}

void 
    __file_close
        (__file* par) {
            if(par->file != INVALID_HANDLE_VALUE) {
                CloseHandle(par->file)          ;
                par->file = INVALID_HANDLE_VALUE;
            }
}

task* 
    __file_read
        (__file* par, ptr par_buf, u64_t par_len) {
            if(ptr_size(par_buf) < par_len)
                return 0;

            __io_res* ret = make(&__io_res_trait) from(1, par->io_sched);
            if (!ret)
                return false_t;

            u64_t  res_byte =           0;
            bool_t res      = ReadFile   (
                par->file       ,
                ptr_raw(par_buf),
                par_len         ,
                &res_byte       ,
                &ret->hnd
            );

            if(!res)    {
                del(ret);
                return 0;
            }

            return ret->task;
}

task* 
    __file_write
        (__file* par, ptr par_buf, u64_t par_len) {
            if(ptr_size(par_buf) < par_len)
                return 0;

            __io_res* ret = make(&__io_res_trait) from(1, par->io_sched);
            if (!ret)
                return false_t;

            u64_t  res_byte =           0;
            bool_t res      = WriteFile  (
                par->file       ,
                ptr_raw(par_buf),
                par_len         ,
                &res_byte       ,
                &ret->hnd
            );

            if(!res)    {
                del(ret);
                return 0;
            }

            return ret->task;
}