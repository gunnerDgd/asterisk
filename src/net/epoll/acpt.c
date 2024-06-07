#include "acpt.h"
#include "end.h"

#include "acpt/tcp.h"


obj_trait acpt_trait = make_trait (
    acpt_new    ,
    acpt_clone  ,
    null_t      ,
    acpt_del    ,
    sizeof(acpt),
    null_t
);

obj_trait* acpt_t = &acpt_trait;

bool_t 
	acpt_new
		(acpt* self, u32_t count, va_list arg)                                 {
			io_sched *sched = null_t; if (count > 0) sched = va_arg(arg, any_t);
			if (trait_of(sched) != io_sched_t) sched = this_io_sched();
			if (trait_of(sched) != io_sched_t) return false_t;

            self->sched = ref (sched);
            self->end   = null_t;
			return true_t;
}

bool_t 
	acpt_clone
		(acpt* self, acpt* clone) {
			return false_t;
}

void   
	acpt_del
		(acpt* self)	     {
		    acpt_close (self);
			del (self->sched);
			del (self->end)  ;
}

bool_t
    acpt_open
        (acpt* self, end* end, obj_trait* pro)          {
            if (trait_of(self) != acpt_t) return false_t;
            if (trait_of(end)  != end_t)  return false_t;
            if (trait_of(&self->pro))     return false_t;
            self->end = end;

            if (pro == tcp_t) return do_tcp_open(self);
            return false_t;
}

void
    acpt_close
        (acpt* self)                            {
            if (trait_of(self) != acpt_t) return;
            if (trait_of(&self->pro) == tcp_t)
                do_tcp_close(self);
}

fut*
    acpt_fut
        (acpt* self)                                   {
            if (trait_of(self) != acpt_t) return null_t;
            if (trait_of(&self->pro) == tcp_t)
                return do_tcp_fut(self);
            return null_t;
}