#include "shared-module/simplefoc/SpeedCalculator.h"
#include <math.h>

void common_hal_simplefoc_speedcalculator_construct(simplefoc_speedcalculator_obj_t *self,
                                                      int ppr, float min_dt) {
    self->pulses_per_revolution = ppr;
    self->min_dt = min_dt;
    self->position_prev = 0;
    self->velocity_prev = 0.0f;
}

float common_hal_simplefoc_speedcalculator_calculate(simplefoc_speedcalculator_obj_t *self,
                                                       int position, float dt) {
    if (dt < self->min_dt) {
        return self->velocity_prev;
    }
    
    // Calculate position delta
    int32_t delta_position = position - self->position_prev;
    self->position_prev = position;
    
    // Convert to velocity (revolutions per second)
    float velocity = (float)delta_position / (float)self->pulses_per_revolution / dt;
    
    self->velocity_prev = velocity;
    return velocity;
}

void common_hal_simplefoc_speedcalculator_reset(simplefoc_speedcalculator_obj_t *self) {
    self->position_prev = 0;
    self->velocity_prev = 0.0f;
}