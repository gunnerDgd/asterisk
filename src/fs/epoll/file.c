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
        (io_res* self)                                                            {
            if (trait_of(self) != io_res_t) return fut_err; file* file = self->dev;
            if (trait_of(file) != file_t)   return fut_err;

            if (file->thd != this_thd()) return fut_pend;
            u8_t *buf = self->buf + self->ret;
            u64_t len = self->len - self->ret;
            int   fd  = file->file;
            self->ret = pread     (
                fd     ,
                buf    ,
                len    ,
                file->in
            );

            if (self->ret < 0) return fut_err;
            file->in += self->ret;
            return fut_ready;
}

u64_t
    file_read_do_ret
        (io_res* self)                                                            {
            if (trait_of(self) != io_res_t) return fut_err; file* file = self->dev;
            if (trait_of(file) != file_t)   return fut_err;
            return self->ret;
}

u64_t
    file_write_do_poll
        (io_res* self)                                                            {
            if (trait_of(self) != io_res_t) return fut_err; file* file = self->dev;
            if (trait_of(file) != file_t)   return fut_err;

            if (file->thd != this_thd()) return fut_pend;
            u8_t *buf = self->buf + self->ret;
            u64_t len = self->len - self->ret;
            int   fd  = file->file;
            self->ret = pwrite    (
                fd      ,
                buf     ,
                len     ,
                file->out
            );

            if (self->ret < 0) return fut_err;
            file->out += self->ret;
            return fut_ready;
}

u64_t
    file_write_do_ret
        (io_res* self)                                                            {
            if (trait_of(self) != io_res_t) return fut_err; file* file = self->dev;
            if (trait_of(file) != file_t)   return fut_err;
            return self->ret;
}

fut_ops file_write_do = make_fut_ops(file_write_do_poll, file_write_do_ret);
fut_ops file_read_do  = make_fut_ops(file_read_do_poll , file_read_do_ret) ;

bool_t
    file_new
        (file* self, u32_t count, va_list arg)                                     {
            io_sched *sched = null_t; if (count > 0) sched = va_arg(arg, io_sched*);
            if (trait_of(sched) != io_sched_t) sched = this_io_sched();
            if (trait_of(sched) != io_sched_t) return false_t;

            self->thd   = this_thd();
            self->sched = ref(sched);
            self->file  = 0;
            self->out   = 0;
            self->in    = 0;

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
        (file* self, str* name)                         {
            if (trait_of(name) != str_t)  return false_t;
            if (trait_of(self) != file_t) return false_t;
            return file_open_cstr(self, str_ptr(name));
}

bool_t
    file_open_cstr
        (file* self, const char* name)                  {
            if (trait_of(self) != file_t) return false_t;
            if (!name)                    return false_t;

            self->file = open(name, O_RDWR | O_NONBLOCK);
            if (self->file <= 0) return false_t;
            return true_t;
}

bool_t
    file_create
        (file* self, str* name)                         {
            if (trait_of(name) != str_t)  return false_t;
            if (trait_of(self) != file_t) return false_t;
            return file_create_cstr(self, str_ptr(name));
}

bool_t 
    file_create_cstr
        (file* self, const char* name)                  {
            if (trait_of(self) != file_t) return false_t;
            if (!name)                    return false_t;

            self->file = open(name, O_RDWR | O_NONBLOCK | O_CREAT, 0755);
            if (self->file <= 0) return false_t;
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
        (file* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != file_t) return null_t;
            if (self->thd != this_thd())  return null_t;
            if (!len)				      return null_t;
            if (!buf)				      return null_t;

            io_res *res = make (io_res) from (3, self, buf, len);
            fut    *ret = make (fut)    from (2, &file_read_do, res);
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;

            fut_poll(ret);
            del     (res);
            return   ret ;
}

fut*
    file_write
        (file* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != file_t) return null_t;
            if (self->thd != this_thd())  return null_t;
            if (!len)				      return null_t;
            if (!buf)				      return null_t;

            io_res *res = make (io_res) from (3, self, buf, len);
            fut    *ret = make (fut)    from (2, &file_write_do, res);
            if (trait_of(res) != io_res_t) return null_t;
            if (trait_of(ret) != fut_t)    return null_t;

            fut_poll(ret);
            del     (res);
            return   ret ;
}