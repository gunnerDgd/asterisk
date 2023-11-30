#include "lib.h"

LPFN_CONNECTEX    ConnectEx							   ;
GUID			  ConnectExGuid    = WSAID_CONNECTEX   ;
LPFN_DISCONNECTEX DisconnectEx						   ;
GUID			  DisconnectExGuid = WSAID_DISCONNECTEX;
WSADATA			  lib_info							   ;
SOCKET			  lib			   = INVALID_SOCKET    ;

bool_t		  lib_mem_res_new(mem_res* par_res, u32_t par_count, va_list par) { return			  true_t; }
void		  lib_mem_res_del(mem_res* par_res)							      { return				    ; }
void*		  lib_mem_new    (mem_res* par, u64_t par_size)					  { return malloc (par_size); }
void		  lib_mem_del    (mem_res* par, void* par_del)					  { free		  (par_del) ; }
mem_res		  lib_mem_res;
mem_res_trait lib_mem_res_trait    = {
	   .on_new     = &lib_mem_res_new,
	   .on_del     = &lib_mem_res_del,

	   .on_mem_new = &lib_mem_new	 ,
	   .on_mem_del = &lib_mem_del
};

bool_t
	__lib_init()							 			{
		mem_res_new(&lib_mem_res, &lib_mem_res_trait, 0);
	    set_mem_res(&lib_mem_res);

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
	__lib_deinit()		{
		closesocket(lib);
		WSACleanup ()	;
}