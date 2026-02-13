#include "shared-module/simplefoc/LowPassFilter.h"
#include <math.h>

void common_hal_simplefoc_lowpassfilter_construct(simplefoc_lowpassfilter_obj_t *self, float Tf) {
    self->Tf = Tf;
    self->y_prev = 0.0f;
}

float common_hal_simplefoc_lowpassfilter_filter(simplefoc_lowpassfilter_obj_t *self, float x, float dt) {
    if (dt <= 0.0f || self->Tf <= 0.0f) {
        return x;
    }
    
    float alpha = dt / (self->Tf + dt);
    float y = alpha * x + (1.0f - alpha) * self->y_prev;
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