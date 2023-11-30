#ifndef __RUN_H__
#define __RUN_H__

#include <lib.h>
#include <thd.h>
#include <obj.h>

#ifndef __cplusplus
#define run()                                                 \
    void* async_main();                                       \
    int         main()           {                            \
        if(!lib_init()) return -1;                            \
        void* main_thd = make(thd_t) from (2, async_main, 0); \
        del(main_thd);                                        \
        lib_deinit();                                         \
    }                                                         \
                                                              \
    void* async_main()                                        \

#define run_thd(count)                                       \
    void* async_main();                                      \
    int         main()           {                           \
        if(!lib_init()) return -1;                           \
                                                             \
        void* main_thd[count];                               \
        for(u64_t i = 0 ; i < count ; ++i)                   \
            main_thd[i] = make(thd_t) from(2, async_main, 0);\
                                                             \
        for(u64_t i = 0 ; i < count ; ++i)                   \
            del(main_thd[i]);                                \
        lib_deinit();                                        \
    }                                                        \
                                                             \
    void* async_main()                                       \

#endif

#ifdef __cplusplus
#define run()                                                                \
    void* async_main();                                                      \
    int         main()           {                                           \
        if(!lib_init()) return -1;                                           \
        void*         main_thd = (void*)obj_new(0, thd_t, 2, async_main, 0); \
        obj_del((obj*)main_thd);                                             \
        lib_deinit();                                                        \
    }                                                                        \
                                                                             \
    void* async_main()                                                       \

#define run_thd(count)                                       \
    void* async_main();                                      \
    int         main()           {                           \
        if(!lib_init()) return -1;                           \
                                                             \
        void* main_thd[count];                               \
        for(u64_t i = 0 ; i < count ; ++i)                   \
            main_thd[i] = make(thd_t) from(2, async_main, 0);\
                                                             \
        for(u64_t i = 0 ; i < count ; ++i)                   \
            del(main_thd[i]);                                \
        lib_deinit();                                        \
    }                                                        \
                                                             \
    void* async_main()                                       \

#endif
#endif