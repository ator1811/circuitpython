#pragma once

#include "shared-bindings/simplefoc/Encoder.h"

// Function declarations for implementation
extern void common_hal_simplefoc_encoder_construct(simplefoc_encoder_obj_t *self,
                                                     mp_obj_t encoder_obj,
                                                     int32_t cpr);
extern void common_hal_simplefoc_encoder_update(simplefoc_encoder_obj_t *self);
extern float common_hal_simplefoc_encoder_get_angle(simplefoc_encoder_obj_t *self);
extern float common_hal_simplefoc_encoder_get_velocity(simplefoc_encoder_obj_t *self);
extern void common_hal_simplefoc_encoder_reset(simplefoc_encoder_obj_t *self);
extern int32_t common_hal_simplefoc_encoder_get_position(simplefoc_encoder_obj_t *self);
