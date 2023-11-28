#ifndef __RUN_H__
#define __RUN_H__

#include "thd.h"
#include "task.h"

#include <obj.h>

#define run()\
    bool_t cstd_mem_res_new() { return true_t; }                                  \
    void   cstd_mem_res_del() { return true_t; }                                  \
                                                                                  \
    void*  cstd_mem_new(mem_res* par, u64_t par_size) { return malloc(par_size); }\
    void   cstd_mem_del(mem_res* par, void* par_del)  { free(par_del); }          \
                                                                                  \
    mem_res_trait cstd_mem_res_trait = {                                          \
	    .on_new = &cstd_mem_res_new,                                              \
	    .on_del = &cstd_mem_res_del,                                              \
                                                                                  \
    	.on_mem_new = &cstd_mem_new,                                              \
	    .on_mem_del = &cstd_mem_del                                               \
    };                                                                            \
                                                                                  \
    mem_res cstd_mem_res;                                                         \
    void* async_main();                                                           \
    int         main() {                                                          \
        mem_res_new(&cstd_mem_res, &cstd_mem_res_trait, 0);                       \
	    set_mem_res(&cstd_mem_res);                                               \
        void* thd = make(thd_t) from (2, async_main, 0);                          \
        del  (thd);                                                               \
    }                                                                             \
                                                                                  \
    void* async_main()

#define run_thd(count)\
    bool_t cstd_mem_res_new() { return true_t; }                                  \
    void   cstd_mem_res_del() { return true_t; }                                  \
                                                                                  \
    void*  cstd_mem_new(mem_res* par, u64_t par_size) { return malloc(par_size); }\
    void   cstd_mem_del(mem_res* par, void* par_del)  { free(par_del); }          \
                                                                                  \
    mem_res_trait cstd_mem_res_trait = {                                          \
	    .on_new = &cstd_mem_res_new,                                              \
	    .on_del = &cstd_mem_res_del,                                              \
                                                                                  \
    	.on_mem_new = &cstd_mem_new,                                              \
	    .on_mem_del = &cstd_mem_del                                               \
    };                                                                            \
                                                                                  \
    mem_res cstd_mem_res;                                                         \
    void* async_main();                                                           \
    int         main() {                                                          \
        mem_res_new(&cstd_mem_res, &cstd_mem_res_trait, 0);                       \
	    set_mem_res(&cstd_mem_res);                                               \
                                                                                  \
        obj* thd[count];                                                          \
        for(u64_t thd_idx = 0 ; thd_idx < count ; ++thd_idx) {                    \
            thd[thd_idx] = make(thd_t) from (2, async_main, 0);                   \
        }                                                                         \
                                                                                  \
        for(u64_t thd_idx = 0 ; thd_idx < count ; ++thd_idx) {                    \
            del(thd[thd_idx]);                                                    \
        }                                                                         \
    }                                                                             \
                                                                                  \
    void* async_main()

#endif