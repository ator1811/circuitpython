#pragma once

#include "py/obj.h"

extern const mp_obj_type_t simplefoc_lowpassfilter_type;

// Note: make_new must be public (non-static)
extern mp_obj_t simplefoc_lowpassfilter_make_new(const mp_obj_type_t *type,
                                                   size_t n_args,
                                                   size_t n_kw,
                                                   const mp_obj_t *all_args);

typedef struct {
    mp_obj_base_t base;
    float Tf;        // Time constant
    float y_prev;    // Previous output
} simplefoc_lowpassfilter_obj_t;