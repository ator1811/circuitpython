#pragma once

#include "py/obj.h"

extern const mp_obj_type_t simplefoc_speedcalculator_type;

// Note: make_new must be public (non-static)
extern mp_obj_t simplefoc_speedcalculator_make_new(const mp_obj_type_t *type,
                                                     size_t n_args,
                                                     size_t n_kw,
                                                     const mp_obj_t *all_args);

typedef struct {
    mp_obj_base_t base;
    int pulses_per_revolution;
    float min_dt;
    int32_t position_prev;
    float velocity_prev;
} simplefoc_speedcalculator_obj_t;