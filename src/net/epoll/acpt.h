#ifndef __NET_EPOLL_ACPT_H__
#define __NET_EPOLL_ACPT_H__

#include <core.h>
#include "tcp.h"

struct io_sched;
struct end;

extern obj_trait* acpt_t;
typedef struct    acpt    {
	obj              head ;
	struct io_sched *sched;
	struct end      *end;
    union      {
        obj pro;
        tcp tcp;
    };
}	acpt;

bool_t     acpt_new  (acpt*, u32_t, va_list);
bool_t     acpt_clone(acpt*, acpt*)         ;
void       acpt_del  (acpt*)				;

bool_t     acpt_open (acpt*, end*, obj_trait*);
void       acpt_close(acpt*);
fut*       acpt_fut  (acpt*);

#endif