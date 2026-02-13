#ifndef MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_PIDCONTROLLER_H
#define MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_PIDCONTROLLER_H

#include "py/obj.h"

typedef struct {
    mp_obj_base_t base;
    float P;
    float I;
    float D;
    float output_ramp;
    float output_limit;
    float integral_prev;
    float error_prev;
    float output_prev;
    uint64_t timestamp_prev;
} simplefoc_pidcontroller_obj_t;

void common_hal_simplefoc_pidcontroller_construct(simplefoc_pidcontroller_obj_t *self,
                                                    float P, float I, float D,
                                                    float ramp, float limit);
float common_hal_simplefoc_pidcontroller_calculate(simplefoc_pidcontroller_obj_t *self,
                                                     float setpoint, float measured, float dt);
void common_hal_simplefoc_pidcontroller_reset(simplefoc_pidcontroller_obj_t *self);
float common_hal_simplefoc_pidcontroller_get_P(simplefoc_pidcontroller_obj_t *self);
void common_hal_simplefoc_pidcontroller_set_P(simplefoc_pidcontroller_obj_t *self, float P);
float common_hal_simplefoc_pidcontroller_get_I(simplefoc_pidcontroller_obj_t *self);
void common_hal_simplefoc_pidcontroller_set_I(simplefoc_pidcontroller_obj_t *self, float I);
float common_hal_simplefoc_pidcontroller_get_D(simplefoc_pidcontroller_obj_t *self);
void common_hal_simplefoc_pidcontroller_set_D(simplefoc_pidcontroller_obj_t *self, float D);

#endif // MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_PIDCONTROLLER_H
