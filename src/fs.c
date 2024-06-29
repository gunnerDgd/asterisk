#include "fs.h"

struct fs fs;

static bool_t
    do_new
        (struct fs* self, u32_t count, va_list arg) {
#ifdef PRESET_FEATURE_THREAD
            return true_t;
#endif
            return false_t;
}

static bool_t do_clone(struct fs* self, struct fs* clone) { return false_t; }
static bool_t do_ref  (struct fs* self)                   { return true_t ; }
static void   do_del  (struct fs* self)                   {                 }

static obj_trait
    do_fs = make_trait   (
        do_new           ,
        do_clone         ,
        do_ref           ,
        do_del           ,
        sizeof(struct fs),
        null_t
);

obj_trait *fs_t = &do_fs;

file*
    new_file
        (const char* name)                  {
            file* ret = make (file) from (0);
            if (!file_open(ret, name))
            if (!file_new (ret, name))
                goto err;

            return   ret;
    err:    del     (ret);
            return null_t;
}

out*
    new_out
        (const char* name)                {
            out *ret = make (out) from (0);

            if (!out_open(ret, name))
            if (!out_new (ret, name))
                goto err;

            return   ret;
    err:    del     (ret);
            return null_t;
}

in*
    new_in
        (const char* name)              {
            in *ret = make (in) from (0);

            if (!in_open(ret, name))
            if (!in_new (ret, name))
                goto err;

            return   ret;
    err:    del     (ret);
            return null_t;
}