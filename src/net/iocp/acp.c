#include "acp.h"
#include "acp/tcp.h"

static bool_t 
	do_new
		(acp* self, u32_t count, va_list arg)							 {
			io_run *run = null_t; if (count > 1) run = va_arg(arg, any_t);
			if (trait_of(run) != io_run_t) run = this_io_run();
			if (trait_of(run) != io_run_t) return false_t;

			self->run = ref(run);
			self->end = null_t;
			return true_t;
}

static bool_t 
	do_clone
		(acp* self, acp* clone) {
			return false_t;
}

static void   
	do_del
		(acp* self)	        {
			acp_close (self);
			del (self->run);
			del (self->end);
}

static obj_trait 
	do_obj = make_trait (
		do_new     ,
		do_clone   ,
		null_t     ,
		do_del     ,
		sizeof(acp),
		null_t
);

obj_trait* acp_t = &do_obj;

bool_t
	acp_open
		(acp* self, end* end, obj_trait* pro)		   {
			if (trait_of(self) != acp_t) return false_t;
            if (trait_of(end)  != end_t) return false_t;
            if (trait_of(&self->pro))    return false_t;
            self->end = end;

            if (pro == tcp_t) return do_tcp_open(self);
            return false_t;
}

void
    acp_close
        (acp* self)                            {
            if (trait_of(self) != acp_t) return;
            if (trait_of(&self->pro) == tcp_t)
                do_tcp_close(self);
}

fut*
	acp_fut
		(acp* self)									  {
			if (trait_of(self) != acp_t) return null_t;
			if (trait_of(&self->pro) == tcp_t) 
				return do_tcp_fut(self);
			return null_t;
}