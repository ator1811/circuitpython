#pragma once

#include "shared-bindings/simplefoc/PIDController.h"

// Function declarations for implementation
extern void common_hal_simplefoc_pidcontroller_construct(simplefoc_pidcontroller_obj_t *self, 
                                                           float P, float I, float D, 
                                                           float ramp, float limit);
extern float common_hal_simplefoc_pidcontroller_calculate(simplefoc_pidcontroller_obj_t *self, 
                                                            float error);
extern void common_hal_simplefoc_pidcontroller_reset(simplefoc_pidcontroller_obj_t *self);

// Property getters and setters
extern float common_hal_simplefoc_pidcontroller_get_P(simplefoc_pidcontroller_obj_t *self);
extern void common_hal_simplefoc_pidcontroller_set_P(simplefoc_pidcontroller_obj_t *self, float P);

extern float common_hal_simplefoc_pidcontroller_get_I(simplefoc_pidcontroller_obj_t *self);
extern void common_hal_simplefoc_pidcontroller_set_I(simplefoc_pidcontroller_obj_t *self, float I);

extern float common_hal_simplefoc_pidcontroller_get_D(simplefoc_pidcontroller_obj_t *self);
extern void common_hal_simplefoc_pidcontroller_set_D(simplefoc_pidcontroller_obj_t *self, float D);

extern float common_hal_simplefoc_pidcontroller_get_output_ramp(simplefoc_pidcontroller_obj_t *self);
extern void common_hal_simplefoc_pidcontroller_set_output_ramp(simplefoc_pidcontroller_obj_t *self, float ramp);

extern float common_hal_simplefoc_pidcontroller_get_limit(simplefoc_pidcontroller_obj_t *self);
extern void common_hal_simplefoc_pidcontroller_set_limit(simplefoc_pidcontroller_obj_t *self, float limit);
