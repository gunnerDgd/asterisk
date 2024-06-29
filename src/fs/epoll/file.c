#include "file.h"
#include "../../io.h"

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static bool_t
    do_new
        (file* self, u32_t count, va_list arg)                             {
            io_run *run = null_t; if (count > 0) run = va_arg(arg, io_run*);
            if (trait_of(run) != io_run_t) run = this_io_run();
            if (trait_of(run) != io_run_t) return false_t;

            if (!make_at(&self->out, out) from (1, run)) goto err;
            if (!make_at(&self->in , in)  from (1, run)) goto err;
            self->file = -1;
            return true_t;
    err:    del (&self->out);
            del (&self->in) ;
            return false_t;
}

static bool_t
    do_clone
        (file* self, file* clone) {
            return false_t;
}

static void
    do_del
        (file* self)        {
            del (&self->out);
            del (&self->in);
}

static obj_trait
    do_file = make_trait (
        do_new      ,
        do_clone    ,
        null_t      ,
        do_del      ,
        sizeof(file),
        null_t
);

obj_trait* file_t = &do_file;

bool_t
    file_open
        (file* self, const char* name)                  {
            if (trait_of(self) != file_t) return false_t;
            if (self->file != -1)         return false_t;
            if (!name)                    return false_t;
            self->file = open (name, O_RDWR | O_NONBLOCK);

            if (self->file == -1)            return false_t;
            if (!out_open(&self->out, name)) return false_t;
            if (!in_open (&self->in , name)) return false_t;
            return true_t;
}

bool_t 
    file_new
        (file* self, const char* name)                  {
            if (trait_of(self) != file_t) return false_t;
            if (self->file != -1)         return false_t;
            if (!name)                    return false_t;

            self->file = open(name, O_RDWR | O_NONBLOCK | O_CREAT, 0755);
            if (self->file == -1)            return false_t;
            if (!out_open(&self->out, name)) return false_t;
            if (!in_open (&self->in , name)) return false_t;
            return true_t;
}

void
    file_close
        (file* self)         {
            close(self->file);

            self->out.out = -1;
            self->in .in  = -1;
            self->file = -1;
}

u64_t
    file_seek
        (file* self, obj_trait* trait, u64_t pos)  {
            if (trait_of(self) != file_t) return -1;

            if (trait == out_t) return out_seek (&self->out, pos);
            if (trait == in_t)  return in_seek  (&self->in , pos);
            return -1;
}

u64_t
    file_pos
        (file* self, obj_trait* trait)             {
            if (trait_of(self) != file_t) return -1;

            if (trait == out_t) return out_pos(&self->out);
            if (trait == in_t)  return in_pos (&self->in) ;
            return -1;
}

bool_t
    file_trunc
        (file* self, u64_t len)                           {
            if (trait_of (self) != file_t)  return false_t;
            if (ftruncate(self->file, len)) return false_t;
            return true_t;
}

fut*
    file_read
        (file* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != file_t) return null_t;
            return in_read (
                &self->in,
                buf      ,
                len
            );
}

fut*
    file_write
        (file* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != file_t) return null_t;
            return out_write (
                &self->out,
                buf       ,
                len
            );
}

u64_t
    file_size
        (file* self)                                    {
            if (trait_of(self) != file_t) return false_t;
            struct stat ret;

            if (fstat(self->file, &ret)) return 0;
            return ret.st_size;
}