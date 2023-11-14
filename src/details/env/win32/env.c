#include "env.h"

LPFN_CONNECTEX    ConnectEx							   ;
GUID			  ConnectExGuid    = WSAID_CONNECTEX   ;
LPFN_DISCONNECTEX DisconnectEx						   ;
GUID			  DisconnectExGuid = WSAID_DISCONNECTEX;
WSADATA			  EnvInfo							   ;
SOCKET			  Env			   = INVALID_SOCKET    ;

bool_t
	__env_init()							 	{
		if(WSAStartup(MAKEWORD(2, 2), &EnvInfo))
			return false_t;

		Env = WSASocketW	   (
			AF_INET			   ,
			SOCK_STREAM		   ,
			IPPROTO_TCP		   ,
			0		           ,
			0				   ,
			WSA_FLAG_OVERLAPPED
		);

		if (Env == INVALID_SOCKET)
			goto __env_init_failed;

		u64_t ret_size;
		i32_t ret     ;
		ret = WSAIoctl						  (
			Env								  ,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&ConnectExGuid					  ,
			sizeof(GUID)					  ,
			&ConnectEx						  ,
			sizeof(LPFN_CONNECTEX)			  ,
			&ret_size						  ,
			NULL							  ,
			NULL
		);

		if (ret)								goto __env_init_failed;
		if (ret_size != sizeof(LPFN_CONNECTEX)) goto __env_init_failed;

		ret = WSAIoctl						  (
			Env								  ,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&DisconnectExGuid				  ,
			sizeof(GUID)					  ,
			&DisconnectEx					  ,
			sizeof(LPFN_DISCONNECTEX)		  ,
			&ret_size						  ,
			NULL							  ,
			NULL
		);

		if (ret)								goto __env_init_failed;
		if (ret_size != sizeof(LPFN_CONNECTEX)) goto __env_init_failed;

		return true_t;
	__env_init_failed:
		WSACleanup  ()		 ;
		closesocket (Env);
		return false_t;
}

void
	__env_deinit()		{
		closesocket(Env);
		WSACleanup ()	;
}