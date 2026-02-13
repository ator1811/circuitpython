#include "shared-module/simplefoc/LowPassFilter.h"

void common_hal_simplefoc_lowpassfilter_construct(simplefoc_lowpassfilter_obj_t *self, float Tf) {
    self->Tf = Tf;
    self->y_prev = 0.0f;
}

float common_hal_simplefoc_lowpassfilter_filter(simplefoc_lowpassfilter_obj_t *self, float value, float dt) {
    // SimpleFOC low pass filter algorithm
    // y(t) = y(t-1) + (Ts / (Tf + Ts)) * (x(t) - y(t-1))
    
    if (dt <= 0.0f) {
        return self->y_prev;
    }
    
    float alpha = dt / (self->Tf + dt);
    float y = self->y_prev + alpha * (value - self->y_prev);
    
    self->y_prev = y;
    return y;
}

void common_hal_simplefoc_lowpassfilter_reset(simplefoc_lowpassfilter_obj_t *self) {
    self->y_prev = 0.0f;
}

float common_hal_simplefoc_lowpassfilter_get_Tf(simplefoc_lowpassfilter_obj_t *self) {
    return self->Tf;
}

void common_hal_simplefoc_lowpassfilter_set_Tf(simplefoc_lowpassfilter_obj_t *self, float Tf) {
    self->Tf = Tf;
}
