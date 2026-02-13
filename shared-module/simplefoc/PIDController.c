#include "shared-module/simplefoc/PIDController.h"
#include <math.h>

void common_hal_simplefoc_pidcontroller_construct(simplefoc_pidcontroller_obj_t *self,
                                                    float P, float I, float D,
                                                    float ramp, float limit) {
    self->P = P;
    self->I = I;
    self->D = D;
    self->output_ramp = ramp;
    self->limit = limit;
    self->integral_prev = 0.0f;
    self->error_prev = 0.0f;
    self->output_prev = 0.0f;
}

float common_hal_simplefoc_pidcontroller_calculate(simplefoc_pidcontroller_obj_t *self,
                                                     float setpoint, float measured, float dt) {
    if (dt <= 0.0f) {
        return self->output_prev;
    }
    
    // Calculate error
    float error = setpoint - measured;
    
    // Proportional term
    float proportional = self->P * error;
    
    // Integral term with anti-windup
    float integral = self->integral_prev + self->I * error * dt;
    
    // Derivative term
    float derivative = self->D * (error - self->error_prev) / dt;
    
    // Calculate output
    float output = proportional + integral + derivative;
    
    // Apply output limiting
    if (self->limit > 0.0f) {
        if (output > self->limit) {
            output = self->limit;
        } else if (output < -self->limit) {
            output = -self->limit;
        }
    }
    
    // Apply rate limiting (ramp)
    if (self->output_ramp > 0.0f) {
        float max_change = self->output_ramp * dt;
        float delta = output - self->output_prev;
        if (delta > max_change) {
            output = self->output_prev + max_change;
        } else if (delta < -max_change) {
            output = self->output_prev - max_change;
        }
    }
    
    // Anti-windup: only update integral if output is not saturated
    if (self->limit <= 0.0f || (fabsf(output) < self->limit)) {
        self->integral_prev = integral;
    }
    
    // Save state
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