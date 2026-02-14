#pragma once

#include "py/obj.h"
#include <stdint.h>

extern const mp_obj_type_t simplefoc_encoder_type;

// Note: make_new must be public (non-static)
extern mp_obj_t simplefoc_encoder_make_new(const mp_obj_type_t *type,
                                            size_t n_args,
                                            size_t n_kw,
                                            const mp_obj_t *all_args);

typedef struct {
    mp_obj_base_t base;
    mp_obj_t encoder_obj;           // Reference to rotaryio.IncrementalEncoder object
    int32_t cpr;                    // Counts per revolution (CPR)
    
    // SimpleFOC velocity calculation state
    float pulse_per_second;         // Current pulse rate
    int32_t prev_pulse_counter;     // Previous position
    uint64_t prev_timestamp_us;     // Previous update timestamp [microseconds]
    float prev_Th;                  // Previous time from last impulse [seconds]
    uint64_t pulse_timestamp_us;    // Last position change timestamp [microseconds]
    int32_t pulse_counter;          // Current position (cached)
    
    // Pre-calculated constants for performance
    float cpr_recip;                // 1.0 / cpr (avoid division)
    float angle_per_count;          // 2π / cpr (for get_angle)
} simplefoc_encoder_obj_t;
