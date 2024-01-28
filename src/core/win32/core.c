#include "core.h"

#include <sched.h>
#include "this.h"
#include "thd.h"

LPFN_CONNECTEX    ConnectEx							   ;
GUID			  ConnectExGuid    = WSAID_CONNECTEX   ;
LPFN_DISCONNECTEX DisconnectEx						   ;
GUID			  DisconnectExGuid = WSAID_DISCONNECTEX;
WSADATA			  lib_info							   ;
SOCKET			  lib			   = INVALID_SOCKET    ;

void* lib_mem_new(mem* par, u64_t par_size) { return malloc (par_size); }
void  lib_mem_del(mem* par, void* par_del)  { free		    (par_del) ; }
mem   lib_mem_res       = {
	.on_new = &lib_mem_new,
	.on_del = &lib_mem_del
};

bool_t
	core_new()				 {
	    set_mem(&lib_mem_res);
		if(WSAStartup(MAKEWORD(2, 2), &lib_info))
			return false_t;

		lib = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
		if (lib == INVALID_SOCKET)
			goto init_failed;

		u64_t ret_size;
		i32_t ret     ;
		ret = WSAIoctl						  (
			lib								  ,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&ConnectExGuid					  ,
			sizeof(GUID)					  ,
			&ConnectEx						  ,
			sizeof(LPFN_CONNECTEX)			  ,
			&ret_size						  ,
			NULL							  ,
			NULL
		);

		if (ret) goto init_failed;
		ret = WSAIoctl						  (
			lib								  ,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&DisconnectExGuid				  ,
			sizeof(GUID)					  ,
			&DisconnectEx					  ,
			sizeof(LPFN_DISCONNECTEX)		  ,
			&ret_size						  ,
			NULL							  ,
			NULL
		);

		if (ret) goto init_failed;
		return true_t;
	init_failed:
		WSACleanup  ()	 ;
		closesocket (lib);
		return false_t;
}

void
	core_del()		    {
		closesocket(lib);
		WSACleanup ()	;
}

int main()					  {
	if (!core_new()) return -1;
	thd* thd = make (thd_t) from (2, async_main, 0);
	del (thd);

	return 0;
}