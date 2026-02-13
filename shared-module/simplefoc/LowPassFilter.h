#pragma once

#include "shared-bindings/simplefoc/LowPassFilter.h"

// Function declarations for implementation
extern void common_hal_simplefoc_lowpassfilter_construct(simplefoc_lowpassfilter_obj_t *self, float Tf);
extern float common_hal_simplefoc_lowpassfilter_filter(simplefoc_lowpassfilter_obj_t *self, float x, float dt);
extern void common_hal_simplefoc_lowpassfilter_reset(simplefoc_lowpassfilter_obj_t *self);
extern float common_hal_simplefoc_lowpassfilter_get_Tf(simplefoc_lowpassfilter_obj_t *self);
extern void common_hal_simplefoc_lowpassfilter_set_Tf(simplefoc_lowpassfilter_obj_t *self, float Tf);