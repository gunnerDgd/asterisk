#include "ws2.h"

LPFN_CONNECTEX    ConnectEx							   ;
GUID			  ConnectExGuid    = WSAID_CONNECTEX   ;
LPFN_DISCONNECTEX DisconnectEx						   ;
GUID			  DisconnectExGuid = WSAID_DISCONNECTEX;
WSADATA			  ws2_info							   ;
SOCKET			  ws2			   = INVALID_SOCKET    ;

bool_t 
    ws2_new()                                                    {
        u64_t ret = 0                                            ;
        u64_t len = 0                                            ;
        if (WSAStartup(MAKEWORD(2, 2), &ws2_info)) return false_t;
        ws2 = WSASocketW      (
            AF_INET           ,
            SOCK_STREAM       ,
            IPPROTO_TCP       ,
            0                 ,
            0                 ,
            WSA_FLAG_OVERLAPPED
        );

        if (ws2 == INVALID_SOCKET) return false_t;
        ret = WSAIoctl							 (
			ws2								  ,
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
			ws2								  ,
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

void   
	ws2_del()			{
		closesocket(ws2);
		WSACleanup ()	;
}