#include "file.h"
#include "../../io.h"

#include <sys/stat.h>
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

bool_t
    file_new
        (file* self, u32_t count, va_list arg)                                     {
            io_sched *sched = null_t; if (count > 0) sched = va_arg(arg, io_sched*);
            if (trait_of(sched) != io_sched_t) sched = this_io_sched();
            if (trait_of(sched) != io_sched_t) return false_t;

            if (!make_at(&self->out, out) from (1, sched)) goto err;
            if (!make_at(&self->in , in)  from (1, sched)) goto err;
            return true_t;
    err:    del (&self->out);
            del (&self->in) ;
            return false_t;
}

bool_t 
    file_clone
        (file* self, file* clone) {
            return false_t;
}

void   
    file_del
        (file* self)        {
            del (&self->out);
            del (&self->in);
}

bool_t
    file_open
        (file* self, str* name)                         {
            if (trait_of(self) != file_t) return false_t;
            if (trait_of(name) != str_t)  return false_t;
            return file_open_cstr(self, str_ptr(name));
}

bool_t
    file_open_cstr
        (file* self, const char* name)                  {
            if (trait_of(self) != file_t) return false_t;
            if (self->file != -1)         return false_t;
            if (!name)                    return false_t;
            self->file = open (name, O_RDWR);

            if (self->file == -1) return false_t;
            self->out.out = self->file;
            self->in .in  = self->file;
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
            if (self->file != -1)         return false_t;
            if (!name)                    return false_t;

            self->file = open(name, O_RDWR | O_NONBLOCK | O_CREAT, 0755);
            if (self->file <= 0) return false_t;
            self->out.out = self->file;
            self->in .in  = self->file;
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