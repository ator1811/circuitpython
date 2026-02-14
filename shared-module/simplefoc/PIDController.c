#include "shared-module/simplefoc/PIDController.h"
#include "shared-bindings/time/__init__.h"
#include <math.h>

// Inline constrain function for performance (matches SimpleFOC _constrain macro)
static inline float _constrain(float amt, float low, float high) {
    return (amt < low) ? low : ((amt > high) ? high : amt);
}

void common_hal_simplefoc_pidcontroller_construct(simplefoc_pidcontroller_obj_t *self,
                                                    float P, float I, float D,
                                                    float ramp, float limit) {
    self->P = P;
    self->I = I;
    self->D = D;
    self->output_ramp = ramp;  // output derivative limit [units/second]
    self->limit = limit;       // output supply limit [units]
    self->integral_prev = 0.0f;
    self->error_prev = 0.0f;
    self->output_prev = 0.0f;
    
    // Initialize timestamp to current time (in microseconds)
    self->timestamp_prev = common_hal_time_monotonic_ns() / 1000ULL;
}

float common_hal_simplefoc_pidcontroller_calculate(simplefoc_pidcontroller_obj_t *self,
                                                     float error) {
    // Calculate the time from the last call (in microseconds, convert to seconds)
    uint64_t timestamp_now = common_hal_time_monotonic_ns() / 1000ULL;
    float Ts = (timestamp_now - self->timestamp_prev) * 1e-6f;
    
    // Quick fix for strange cases (micros overflow or first call)
    if (Ts <= 0.0f || Ts > 0.5f) {
        Ts = 1e-3f;  // Default to 1ms if timing is suspicious
    }
    
    // u(s) = (P + I/s + Ds)e(s)
    // Discrete implementations
    
    // Proportional part: u_p = P * e(k)
    float proportional = self->P * error;
    
    // Tustin transform of the integral part
    // u_ik = u_ik_1 + I*Ts/2*(e(k) + e(k-1))
    // Pre-calculate half timestep for efficiency
    float half_Ts = Ts * 0.5f;
    float integral = self->integral_prev + self->I * half_Ts * (error + self->error_prev);
    
    // Antiwindup - limit the integral component
    integral = _constrain(integral, -self->limit, self->limit);
    
    // Discrete derivation: u_dk = D*(e(k) - e(k-1))/Ts
    // Pre-calculate reciprocal for performance (division is expensive)
    float derivative = self->D * (error - self->error_prev) / Ts;
    
    // Sum all the components
    float output = proportional + integral + derivative;
    
    // Antiwindup - limit the output variable
    output = _constrain(output, -self->limit, self->limit);
    
    // If output ramp defined, limit the acceleration by ramping the output
    if (self->output_ramp > 0.0f) {
        float output_rate = (output - self->output_prev) / Ts;
        if (output_rate > self->output_ramp) {
            output = self->output_prev + self->output_ramp * Ts;
        } else if (output_rate < -self->output_ramp) {
            output = self->output_prev - self->output_ramp * Ts;
        }
    }
    
    // Save state for next iteration
    self->integral_prev = integral;
    self->output_prev = output;
    self->error_prev = error;
    self->timestamp_prev = timestamp_now;
    
    return output;
}

void common_hal_simplefoc_pidcontroller_reset(simplefoc_pidcontroller_obj_t *self) {
    self->integral_prev = 0.0f;
    self->output_prev = 0.0f;
    self->error_prev = 0.0f;
    // Reset timestamp to current time
    self->timestamp_prev = common_hal_time_monotonic_ns() / 1000ULL;
}

// Property getters and setters (inline for performance)
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

float common_hal_simplefoc_pidcontroller_get_output_ramp(simplefoc_pidcontroller_obj_t *self) {
    return self->output_ramp;
}

void common_hal_simplefoc_pidcontroller_set_output_ramp(simplefoc_pidcontroller_obj_t *self, float ramp) {
    self->output_ramp = ramp;
}

float common_hal_simplefoc_pidcontroller_get_limit(simplefoc_pidcontroller_obj_t *self) {
    return self->limit;
}

void common_hal_simplefoc_pidcontroller_set_limit(simplefoc_pidcontroller_obj_t *self, float limit) {
    self->limit = limit;
}
