#include "net.h"

LPFN_CONNECTEX    ConnectEx							   ;
GUID			  ConnectExGuid    = WSAID_CONNECTEX   ;
LPFN_DISCONNECTEX DisconnectEx						   ;
GUID			  DisconnectExGuid = WSAID_DISCONNECTEX;

obj_trait net_trait = make_trait (
	net_new			  ,
	net_clone		  ,
	net_ref			  ,
	net_del			  ,
	sizeof(struct net),
	null_t
);

obj_trait *net_t = &net_trait;

struct net net;

bool_t
	net_do_connectex
		(struct net* self)							   {
			if (trait_of(self) != net_t) return false_t;
			u32_t len;
			u32_t ret;

			ret = WSAIoctl						  (
				self->ws2						  ,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&ConnectExGuid					  ,
				sizeof(GUID)					  ,
				&ConnectEx						  ,
				sizeof(LPFN_CONNECTEX)			  ,
				&len						      ,
				NULL						      ,
				NULL
			);

			if (ret) return false_t;
			return true_t;
}

bool_t
	net_do_disconnectex
		(struct net* self)							   {
			if (trait_of(self) != net_t) return false_t;
			u32_t len;
			u32_t ret;

			ret = WSAIoctl						  (
				self->ws2						  ,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&DisconnectExGuid				  ,
				sizeof(GUID)					  ,
				&DisconnectEx				      ,
				sizeof(LPFN_DISCONNECTEX)		  ,
				&len						      ,
				NULL						      ,
				NULL
			);

			if (ret) return false_t;
			return true_t;
}

bool_t
	net_do_ws
		(struct net* self)												   {
			if (WSAStartup(MAKEWORD(2, 2), &self->ws2_info)) return false_t;
			self->ws2 = WSASocketW (
				AF_INET			   ,
				SOCK_STREAM		   ,
				IPPROTO_TCP		   ,
				0				   ,
				0				   ,
				WSA_FLAG_OVERLAPPED
			);

			if (self->ws2 == INVALID_SOCKET) return false_t;
			return true_t;
}

bool_t
    net_new
		(struct net* self, u32_t count, va_list arg)      {
			if (!net_do_ws			(self)) return false_t;
			if (!net_do_connectex   (self)) return false_t;
			if (!net_do_disconnectex(self)) return false_t;
			return true_t;
}

bool_t net_clone(struct net* self, struct net* clone) { return false_t; }
bool_t net_ref  (struct net* self)				      { return false_t; }

void   
	net_del
		(struct net* self)		  {
			closesocket(self->ws2);
			WSACleanup ()		  ;
}