#pragma once

#include "py/obj.h"
#include <stdint.h>

extern const mp_obj_type_t simplefoc_pidcontroller_type;

// Note: make_new must be public (non-static)
extern mp_obj_t simplefoc_pidcontroller_make_new(const mp_obj_type_t *type,
                                                   size_t n_args,
                                                   size_t n_kw,
                                                   const mp_obj_t *all_args);

typedef struct {
    mp_obj_base_t base;
    float P;                      // Proportional gain
    float I;                      // Integral gain
    float D;                      // Derivative gain
    float output_ramp;            // Maximum speed of change of output [units/sec]
    float limit;                  // Maximum output value [units]
    float integral_prev;          // Last integral component value
    float error_prev;             // Last tracking error value
    float output_prev;            // Last PID output value
    uint64_t timestamp_prev;      // Last execution timestamp [microseconds]
} simplefoc_pidcontroller_obj_t;
