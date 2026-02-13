#include "shared-module/simplefoc/PIDController.h"
#include <math.h>

// Constrain value between min and max
static inline float constrain(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

void common_hal_simplefoc_pidcontroller_construct(simplefoc_pidcontroller_obj_t *self,
                                                    float P, float I, float D,
                                                    float ramp, float limit) {
    self->P = P;
    self->I = I;
    self->D = D;
    self->output_ramp = ramp;
    self->output_limit = limit;
    self->integral_prev = 0.0f;
    self->error_prev = 0.0f;
    self->output_prev = 0.0f;
    self->timestamp_prev = 0;
}

float common_hal_simplefoc_pidcontroller_calculate(simplefoc_pidcontroller_obj_t *self,
                                                     float setpoint, float measured, float dt) {
    // Calculate error
    float error = setpoint - measured;
    
    // Proportional term
    float P_term = self->P * error;
    
    // Integral term with anti-windup
    float I_term = self->integral_prev + self->I * dt * error;
    
    // Derivative term
    float D_term = 0.0f;
    if (dt > 0.0f) {
        D_term = self->D * (error - self->error_prev) / dt;
    }
    
    // Calculate output
    float output = P_term + I_term + D_term;
    
    // Apply output limit
    if (self->output_limit != 0.0f) {
        output = constrain(output, -self->output_limit, self->output_limit);
    }
    
    // Apply output ramp
    if (self->output_ramp != 0.0f && dt > 0.0f) {
        float output_rate = (output - self->output_prev) / dt;
        if (fabsf(output_rate) > self->output_ramp) {
            if (output_rate > 0.0f) {
                output = self->output_prev + self->output_ramp * dt;
            } else {
                output = self->output_prev - self->output_ramp * dt;
            }
        }
    }
    
    // Update state for next iteration
    self->integral_prev = I_term;
    self->error_prev = error;
    self->output_prev = output;
    
    return output;
}

void common_hal_simplefoc_pidcontroller_reset(simplefoc_pidcontroller_obj_t *self) {
    self->integral_prev = 0.0f;
    self->error_prev = 0.0f;
    self->output_prev = 0.0f;
}

float common_hal_simplefoc_pidcontroller_get_P(simplefoc_pidcontroller_obj_t *self) {
    return self->P;
}

void common_hal_simplefoc_pidcontroller_set_P(simplefoc_pidcontroller_obj_t *self, float P) {
    self->P = P;
}

float common_hal_simplefoc_pidcontroller_get_I(simplefoc_pidcontroller_obj_t *self) {
    return self->I;
}

void common_hal_simplefoc_pidcontroller_set_I(simplefoc_pidcontroller_obj_t *self, float I) {
    self->I = I;
}

float common_hal_simplefoc_pidcontroller_get_D(simplefoc_pidcontroller_obj_t *self) {
    return self->D;
}

void common_hal_simplefoc_pidcontroller_set_D(simplefoc_pidcontroller_obj_t *self, float D) {
    self->D = D;
}
