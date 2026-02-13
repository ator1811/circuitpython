#ifndef MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_SPEEDCALCULATOR_H
#define MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_SPEEDCALCULATOR_H

#include "py/obj.h"

typedef struct {
    mp_obj_base_t base;
    int pulses_per_rev;
    float min_dt;
    int position_prev;
    float velocity_prev;
    bool initialized;
} simplefoc_speedcalculator_obj_t;

void common_hal_simplefoc_speedcalculator_construct(simplefoc_speedcalculator_obj_t *self,
                                                      int ppr, float min_dt);
float common_hal_simplefoc_speedcalculator_calculate(simplefoc_speedcalculator_obj_t *self,
                                                       int position, float dt);
void common_hal_simplefoc_speedcalculator_reset(simplefoc_speedcalculator_obj_t *self);

#endif // MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_SPEEDCALCULATOR_H
