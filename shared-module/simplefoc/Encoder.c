#include "shared-module/simplefoc/Encoder.h"
#include "shared-bindings/time/__init__.h"
#include "py/runtime.h"
#include <math.h>

// Constants
#define _2PI 6.28318530718f
#define VELOCITY_TIMEOUT_S 0.1f  // Reset velocity to 0 if no movement for 100ms

// Helper function to read position from rotaryio.IncrementalEncoder object
static inline int32_t read_encoder_position(mp_obj_t encoder_obj) {
    mp_obj_t position_obj = mp_load_attr(encoder_obj, MP_QSTR_position);
    return mp_obj_get_int(position_obj);
}

// Helper function to write position to rotaryio.IncrementalEncoder object
// rotaryio.IncrementalEncoder.position is writable in CircuitPython
static inline void write_encoder_position(mp_obj_t encoder_obj, int32_t count) {
    mp_store_attr(encoder_obj, MP_QSTR_position, mp_obj_new_int(count));
}

void common_hal_simplefoc_encoder_construct(simplefoc_encoder_obj_t *self,
                                              mp_obj_t encoder_obj,
                                              int32_t cpr) {
    // Store encoder object reference
    self->encoder_obj = encoder_obj;
    self->cpr = cpr;

    // Pre-calculate reciprocals for performance (avoid divisions in hot path)
    self->cpr_recip = 1.0f / (float)cpr;
    self->angle_per_count = _2PI * self->cpr_recip;

    // Initialize SimpleFOC velocity calculation state
    self->pulse_per_second = 0.0f;
    self->prev_Th = 0.0f;

    // Get current timestamp and position
    uint64_t timestamp_now = common_hal_time_monotonic_ns() / 1000ULL;
    int32_t position_now = read_encoder_position(encoder_obj);

    self->prev_timestamp_us = timestamp_now;
    self->pulse_timestamp_us = timestamp_now;
    self->prev_pulse_counter = position_now;
    self->pulse_counter = position_now;
}

void common_hal_simplefoc_encoder_update(simplefoc_encoder_obj_t *self) {
    // Read current position from hardware encoder
    int32_t position_now = read_encoder_position(self->encoder_obj);

    // Check if position changed (pulse detected)
    if (position_now != self->pulse_counter) {
        // Update pulse timestamp and counter
        self->pulse_timestamp_us = common_hal_time_monotonic_ns() / 1000ULL;
        self->pulse_counter = position_now;
    }
}

float common_hal_simplefoc_encoder_get_angle(simplefoc_encoder_obj_t *self) {
    // Convert counts to radians using pre-calculated constant
    // angle = position * (2π / cpr)
    return (float)self->pulse_counter * self->angle_per_count;
}

float common_hal_simplefoc_encoder_get_velocity(simplefoc_encoder_obj_t *self) {
    // SimpleFOC mixed time/frequency velocity calculation
    // Based on Encoder.cpp getVelocity()

    // Get current timestamp (microseconds)
    uint64_t timestamp_us = common_hal_time_monotonic_ns() / 1000ULL;

    // Sampling time calculation (time since last getVelocity call)
    float Ts = (timestamp_us - self->prev_timestamp_us) * 1e-6f;

    // Quick fix for strange cases (micros overflow or first call)
    if (Ts <= 0.0f || Ts > 0.5f) {
        Ts = 1e-3f;  // Default to 1ms
    }

    // Time from last position change (impulse)
    float Th = (timestamp_us - self->pulse_timestamp_us) * 1e-6f;

    // Position change since last velocity calculation
    int32_t dN = self->pulse_counter - self->prev_pulse_counter;

    // Mixed time/frequency calculation (SimpleFOC Eq.3)
    // dt combines sampling time and impulse timing for better accuracy
    float dt = Ts + self->prev_Th - Th;

    // Update pulse_per_second only if:
    // 1. Position changed (dN != 0)
    // 2. dt is reasonable (> Ts/2 prevents division by very small numbers)
    float half_Ts = Ts * 0.5f;
    if (dN != 0 && dt > half_Ts) {
        self->pulse_per_second = (float)dN / dt;
    }
    // else: keep previous pulse_per_second value

    // Timeout: if more than 100ms passed without position change, velocity = 0
    if (Th > VELOCITY_TIMEOUT_S) {
        self->pulse_per_second = 0.0f;
    }

    // Convert pulse_per_second to rad/s
    // velocity = (pulses/sec) / cpr * 2π
    // Optimized: velocity = pulse_per_second * cpr_recip * 2π
    float velocity = self->pulse_per_second * self->cpr_recip * _2PI;

    // Save state for next iteration
    self->prev_timestamp_us = timestamp_us;
    self->prev_Th = Th;
    self->prev_pulse_counter = self->pulse_counter;

    return velocity;
}

void common_hal_simplefoc_encoder_reset(simplefoc_encoder_obj_t *self) {
    // Reset velocity calculation state (does NOT change position)
    self->pulse_per_second = 0.0f;
    self->prev_Th = 0.0f;

    // Resync timestamps and counters to current hardware state
    uint64_t timestamp_now = common_hal_time_monotonic_ns() / 1000ULL;
    int32_t position_now = read_encoder_position(self->encoder_obj);

    self->prev_timestamp_us = timestamp_now;
    self->pulse_timestamp_us = timestamp_now;
    self->prev_pulse_counter = position_now;
    self->pulse_counter = position_now;
}

int32_t common_hal_simplefoc_encoder_get_position(simplefoc_encoder_obj_t *self) {
    // Return cached position in counts (updated by update() method)
    return self->pulse_counter;
}

void common_hal_simplefoc_encoder_set_angle(simplefoc_encoder_obj_t *self, float angle) {
    // Set the current shaft angle to a desired value (in radians).
    //
    // Use cases:
    //   encoder.set_angle(0.0)      # zero the position counter
    //   encoder.set_angle(3.14159)  # preset to π radians
    //
    // How it works:
    //   1. Convert desired angle (rad) → encoder counts
    //   2. Write new count to rotaryio.IncrementalEncoder.position
    //      (CircuitPython allows writing this property)
    //   3. Update all internal velocity state so there is no
    //      spurious velocity spike from the position discontinuity.

    // Step 1: convert radians to counts
    // counts = angle / angle_per_count
    //        = angle * cpr / 2π
    // Use angle_per_count (pre-calculated) to avoid an extra division.
    int32_t new_count = (int32_t)(angle / self->angle_per_count);

    // Step 2: write new position to the hardware encoder object
    write_encoder_position(self->encoder_obj, new_count);

    // Step 3: update all internal state atomically so velocity
    //         calculation does not see a spurious jump.
    uint64_t timestamp_now = common_hal_time_monotonic_ns() / 1000ULL;

    self->pulse_counter      = new_count;
    self->prev_pulse_counter = new_count;  // dN = 0 on next velocity call
    self->pulse_timestamp_us = timestamp_now;
    self->prev_timestamp_us  = timestamp_now;

    // Clear velocity state - position jumped, old velocity is meaningless
    self->pulse_per_second = 0.0f;
    self->prev_Th          = 0.0f;
}
