#include "file.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

obj_trait file_trait = make_trait (
    file_new    ,
    file_clone  ,
    null_t      ,
    file_del    ,
    sizeof(file),
    null_t
);

obj_trait* file_t = &file_trait;

u64_t
    file_read_do_poll
        (io_res* par)                                                            {
            if (trait_of(par)  != io_res_t) return fut_err; file* file = par->dev;
            if (trait_of(file) != file_t)   return fut_err;
            u8_t *buf = par->buf + par->ret;
            u64_t len = par->len -  par->ret;
            int   fd  = file->file;
            i64_t ret = read      (
                fd ,
                buf,
                len
            );

            if (ret < 0) return fut_err;
            par->ret  += ret;
            return fut_ready;
}

u64_t
    file_read_do_ret
        (io_res* par)                                                            {
            if (trait_of(par)  != io_res_t) return fut_err; file* file = par->dev;
            if (trait_of(file) != file_t)   return fut_err;
            return par->ret;
}

u64_t
    file_write_do_poll
        (io_res* par)                                                            {
            if (trait_of(par)  != io_res_t) return fut_err; file* file = par->dev;
            if (trait_of(file) != file_t)   return fut_err;
            u8_t *buf = par->buf + par->ret;
            u64_t len = par->len - par->ret;
            int   fd  = file->file;
            i64_t ret = write     (
                fd ,
                buf,
                len
            );

            if (ret < 0) return fut_err;
            par->ret  += ret;
            return fut_ready;
}

u64_t
    file_write_do_ret
        (io_res* par)                                                            {
            if (trait_of(par)  != io_res_t) return fut_err; file* file = par->dev;
            if (trait_of(file) != file_t)   return fut_err;
            return par->ret;
}

fut_ops file_write_do = make_fut_ops(file_write_do_poll, file_write_do_ret);
fut_ops file_read_do  = make_fut_ops(file_read_do_poll , file_read_do_ret) ;

bool_t
    file_new
        (file* par_file, u32_t par_count, va_list par)                            {
            io_sched *sched = 0; if (par_count > 0) sched = va_arg(par, io_sched*);
            if (trait_of(sched) != io_sched_t) return false_t;
            par_file->sched = ref(sched);
            par_file->file  = 0         ;
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
        (file* par, str* par_name)                          {
            if (trait_of(par_name) != str_t)  return false_t;
            if (trait_of(par)      != file_t) return false_t;
            return file_open_cstr(par, str_ptr(par_name));
}

bool_t
    file_open_cstr
        (file* par, const char* par_name)                         {
            if (trait_of(par)        != file_t)     return false_t;
            if (trait_of(par->sched) != io_sched_t) return false_t;

            par->file = open(par_name, O_RDWR | O_NONBLOCK);
            if (par->file <= 0) return false_t;
            return true_t;
}

bool_t
    file_create
        (file* par, str* par_name)                          {
            if (trait_of(par_name) != str_t)  return false_t;
            if (trait_of(par)      != file_t) return false_t;
            return file_create_cstr(par, str_ptr(par_name));
}

bool_t 
    file_create_cstr
        (file* par, const char* par_name)                         {
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
        (file* par, u8_t* par_buf, u64_t par_len)     {
            if (trait_of(par) != file_t) return null_t;
            if (!par_len)				 return null_t;
            if (!par_buf)				 return null_t;

            io_res *res = make (io_res) from (3, par, par_buf, par_len);
            fut    *ret = make (fut)    from (2, &file_read_do, res)   ;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;

            fut_poll(ret);
            del     (res);
            return   ret ;
}

fut*
    file_write
        (file* par, u8_t* par_buf, u64_t par_len)     {
            if (trait_of(par) != file_t) return null_t;
            if (!par_len)				 return null_t;
            if (!par_buf)				 return null_t;

            io_res *res = make (io_res) from (3, par, par_buf, par_len);
            fut    *ret = make (fut)    from (2, &file_write_do, res)  ;
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;

            fut_poll(ret);
            del     (res);
            return   ret ;
}