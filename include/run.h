#ifndef __RUN_H__
#define __RUN_H__

#ifdef PRESET_LINUX
#define  run_async() \
    void async_run();                                         \
    void run      (){                                         \
        fut*  fut = async((void*(*)(void*))async_run, null_t);\
        while(fut_poll(fut) == fut_pend);                     \
        del  (fut);                                           \
    }                                                         \
    void async_run()
#endif

#ifdef PRESET_WIN32
#define  run_async() \
    void async_run();                                         \
    void run      (){                                         \
                    \
        fut*  fut = async((void*(*)(void*))async_run, null_t);\
        while(fut_poll(fut) == fut_pend);                     \
        del  (fut);                                           \
    }                                                         \
    void async_run()
#endif

#endif
