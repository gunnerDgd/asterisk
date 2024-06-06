#include "out.h"

#include "../../io.h"

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static u64_t
    out_do_poll
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; out* out = self->dev;
            if (trait_of(out)  != out_t)    return fut_err;

            u8_t *buf = self->buf + self->ret;
            u64_t len = self->len - self->ret;
            int   fd  = out->out;
            i64_t ret = write   (
                fd ,
                buf,
                len
            );

            if (ret == -1) return fut_err;
            self->ret += ret;

            if (self->ret < self->len) return fut_pend;
            return fut_ready;
}

static u64_t
    out_do_ret
        (io_res* self)                                                          {
            if (trait_of(self) != io_res_t) return fut_err; out* out = self->dev;
            if (trait_of(out)  != out_t)    return fut_err;
            return self->ret;
}

fut_ops out_do = make_fut_ops (
    out_do_poll,
    out_do_ret
);

obj_trait out_trait = make_trait (
    out_new     ,
    out_clone   ,
    null_t     ,
    out_del     ,
    sizeof (out),
    null_t
);

obj_trait *out_t = &out_trait;

bool_t
    out_new
        (out* self, u32_t count, va_list arg)                                   {
            io_sched *sched = null_t; if (count > 0) sched = va_arg(arg, any_t);
            if (trait_of(sched) != io_sched_t) sched = this_io_sched();
            if (trait_of(sched) != io_sched_t) return false_t;

            self->sched = ref (sched);
            self->out    = 0;
            return true_t;
}

bool_t
    out_clone
        (out* self, out* clone) {
            return false_t;
}

void
    out_del
        (out* self)             {
            close (self->out)   ;
            del   (self->sched);
}

bool_t
    out_create_cstr
        (out* self, const char* name)                  {
            if (trait_of(self) != out_t) return false_t;
            if (self->out != -1)         return false_t;
            if (!name)                   return false_t;

            self->out = open (name, O_WRONLY | O_CREAT, 0755);
            if (self->out <= 0) return false_t;
            return true_t;
}

bool_t
    out_open_cstr
        (out* self, const char* name)                  {
            if (trait_of(self) != out_t) return false_t;
            if (self->out != -1)         return false_t;
            if (!name)                   return false_t;
            self->out = open (name, O_RDONLY);

            if (self->out <= 0) return false_t;
            return true_t;
}

bool_t
    out_create
        (out* self, str* name)                          {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != out_t)  return false_t;
            return out_create_cstr(self, str_ptr(name));
}

bool_t
    out_open
        (out* self, str* name)                          {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != out_t)  return false_t;
            return out_open_cstr(self, str_ptr(name));
}

void
    out_close
        (out* self)                            {
            if (trait_of(self) != out_t) return;
            close (self->out);
            self->out = -1;
}

fut*
    out_write
        (out* self, any_t buf, u64_t len)             {
            if (trait_of(self) != out_t) return null_t;
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
                2      ,
                &out_do,
                res
            );

            if (trait_of(ret) != fut_t) goto err;
            return ret;
    err:    del (res);
            del (ret);
            return false_t;
}

u64_t
    out_seek
        (out* self, u64_t pos)                    {
            if (trait_of(self) != out_t) return -1;
            return lseek     (
                self->out,
                pos      ,
                SEEK_SET
            );
}

u64_t
    out_pos
        (out* self)                               {
            if (trait_of(self) != out_t) return -1;
            return lseek     (
                self->out,
                0        ,
                SEEK_CUR
            );
}