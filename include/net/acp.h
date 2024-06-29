#ifndef NET_ACP_H
#define NET_ACP_H

#include "end.h"

extern obj_trait* acp_t;
typedef struct    acp { u8_t acpt[256]; } acp;

bool_t acp_open (acp*, end*, obj_trait*);
void   acp_close(acp*);
fut*   acp_fut  (acp*);

#endif