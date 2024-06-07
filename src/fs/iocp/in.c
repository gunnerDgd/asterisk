#include "in.h"

obj_trait in_trait = make_trait (
    in_new    ,
    in_clone  ,
    null_t    ,
    in_del    ,
    sizeof(in),
    null_t
);

obj_trait *in_t = &in_trait;

bool_t
    in_new
        (in* self, u32_t count, va_list arg)                                       {
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
    in_clone
        (in* self, in* clone) {
            return false_t;
}

void   
    in_del
        (in* self)                {
            CloseHandle(self->ioc);
            CloseHandle(self->dev);
            del(self->sched);
            
}

bool_t
    in_open
        (in* self, str* name)                          {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != in_t)  return false_t;
            return in_open_cstr(self, str_ptr(name));
}

bool_t
    in_open_cstr
        (in* self, const char* name)                               {
            if (trait_of(self)        != in_t)       return false_t;
            if (trait_of(self->sched) != io_sched_t) return false_t;
            if (self->dev != INVALID_HANDLE_VALUE)   return false_t;
            self->dev = CreateFile                                 (
                name                              ,
                GENERIC_READ                      ,
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

            if(!self->ioc)                      {
                CloseHandle(self->dev)          ;
                self->dev = INVALID_HANDLE_VALUE;
                return false_t                  ;
            }

            return true_t;
}

bool_t
    in_create
        (in* self, str* name)                          {
            if (trait_of(name) != str_t) return false_t;
            if (trait_of(self) != in_t)  return false_t;
            return file_create_cstr(self, str_ptr(name));
}

bool_t 
    in_create_cstr
        (in* self, const char* name)                               {
            if (trait_of(self)        != in_t)       return false_t;
            if (trait_of(self->sched) != io_sched_t) return false_t;
            if (self->dev != INVALID_HANDLE_VALUE)   return false_t;
            self->dev = CreateFile                                 (
                name                              ,
                GENERIC_READ                      ,
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
    in_close
        (in* self)                                       {
            if (self->dev == INVALID_HANDLE_VALUE) return; 
            CloseHandle(self->dev);
            CloseHandle(self->ioc);

            self->ioc = INVALID_HANDLE_VALUE;
            self->dev = INVALID_HANDLE_VALUE;
}

fut*
    in_read
        (in* self, u8_t* buf, u64_t len)                          {
            if (trait_of(self)        != in_t)       return null_t;
            if (trait_of(self->sched) != io_sched_t) return null_t;
            if (self->dev == INVALID_HANDLE_VALUE)   return null_t;

            io_res *ret = make (io_res) from (1, self->sched);
            if (trait_of(ret) != io_res_t) return null_t;

            ret->res.OffsetHigh = shl  (self->pos, 32);
            ret->res.Offset     = self->pos & mask(32);
            bool_t res = ReadFile                     (
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

            self->pos += len;
            del   (ret);
			return fut ;
}

u64_t
    in_seek
        (in* self, u64_t pos)                    {
            if (trait_of(self) != in_t) return -1;
            u64_t  ret = self->pos;
            self ->pos = pos;
            return ret;
            
}

u64_t
    in_pos
        (in* self)                               {
            if (trait_of(self) != in_t) return -1;
            return self->pos;
}