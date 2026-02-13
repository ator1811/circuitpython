#include "shared-bindings/simplefoc/LowPassFilter.h"
#include "shared-module/simplefoc/LowPassFilter.h"

#include "py/objproperty.h"
#include "py/runtime.h"

//| class LowPassFilter:
//|     """Low-pass filter for smoothing sensor data"""
//|
//|     def __init__(self, time_constant: float) -> None:
//|         """Create a low-pass filter with the given time constant.
//|
//|         :param float time_constant: Time constant in seconds"""
//|         ...
//|
//|     def filter(self, x: float, dt: float) -> float:
//|         """Apply the low-pass filter to a value.
//|
//|         :param float x: Input value
//|         :param float dt: Time step in seconds
//|         :return: Filtered output value"""
//|         ...

STATIC mp_obj_t simplefoc_lowpassfilter_make_new(const mp_obj_type_t *type,
    size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_time_constant };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_time_constant, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Get time constant
    mp_float_t time_constant = mp_obj_get_float(args[ARG_time_constant].u_obj);

    // Create object
    simplefoc_lowpassfilter_obj_t *self = mp_obj_malloc(simplefoc_lowpassfilter_obj_t, &simplefoc_lowpassfilter_type);
    self->base.type = &simplefoc_lowpassfilter_type;

    // Initialize the filter
    common_hal_simplefoc_lowpassfilter_construct(self, time_constant);

    return MP_OBJ_FROM_PTR(self);
}

//|     def reset(self) -> None:
//|         """Reset the filter state"""
//|         ...
//|
//|     time_constant: float
//|     """Time constant of the filter (read/write)"""

STATIC mp_obj_t simplefoc_lowpassfilter_filter(size_t n_args, const mp_obj_t *args) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_float_t x = mp_obj_get_float(args[1]);
    mp_float_t dt = mp_obj_get_float(args[2]);
    
    mp_float_t result = common_hal_simplefoc_lowpassfilter_filter(self, x, dt);
    return mp_obj_new_float(result);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(simplefoc_lowpassfilter_filter_obj, 3, 3, simplefoc_lowpassfilter_filter);

// reset method
STATIC mp_obj_t simplefoc_lowpassfilter_reset(mp_obj_t self_in) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_simplefoc_lowpassfilter_reset(self);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_lowpassfilter_reset_obj, simplefoc_lowpassfilter_reset);

// time_constant property getter
STATIC mp_obj_t simplefoc_lowpassfilter_get_time_constant(mp_obj_t self_in) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_lowpassfilter_get_time_constant(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_lowpassfilter_get_time_constant_obj, simplefoc_lowpassfilter_get_time_constant);

STATIC mp_obj_t simplefoc_lowpassfilter_set_time_constant(mp_obj_t self_in, mp_obj_t Tf_obj) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_float_t Tf = mp_obj_get_float(Tf_obj);
    common_hal_simplefoc_lowpassfilter_set_time_constant(self, Tf);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_lowpassfilter_set_time_constant_obj, simplefoc_lowpassfilter_set_time_constant);

MP_PROPERTY_GETSET(simplefoc_lowpassfilter_time_constant_obj,
    (mp_obj_t)&simplefoc_lowpassfilter_get_time_constant_obj,
    (mp_obj_t)&simplefoc_lowpassfilter_set_time_constant_obj);

STATIC const mp_rom_map_elem_t simplefoc_lowpassfilter_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_filter), MP_ROM_PTR(&simplefoc_lowpassfilter_filter_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&simplefoc_lowpassfilter_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_time_constant), MP_ROM_PTR(&simplefoc_lowpassfilter_time_constant_obj) },
};
STATIC MP_DEFINE_CONST_DICT(simplefoc_lowpassfilter_locals_dict, simplefoc_lowpassfilter_locals_dict_table);

// FIXED: New-style type definition for CircuitPython 10.x
MP_DEFINE_CONST_OBJ_TYPE(
    simplefoc_lowpassfilter_type,
    MP_QSTR_LowPassFilter,
    MP_TYPE_FLAG_NONE,
    make_new, simplefoc_lowpassfilter_make_new,
    locals_dict, &simplefoc_lowpassfilter_locals_dict
);
