#ifndef NET_H
#define NET_H

#include "net/acp.h"
#include "net/tcp.h"

#include "net/udp.h"
#include "net/v4.h"
#include "net/v6.h"
#include "net/end.h"

extern obj_trait *net_t;
extern struct     net { u8_t net[2 KB]; } net;

v4*  new_v4 (const char*);
v6*  new_v6 (const char*);

acp* new_acp(end*, obj_trait*);
tcp* new_tcp(obj_trait*);
udp* new_udp(obj_trait*);

#endif