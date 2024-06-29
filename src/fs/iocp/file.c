#include "file.h"

static bool_t
    do_new
        (file* self, u32_t count, va_list arg)                            {
            io_run *run = null_t; if (count > 0)  run = va_arg(arg, any_t);
            if (trait_of(run) != io_run_t)        run = this_io_run();
            if (trait_of(run) != io_run_t)        return false_t;
            if (run->hnd == INVALID_HANDLE_VALUE) return false_t;
            
            if (!make_at (&self->out, out) from (1, run)) goto err;
            if (!make_at (&self->in , in)  from (1, run)) goto err;
            self->run = ref  (run);
            self->dev = (any_t) -1;
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
            file_close(self);
            del(self->run);
}

static obj_trait 
    do_obj = make_trait (
        do_new      ,
        do_clone    ,
        null_t      ,
        do_del      ,
        sizeof(file),
        null_t
);

obj_trait* file_t = &do_obj;

bool_t 
    file_open
        (file* self, const char* name)                  {
            if (trait_of(self) != file_t) return false_t;
            if (!name)                    return false_t;

            if (self->dev != INVALID_HANDLE_VALUE) return false_t;
            self->dev = CreateFile                               (
                name                              ,
                GENERIC_READ | GENERIC_WRITE      ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                0                                 ,
                OPEN_EXISTING                     ,
                FILE_FLAG_OVERLAPPED              ,
                0
            );

            if (self->dev == INVALID_HANDLE_VALUE) goto err;
            self->ioc = CreateIoCompletionPort             (
                self->dev     ,
                self->run->hnd,
                self->run     ,
                0
            );

            if (!out_open(&self->out, name))  goto err;
            if (!in_open (&self->in , name))  goto err;
            self->out.ioc = INVALID_HANDLE_VALUE;
            self->in .ioc = INVALID_HANDLE_VALUE;
            return true_t;
    err:    out_close (&self->out);
            in_close  (&self->in) ;
            return false_t;
}

bool_t 
    file_new
        (file* self, const char* name)                  {
            if (trait_of(self) != file_t) return false_t;
            if (!name)                    return false_t;

            if (self->dev != INVALID_HANDLE_VALUE) return false_t;
            self->dev = CreateFile                               (
                name                              ,
                GENERIC_READ | GENERIC_WRITE      ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                0                                 ,
                CREATE_NEW                        ,
                FILE_FLAG_OVERLAPPED              ,
                0
            );

            if (self->dev == INVALID_HANDLE_VALUE) goto err;
            if (!out_open (&self->out, name))      goto err;
            if (!in_open  (&self->in , name))      goto err;
            return true_t;
    err:    out_close (&self->out);
            in_close  (&self->in) ;
            return false_t;
}

void
    file_close
        (file* self)               {
            CloseHandle (self->dev);
            CloseHandle (self->ioc);

            self->out.ioc = INVALID_HANDLE_VALUE;
            self->in .ioc = INVALID_HANDLE_VALUE;
            self->dev = INVALID_HANDLE_VALUE;
            self->ioc = INVALID_HANDLE_VALUE;

            out_close(&self->out);
            in_close (&self->in) ;
}

fut*
    file_read
        (file* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != file_t) return null_t;
            if (!buf)                     return null_t;
            if (!len)                     return null_t;
            return in_read                             (
                &self->in,
                buf      ,
                len
            );
}

fut*
    file_write
        (file* self, u8_t* buf, u64_t len)             {
            if (trait_of(self) != file_t) return null_t;
            if (!buf)                     return null_t;
            if (!len)                     return null_t;
            return out_write                           (
                &self->out,
                buf       ,
                len
            );
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
        (file* self, u64_t len)                         {
            if (trait_of(self) != file_t) return false_t;
            u64_t lo = len & mask (32);
            u64_t hi = shr   (len, 32);
            
            if (!SetFilePointer(self->dev, lo, hi, FILE_BEGIN)) return false_t;
            if (!SetEndOfFile  (self->dev))                     return false_t;
            return true_t;
}

u64_t  
    file_size
        (file* self)                               {
            if (trait_of(self) != file_t) return -1;
            LARGE_INTEGER ret;

            if (!GetFileSizeEx(self->dev, &ret)) return -1;
            return ret.QuadPart;
}