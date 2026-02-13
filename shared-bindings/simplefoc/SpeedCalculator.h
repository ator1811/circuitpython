#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_SIMPLEFOC_SPEEDCALCULATOR_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_SIMPLEFOC_SPEEDCALCULATOR_H

#include "py/obj.h"
#include "shared-module/simplefoc/SpeedCalculator.h"

extern const mp_obj_type_t simplefoc_speedcalculator_type;

extern mp_obj_t simplefoc_speedcalculator_make_new(const mp_obj_type_t *type,
                                                     size_t n_args,
                                                     size_t n_kw,
                                                     const mp_obj_t *args);

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_SIMPLEFOC_SPEEDCALCULATOR_H
