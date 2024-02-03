#include "file.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

obj_trait file_trait     = {
    .on_new   = &file_new  ,
    .on_clone = &file_clone,
    .on_ref   =           0,
    .on_del   = &file_del  ,
    .size     = sizeof(file)
};

obj_trait* file_t = &file_trait;

i64_t
    file_do_read
        (file* par, u8_t* par_buf, u64_t par_len, void* par_arg) {
            if (!par_buf)                return -1;
            if (!par)                    return -1;
            if (trait_of(par) != file_t) return -1;
            return read                           (
                par->file,
                par_buf  ,
                par_len
            );
}

i64_t
    file_do_write
        (file* par, u8_t* par_buf, u64_t par_len, void* par_arg) {
            if (!par_buf)                return -1;
            if (!par)                    return -1;
            if (trait_of(par) != file_t) return -1;
            return write                          (
                par->file,
                par_buf  ,
                par_len
            );
}

bool_t
    file_new
        (file* par_file, u32_t par_count, va_list par)                            {
            io_sched *sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
            if (!sched)                             sched = this_io_sched()       ;
            if (!sched)                        return false_t;
            if (trait_of(sched) != io_sched_t) return false_t;
            par_file->sched = ref(sched);
            return true_t;
}

bool_t 
    file_clone
        (file* par, file* par_clone) {
            return false_t;
}

void   
    file_del
        (file* par)               {
            file_close(par)       ;
            del       (par->sched);
}

bool_t
    file_open
        (file* par, str* par_name)                        {
            if (!par_name)                    return false_t;
            if (!par)                         return false_t;
            if (trait_of(par_name) != str_t)  return false_t;
            if (trait_of(par)      != file_t) return false_t;
            return file_open_cstr(par, str_ptr(par_name));
}

bool_t
    file_open_cstr
        (file* par, const char* par_name)                       {
            if (!par_name)                          return false_t;
            if (!par)                               return false_t;
            if (trait_of(par)        != file_t)     return false_t;
            if (trait_of(par->sched) != io_sched_t) return false_t;

            par->file = open(par_name, O_RDWR | O_NONBLOCK);
            if (par->file <= 0) return false_t;
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

            par->file = open(par_name, O_RDWR | O_NONBLOCK | O_CREAT);
            if (par->file <= 0) return false_t;
            return true_t;
}

void
    file_close
        (file* par)          {
            close(par->file) ;
            del  (par->sched);
}

fut*
    file_read
        (file* par, u8_t* par_buf, u64_t par_len) {
            if (!par_len)				 return 0;
            if (!par_buf)				 return 0;
            if (!par)					 return 0;
            if (trait_of(par) != file_t) return 0;
            io_res *ret = make (io_res_t) from (
                4           ,
                par         ,
                file_do_read,
                par_buf     ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}

fut*
    file_write
        (file* par, u8_t* par_buf, u64_t par_len) {
            if (!par_len)				 return 0;
            if (!par_buf)				 return 0;
            if (!par)					 return 0;
            if (trait_of(par) != file_t) return 0;
            io_res *ret = make (io_res_t) from (
                4            ,
                par          ,
                file_do_write,
                par_buf      ,
                par_len
            );

            if (trait_of(ret) != io_res_t) return 0; fut* fut = io_res_fut(ret);
            if (trait_of(fut) != fut_t)    return 0; del (ret);
            return fut;
}