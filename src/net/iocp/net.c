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

__declspec(thread) struct net net;

bool_t
    net_new
		(struct net* par_net, u32_t par_count, va_list par)					  {
			if (WSAStartup(MAKEWORD(2, 2), &par_net->ws2_info)) return false_t;
			u64_t ret = 0;
			u64_t len = 0;

			par_net->ws2 = WSASocketW (
				AF_INET           ,
				SOCK_STREAM       ,
				IPPROTO_TCP       ,
				0                 ,
				0                 ,
				WSA_FLAG_OVERLAPPED
			);

			if (par_net->ws2 == INVALID_SOCKET) return false_t;
			ret = WSAIoctl									  (
				par_net->ws2					  ,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&ConnectExGuid					  ,
				sizeof(GUID)					  ,
				&ConnectEx						  ,
				sizeof(LPFN_CONNECTEX)			  ,
				&len							  ,
				NULL							  ,
				NULL
			);

			if (ret) return false_t;
			ret = WSAIoctl						  (
				par_net->ws2					  ,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&DisconnectExGuid				  ,
				sizeof(GUID)					  ,
				&DisconnectEx					  ,
				sizeof(LPFN_DISCONNECTEX)		  ,
				&len							  ,
				NULL							  ,
				NULL
			);

			if (ret) return false_t;
			return true_t;

}

bool_t net_clone(struct net* par, struct net* par_clone) { return false_t; }
bool_t net_ref  (struct net* par)						 { return false_t; }

void   
	net_del
		(struct net* par)		 {
			closesocket(par->ws2);
			WSACleanup ()		 ;
}