#include "in.h"
#include "../../io.h"

#include <unistd.h>
#include <fcntl.h>

static u64_t
    in_do_poll
        (io_res* self)                                                        {
            if (trait_of(self) != io_res_t) return fut_err; in* in = self->dev;
            if (trait_of(in)   != in_t)     return fut_err;

            u8_t *buf = self->buf + self->ret;
            u64_t len = self->len - self->ret;
            int   fd  = in->in;
            i64_t ret = read  (
                fd ,
                buf,
                len
            );

            if (ret == -1) return fut_err  ;
            if (ret ==  0) return fut_ready;
            self->ret += ret;

            if (self->ret < self->len) return fut_pend;
            return fut_ready;
}

static u64_t
    in_do_ret
        (io_res* self)                                                        {
            if (trait_of(self) != io_res_t) return fut_err; in* in = self->dev;
            if (trait_of(in)   != in_t)     return fut_err;
            return self->ret;
}

fut_ops in_do = make_fut_ops (
    in_do_poll,
    in_do_ret
);

obj_trait in_trait = make_trait (
    in_new     ,
    in_clone   ,
    null_t     ,
    in_del     ,
    sizeof (in),
    null_t
);

obj_trait *in_t = &in_trait;

bool_t
    in_new
        (in* self, u32_t count, va_list arg)                                   {
            io_sched *sched = null_t; if (count > 0) sched = va_arg(arg, any_t);
            if (trait_of(sched) != io_sched_t) sched = this_io_sched();
            if (trait_of(sched) != io_sched_t) return false_t;

            self->sched = ref (sched);
            self->in    = -1;
            return true_t;
}

bool_t
    in_clone
        (in* self, in* clone) {
            return false_t;
}

void
    in_del
        (in* self)             {
            close (self->in)   ;
            del   (self->sched);
}

bool_t
    in_create_cstr
        (in* self, const char* name)                  {
            if (trait_of(self) != in_t) return false_t;
            if (self->in != -1)         return false_t;
            if (!name)                  return false_t;
            self->in = open (name, O_RDONLY | O_NONBLOCK | O_CREAT, 0755);

            if (self->in <= 0) return false_t;
            return true_t;
}

bool_t
    in_open_cstr
        (in* self, const char* name)                  {
            if (trait_of(self) != in_t) return false_t;
            if (self->in != -1)         return false_t;
            if (!name)                  return false_t;
            self->in = open (name, O_RDONLY | O_NONBLOCK);

            if (self->in <= 0) return false_t;
            return true_t;
}

bool_t
    in_create
        (in* self, str* name)                          {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != in_t)  return false_t;
            return in_create_cstr(self, str_ptr(name));
}

bool_t
    in_open
        (in* self, str* name)                          {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != in_t)  return false_t;
            return in_open_cstr(self, str_ptr(name));
}

void
    in_close
        (in* self)                            {
            if (trait_of(self) != in_t) return;
            close (self->in);
            self->in = -1;
}

fut*
    in_read
        (in* self, any_t buf, u64_t len)             {
            if (trait_of(self) != in_t) return null_t;
            if (!buf)                   return null_t;
            if (!len)                   return null_t;
            io_res *res = null_t;
            fut    *ret = null_t;

            res = make (io_res) from (
                3   ,
                self,
                buf ,
                len
            );

            if (trait_of(res) != io_res_t) goto err;
            ret = make (fut) from                  (
                2     ,
                &in_do,
                res
            );

            if (trait_of(ret) != fut_t) goto err;
            return ret;
    err:    del (res);
            del (ret);
            return false_t;
}


u64_t
    in_seek
        (in* self, u64_t pos)                    {
            if (trait_of(self) != in_t) return -1;
            return lseek     (
                self->in,
                pos     ,
                SEEK_SET
            );
}

u64_t
    in_pos
        (in* self)                               {
            if (trait_of(self) != in_t) return -1;
            if (trait_of(self) != in_t) return -1;
            return lseek     (
                self->in,
                0       ,
                SEEK_CUR
            );
}