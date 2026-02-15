#include "shared-module/simplefoc/LowPassFilter.h"
#include "shared-bindings/time/__init__.h"
#include <math.h>

// Constants
#define TIMEOUT_THRESHOLD_S 0.3f  // Reset filter if no calls for >300ms

void common_hal_simplefoc_lowpassfilter_construct(simplefoc_lowpassfilter_obj_t *self, float Tf) {
    self->Tf = Tf;
    self->y_prev = 0.0f;
    
    // Initialize timestamp to current time (in microseconds)
    self->timestamp_prev = common_hal_time_monotonic_ns() / 1000ULL;
}

float common_hal_simplefoc_lowpassfilter_filter(simplefoc_lowpassfilter_obj_t *self, float x) {
    // Get current timestamp (in microseconds)
    uint64_t timestamp = common_hal_time_monotonic_ns() / 1000ULL;
    
    // Calculate time step in seconds
    float dt = (timestamp - self->timestamp_prev) * 1e-6f;
    
    // Handle edge cases (SimpleFOC algorithm)
    if (dt < 0.0f) {
        // Overflow or timing error - use default 1ms
        dt = 1e-3f;
    } else if (dt > TIMEOUT_THRESHOLD_S) {
        // If more than 300ms has passed, reset filter to input value
        // This prevents stale state from affecting new measurements
        self->y_prev = x;
        self->timestamp_prev = timestamp;
        return x;
    }
    
    // Check for invalid time constant
    if (self->Tf <= 0.0f) {
        // No filtering if Tf is zero or negative
        return x;
    }
    
    // SimpleFOC Low-Pass Filter Algorithm (CORRECTED!)
    // α = Tf / (Tf + dt)
    // y = α * y_prev + (1 - α) * x
    //
    // Performance optimization: Calculate alpha directly
    float Tf_plus_dt = self->Tf + dt;
    float alpha = self->Tf / Tf_plus_dt;
    float one_minus_alpha = dt / Tf_plus_dt;  // Equivalent to (1 - alpha), avoids subtraction
    
    // Apply filter
    float y = alpha * self->y_prev + one_minus_alpha * x;
    
    // Save state for next iteration
    self->y_prev = y;
    self->timestamp_prev = timestamp;
    
    return y;
}

void common_hal_simplefoc_lowpassfilter_reset(simplefoc_lowpassfilter_obj_t *self) {
    // Reset filter state
    self->y_prev = 0.0f;
    
    // Reset timestamp to current time
    self->timestamp_prev = common_hal_time_monotonic_ns() / 1000ULL;
}

float common_hal_simplefoc_lowpassfilter_get_Tf(simplefoc_lowpassfilter_obj_t *self) {
    return self->Tf;
}

void common_hal_simplefoc_lowpassfilter_set_Tf(simplefoc_lowpassfilter_obj_t *self, float Tf) {
    self->Tf = Tf;
}
