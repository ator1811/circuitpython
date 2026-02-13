#ifndef MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_LOWPASSFILTER_H
#define MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_LOWPASSFILTER_H

#include "py/obj.h"

typedef struct {
    mp_obj_base_t base;
    float Tf;           // Time constant
    float y_prev;       // Previous output
} simplefoc_lowpassfilter_obj_t;

extern void common_hal_simplefoc_lowpassfilter_construct(simplefoc_lowpassfilter_obj_t *self, float Tf);
extern float common_hal_simplefoc_lowpassfilter_filter(simplefoc_lowpassfilter_obj_t *self, float value, float dt);
extern void common_hal_simplefoc_lowpassfilter_reset(simplefoc_lowpassfilter_obj_t *self);
extern float common_hal_simplefoc_lowpassfilter_get_Tf(simplefoc_lowpassfilter_obj_t *self);
extern void common_hal_simplefoc_lowpassfilter_set_Tf(simplefoc_lowpassfilter_obj_t *self, float Tf);

#endif // MICROPY_INCLUDED_SHARED_MODULE_SIMPLEFOC_LOWPASSFILTER_H
