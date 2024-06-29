#ifndef IO_IOCP_IO_RUN_H
#define IO_IOCP_IO_RUN_H

#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_

#include <Windows.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#include <core.h>

extern obj_trait* io_run_t   ;
typedef struct    io_run     {
	obj				 head	 ;
	u64_t			 stat	 ;
	u64_t			 pend    ;
	OVERLAPPED_ENTRY res[128];
	void			*hnd	 ;
}	io_run;

u64_t io_run_flush(io_run*);
fut*  io_run_fut  (io_run*);

#endif