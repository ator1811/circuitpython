#pragma once

#include "shared-bindings/simplefoc/SpeedCalculator.h"

// Function declarations for implementation
extern void common_hal_simplefoc_speedcalculator_construct(simplefoc_speedcalculator_obj_t *self, int ppr, float min_dt);
extern float common_hal_simplefoc_speedcalculator_calculate(simplefoc_speedcalculator_obj_t *self, int position, float dt);
extern void common_hal_simplefoc_speedcalculator_reset(simplefoc_speedcalculator_obj_t *self);