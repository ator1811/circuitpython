#include "shared-module/simplefoc/SpeedCalculator.h"

void common_hal_simplefoc_speedcalculator_construct(simplefoc_speedcalculator_obj_t *self,
                                                      int ppr, float min_dt) {
    self->pulses_per_rev = ppr;
    self->min_dt = min_dt;
    self->position_prev = 0;
    self->velocity_prev = 0.0f;
    self->initialized = false;
}

float common_hal_simplefoc_speedcalculator_calculate(simplefoc_speedcalculator_obj_t *self,
                                                       int position, float dt) {
    // First call - just store position
    if (!self->initialized) {
        self->position_prev = position;
        self->initialized = true;
        return 0.0f;
    }
    
    // Check minimum dt
    if (dt < self->min_dt) {
        return self->velocity_prev;
    }
    
    // Calculate velocity (SimpleFOC algorithm)
    int delta_position = position - self->position_prev;
    float revolutions = (float)delta_position / (float)self->pulses_per_rev;
    float velocity = revolutions / dt;  // rev/s
    
    // Update state
    self->position_prev = position;
    self->velocity_prev = velocity;
    
    return velocity;
}

void common_hal_simplefoc_speedcalculator_reset(simplefoc_speedcalculator_obj_t *self) {
    self->position_prev = 0;
    self->velocity_prev = 0.0f;
    self->initialized = false;
}
