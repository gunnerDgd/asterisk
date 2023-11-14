#include "file.h"
#include "io_res.h"

#include <stdio.h>

obj_trait __file_trait                  = {
    .init          = &__file_init         ,
    .init_as_clone = &__file_init_as_clone,
    .init_as_ref   =                     0,
    .deinit        = &__file_deinit       ,
    .name          = &__file_name         ,
    .size          = &__file_obj_size
};

bool_t
    __file_init
        (__file* par_file, u32_t par_count, va_list par) {
            if(!make_at(par_file->path, str_t) from (0))
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
        (__file* par)                      {
            CloseHandle(par->file)         ;
            CloseHandle(par->file_io_sched);
            del       (&par->path)         ;
}

str*
    __file_name
        (__file* par)        {
            return &par->path;
}

u64_t  
    __file_obj_size()        {
        return sizeof(__file);
}

bool_t 
    __file_open
        (__file* par)                            {
            if(par->file != INVALID_HANDLE_VALUE)
                return false_t;

            par->file = CreateFile          (
                ptr_raw(str_ptr(&par->path)),
                GENERIC_READ | GENERIC_WRITE,
                0                           ,
                0                           ,
                OPEN_EXISTING               ,
                FILE_FLAG_OVERLAPPED        ,
                0
            );

            if (par->file == INVALID_HANDLE_VALUE)
                return false_t;

            par->file_io_sched = CreateIoCompletionPort (
                par->file         ,
                par->io_sched->hnd,
                par->io_sched     ,
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

            if (par->file == INVALID_HANDLE_VALUE)
                return false_t;

            par->file_io_sched = CreateIoCompletionPort (
                par->file         ,
                par->io_sched->hnd,
                par->io_sched     ,
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
        (__file* par) {
            if(par->file != INVALID_HANDLE_VALUE) {
                CloseHandle(par->file)          ;
                CloseHandle(par->file_io_sched) ;
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

            bool_t res      = ReadFile (
                par->file       ,
                ptr_raw(par_buf),
                par_len         ,
                0               ,
                &ret->hnd       
            );

            if(!res && GetLastError() != ERROR_IO_PENDING) {
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

            bool_t res = WriteFile (
                par->file       ,
                ptr_raw(par_buf),
                par_len         ,
                0               ,
                &ret->hnd       
            );

            if(!res && GetLastError() !=  ERROR_IO_PENDING) {
                del(ret);
                return 0;
            }

            return ret->task;
}