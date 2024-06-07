#include "out.h"

obj_trait out_trait = make_trait (
    out_new    ,
    out_clone  ,
    null_t    ,
    out_del    ,
    sizeof(out),
    null_t
);

obj_trait *out_t = &out_trait;

bool_t
    out_new
        (out* self, u32_t count, va_list arg)                                       {
            io_sched *sched = null_t; if (count > 0) sched = va_arg(arg, io_sched*);
            if (trait_of(sched) != io_sched_t)       sched = this_io_sched();
            if (trait_of(sched) != io_sched_t)       return false_t;
            if (sched->hnd == INVALID_HANDLE_VALUE)  return false_t;
            
            self->sched = ref(sched)          ;
            self->ioc   = INVALID_HANDLE_VALUE;
            self->dev   = INVALID_HANDLE_VALUE;
            self->pos   = 0                   ;
            return true_t;
}

bool_t 
    out_clone
        (out* self, out* clone) {
            return false_t;
}

void   
    out_del
        (out* self)               {
            CloseHandle(self->ioc);
            CloseHandle(self->dev);
            del(self->sched);
            
}

bool_t
    out_open
        (out* self, str* name)                         {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != out_t) return false_t;
            return out_open_cstr(self, str_ptr(name));
}

bool_t
    out_open_cstr
        (out* self, const char* name)                              {
            if (trait_of(self)        != out_t)      return false_t;
            if (trait_of(self->sched) != io_sched_t) return false_t;
            if (self->dev != INVALID_HANDLE_VALUE)   return false_t;
            self->dev = CreateFile                                 (
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
                self->dev       ,
                self->sched->hnd,
                self->sched     ,
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
    out_create
        (out* self, str* name)                          {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != out_t)  return false_t;
            return file_create_cstr(self, str_ptr(name));
}

bool_t 
    out_create_cstr
        (out* self, const char* name)                              {
            if (trait_of(self)        != out_t)      return false_t;
            if (trait_of(self->sched) != io_sched_t) return false_t;
            if (self->dev != INVALID_HANDLE_VALUE)   return false_t;
            self->dev = CreateFile                                 (
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
                self->dev       ,
                self->sched->hnd,
                self->sched     ,
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
        (out* self, u8_t* buf, u64_t len)                         {
            if (trait_of(self)        != out_t)      return null_t;
            if (trait_of(self->sched) != io_sched_t) return null_t;
            if (self->dev == INVALID_HANDLE_VALUE)   return null_t;
            
            io_res *ret = make (io_res) from (1, self->sched);
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