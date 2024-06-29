#ifndef NET_EPOLL_ACP_H
#define NET_EPOLL_ACP_H

#include <core.h>
#include "tcp.h"

struct io_run;
struct end;

extern obj_trait* acp_t;
typedef struct    acp  {
	obj            head;
	struct io_run *run;
	struct end    *end;
    union      {
        obj pro;
        tcp tcp;
    };
}	acp;

bool_t acp_open (acp*, end*, obj_trait*);
void   acp_close(acp*);
fut*   acp_fut  (acp*);

#endif