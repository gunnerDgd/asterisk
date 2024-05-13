#include "fs.h"
#include <thread.h>

obj_trait fs_trait = make_trait (
    fs_new           ,
    fs_clone         ,
    fs_ref           ,
    fs_del           ,
    sizeof(struct fs),
    null_t
);

obj_trait *fs_t = &fs_trait;

struct fs fs;

bool_t
    fs_new
        (struct fs* self, u32_t count, va_list arg) {
#ifdef PRESET_FEATURE_THREAD
            return true_t;
#endif
            return false_t;
}

bool_t fs_clone(struct fs* self, struct fs* clone) { return false_t; }
bool_t fs_ref  (struct fs* self)                   { return false_t; }
void   fs_del  (struct fs* self)                   { return; }