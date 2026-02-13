#pragma once

#include "py/obj.h"

extern const mp_obj_type_t simplefoc_pidcontroller_type;

// Note: make_new must be public (non-static)
extern mp_obj_t simplefoc_pidcontroller_make_new(const mp_obj_type_t *type,
                                                   size_t n_args,
                                                   size_t n_kw,
                                                   const mp_obj_t *all_args);

typedef struct {
    mp_obj_base_t base;
    float P;
    float I;
    float D;
    float output_ramp;
    float limit;
    float integral_prev;
    float error_prev;
    float output_prev;
} simplefoc_pidcontroller_obj_t;