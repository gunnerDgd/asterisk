#include "file.h"

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
        (file* par_file, u32_t par_count, va_list par)                            {
            io_sched *sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
            if (!sched)                             sched = this_io_sched();
            if (!sched)                             return false_t;
            if (trait_of(sched) != io_sched_t)      return false_t;
            if (sched->hnd == INVALID_HANDLE_VALUE) return false_t;
            
            par_file->sched   = ref(sched)          ;
            par_file->file_io = INVALID_HANDLE_VALUE;
            par_file->file    = INVALID_HANDLE_VALUE;
            return true_t;
}

bool_t 
    file_clone
        (file* par, file* par_clone) {
            return false_t;
}

void   
    file_del
        (file* par)                  {
            CloseHandle(par->file)   ;
            CloseHandle(par->file_io);
            del        (par->sched)  ;
}

bool_t
    file_open
        (file* par, str* par_name)                          {
            if (!par_name)                    return false_t;
            if (!par)                         return false_t;
            if (trait_of(par_name) != str_t)  return false_t;
            if (trait_of(par)      != file_t) return false_t;
            return file_open_cstr(par, str_ptr(par_name));
}

bool_t
    file_open_cstr
        (file* par, const char* par_name)                         {
            if (!par_name)                          return false_t;
            if (!par)                               return false_t;
            if (trait_of(par)        != file_t)     return false_t;
            if (trait_of(par->sched) != io_sched_t) return false_t;
            if (par->file != INVALID_HANDLE_VALUE)  return false_t;
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
            par->file_io = CreateIoCompletionPort (
                par->file      ,
                par->sched->hnd,
                par->sched     ,
                0
            );

            if(!par->file_io)                   {
                CloseHandle(par->file)          ;
                par->file = INVALID_HANDLE_VALUE;
                return false_t                  ;
            }

            return true_t;
}

bool_t
    file_create
        (file* par, str* par_name)                          {
            if (!par_name)                    return false_t;
            if (!par)                         return false_t;
            if (trait_of(par_name) != str_t)  return false_t;
            if (trait_of(par)      != file_t) return false_t;
            return file_create_cstr(par, str_ptr(par_name));
}

bool_t 
    file_create_cstr
        (file* par, const char* par_name)                         {
            if (!par_name)                          return false_t;
            if (!par)                               return false_t;
            if (trait_of(par)        != file_t)     return false_t;
            if (trait_of(par->sched) != io_sched_t) return false_t;
            if (par->file != INVALID_HANDLE_VALUE)  return false_t;

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
            par->file_io = CreateIoCompletionPort (
                par->file      ,
                par->sched->hnd,
                par->sched     ,
                0
            );

            if(!par->file_io)                   {
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
                CloseHandle(par->file)   ;
                CloseHandle(par->file_io);

                par->file = INVALID_HANDLE_VALUE;
            }
}

fut*
    file_read
        (file* par, u8_t* par_buf, u64_t par_len)           {
            if (!par_buf)                           return 0;
            if (!par)                               return 0;
            if (trait_of(par)        != file_t)     return 0;
            if (trait_of(par->sched) != io_sched_t) return 0;
            if (par->file == INVALID_HANDLE_VALUE)  return 0;

            io_res *ret = make (io_res_t) from (1, par->sched);
            if (!ret)                      return 0;
            if (trait_of(ret) != io_res_t) return 0;
            ret->res.Offset     = -1;
            ret->res.OffsetHigh = -1;
            bool_t res = ReadFile   (
                par->file,
                par_buf  ,
                par_len  , 
                0        ,
                &ret->res
            );

            fut* fut = io_res_fut(ret);
			if (ret && GetLastError() != ERROR_IO_PENDING) {
                ret->stat = fut_err;
                del   (ret);
				return fut ;
			}

            del   (ret);
			return fut ;
}

fut*
    file_write
        (file* par, u8_t* par_buf, u64_t par_len)           {
            if (!par_buf)                           return 0;
            if (!par)                               return 0;
            if (trait_of(par)        != file_t)     return 0;
            if (trait_of(par->sched) != io_sched_t) return 0;
            if (par->file == INVALID_HANDLE_VALUE)  return 0;

            io_res *ret = make (io_res_t) from (1, par->sched);
            if (!ret)                      return 0;
            if (trait_of(ret) != io_res_t) return 0;
            ret->res.Offset     = -1;
            ret->res.OffsetHigh = -1;
            bool_t res = WriteFile  (
                par->file,
                par_buf  ,
                par_len  , 
                0        ,
                &ret->res
            );

            fut* fut = io_res_fut(ret);
			if (ret && GetLastError() != ERROR_IO_PENDING) {
                ret->stat = fut_err;
                del   (ret);
				return fut ;
			}

            del   (ret);
			return fut ;
}