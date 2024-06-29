#ifndef NET_IOCP_ACPT_H
#define NET_IOCP_ACPT_H

#include "tcp.h"

struct end;

extern obj_trait* acp_t;
typedef struct    acp  {
	obj			head;
	io_run     *run;
	struct end *end;
	union	   {
		obj pro;
		tcp tcp;
	};
}	acp;

bool_t acp_open (acp*, end*, obj_trait*);
void   acp_close(acp*)				    ;
fut*   acp_fut  (acp*)				    ;

#endif