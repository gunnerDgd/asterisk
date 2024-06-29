#include "out.h"

static bool_t
    do_new
        (out* self, u32_t count, va_list arg)                             {
            io_run *run = null_t; if (count > 0)  run = va_arg(arg, any_t);
            if (trait_of(run) != io_run_t)        run = this_io_run();
            if (trait_of(run) != io_run_t)        return false_t;
            if (run->hnd == INVALID_HANDLE_VALUE) return false_t;
            
            self->ioc = INVALID_HANDLE_VALUE;
            self->dev = INVALID_HANDLE_VALUE;
            self->run = ref(run);
            self->pos = 0;
            return true_t;
}

static bool_t 
    do_clone
        (out* self, out* clone) {
            return false_t;
}

static void   
    do_del
        (out* self)               {
            CloseHandle(self->ioc);
            CloseHandle(self->dev);
            del(self->run);
            
}

static obj_trait 
    do_obj = make_trait (
        do_new     ,
        do_clone   ,
        null_t     ,
        do_del     ,
        sizeof(out),
        null_t
);

obj_trait* out_t = &do_obj;


bool_t
    out_open
        (out* self, const char* name)                            {
            if (trait_of(self) != out_t)           return false_t;
            if (self->dev != INVALID_HANDLE_VALUE) return false_t;
            self->dev = CreateFile                               (
                name                              ,
                GENERIC_WRITE                     ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                0                                 ,
                OPEN_EXISTING                     ,
                FILE_FLAG_OVERLAPPED              ,
                0
            );

            if (self->dev == INVALID_HANDLE_VALUE) return false_t;
            self->ioc = CreateIoCompletionPort                   (
                self->dev     ,
                self->run->hnd,
                self->run     ,
                0
            );

            if(!self->dev)                      {
                CloseHandle(self->dev)          ;
                self->dev = INVALID_HANDLE_VALUE;
                return false_t                  ;
            }

            return true_t;
}

bool_t 
    out_new
        (out* self, const char* name)                            {
            if (trait_of(self) != out_t)           return false_t;
            if (self->dev != INVALID_HANDLE_VALUE) return false_t;
            self->dev = CreateFile                               (
                name                              ,
                GENERIC_WRITE                     ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                0                                 ,
                CREATE_NEW                        ,
                FILE_FLAG_OVERLAPPED              ,
                0
            );

            if (self->dev == INVALID_HANDLE_VALUE) return false_t;
            self->ioc = CreateIoCompletionPort                   (
                self->dev     ,
                self->run->hnd,
                self->run     ,
                0
            );

            if(!self->ioc)                      {
                CloseHandle(self->dev)          ;
                self->dev = INVALID_HANDLE_VALUE;
                return false_t                  ;
            }

            return true_t;
}

void
    out_close
        (out* self)                                      {
            if (self->dev == INVALID_HANDLE_VALUE) return;
            CloseHandle(self->dev);
            CloseHandle(self->ioc);

            self->ioc = INVALID_HANDLE_VALUE;
            self->dev = INVALID_HANDLE_VALUE;
}

fut*
    out_write
        (out* self, u8_t* buf, u64_t len)                       {
            if (trait_of(self) != out_t)           return null_t;
            if (self->dev == INVALID_HANDLE_VALUE) return null_t;
            
            io_res *ret = make (io_res) from (1, self->run);
            if (trait_of(ret) != io_res_t) return 0;

            ret->res.Offset     = -1;
            ret->res.OffsetHigh = -1;
            bool_t res = WriteFile  (
                self->dev,
                buf      ,
                len      , 
                null_t   ,
                &ret->res
            );

            fut* fut = io_res_fut(ret);
			if (ret && GetLastError() != ERROR_IO_PENDING) {
                ret->stat = fut_err       ;
                ret->ret  = GetLastError();
                del   (ret);
				return fut ;
			}

            del   (ret);
			return fut ;
}

u64_t
    out_seek
        (out* self, u64_t pos)                    {
            if (trait_of(self) != out_t) return -1;
            u64_t  ret = self->pos;
            self ->pos = pos;
            return ret;
            
}

u64_t
    out_pos
        (out* self)                               {
            if (trait_of(self) != out_t) return -1;
            return self->pos;
}