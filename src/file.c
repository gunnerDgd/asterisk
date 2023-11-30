#include "file.h"
#include "details/file.h"

obj_trait* file_t = &__file_trait;

bool_t 
    file_open  
        (file* par, const char* par_path)              { 
            if (!par)                    return false_t;
            if (trait_of(par) != file_t) return false_t;

            return __file_open(par, par_path); 
}

bool_t 
    file_create
        (file* par, const char* par_path)              {
            if (!par)                    return false_t;
            if (trait_of(par) != file_t) return false_t;

            return __file_create(par, par_path); 
}

void   
    file_close 
        (file* par)                            {
            if (!par)                    return;
            if (trait_of(par) != file_t) return;
}

io_task
    file_read 
        (file* par, u8_t* par_buf, u64_t par_len) { 
            if (!par)                    return 0;
            if (trait_of(par) != file_t) return 0;
            if (!par_buf)                return 0;

            return __file_read (par, par_buf, par_len);
}

io_task
    file_write
        (file* par, u8_t* par_buf, u64_t par_len) { 
            if (!par)                    return 0;
            if (trait_of(par) != file_t) return 0;
            if (!par_buf)                return 0;

            return __file_write (par, par_buf, par_len);
}