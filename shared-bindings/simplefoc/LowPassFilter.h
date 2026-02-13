#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_SIMPLEFOC_LOWPASSFILTER_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_SIMPLEFOC_LOWPASSFILTER_H

#include "py/obj.h"
#include "shared-module/simplefoc/LowPassFilter.h"

extern const mp_obj_type_t simplefoc_lowpassfilter_type;

extern mp_obj_t simplefoc_lowpassfilter_make_new(const mp_obj_type_t *type,
                                                   size_t n_args,
                                                   size_t n_kw,
                                                   const mp_obj_t *args);

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_SIMPLEFOC_LOWPASSFILTER_H
