#include "file.h"
#include "details/file.h"

obj_trait* file_t = &__file_trait;

bool_t file_open  (file* par) { return (trait_of(par) == file_t) ? __file_open  (par) : false_t; }
bool_t file_create(file* par) { return (trait_of(par) == file_t) ? __file_create(par) : false_t; }
void   file_close (file* par) { if(trait_of(par) == file_t) __file_close(par); }

task* file_read (file* par, ptr par_buf, u64_t par_len) { return (trait_of(par) == file_t) ? __file_read (par, par_buf, par_len) : 0; }
task* file_write(file* par, ptr par_buf, u64_t par_len) { return (trait_of(par) == file_t) ? __file_write(par, par_buf, par_len) : 0; }