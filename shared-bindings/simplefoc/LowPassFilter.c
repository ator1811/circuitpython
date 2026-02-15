#include "shared-bindings/simplefoc/LowPassFilter.h"
#include "shared-module/simplefoc/LowPassFilter.h"

#include "py/objproperty.h"
#include "py/runtime.h"

//| class LowPassFilter:
//|     """Low-pass filter for smoothing sensor data
//|
//|     This is a C implementation of the SimpleFOC low-pass filter algorithm
//|     for high-performance sensor smoothing. Automatically handles timing
//|     using microsecond timestamps."""
//|
//|     def __init__(self, time_constant: float) -> None:
//|         """Create a low-pass filter
//|
//|         :param float time_constant: Time constant Tf in seconds
//|
//|         The time constant determines the filter response:
//|         - Smaller Tf = faster response, less smoothing
//|         - Larger Tf = slower response, more smoothing
//|
//|         Example: Tf=0.01 gives a 10ms time constant."""
//|         ...

mp_obj_t simplefoc_lowpassfilter_make_new(const mp_obj_type_t *type,
                                           size_t n_args,
                                           size_t n_kw,
                                           const mp_obj_t *all_args) {
    enum { ARG_time_constant };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_time_constant, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    simplefoc_lowpassfilter_obj_t *self = mp_obj_malloc(simplefoc_lowpassfilter_obj_t, &simplefoc_lowpassfilter_type);
    self->base.type = &simplefoc_lowpassfilter_type;
    
    float time_constant = mp_obj_get_float(args[ARG_time_constant].u_obj);
    
    common_hal_simplefoc_lowpassfilter_construct(self, time_constant);
    
    return MP_OBJ_FROM_PTR(self);
}

//|     def filter(self, x: float) -> float:
//|         """Apply the low-pass filter to a value
//|
//|         :param float x: Input value (unfiltered)
//|         :return: Filtered output value
//|
//|         The filter automatically calculates the time step since the last
//|         call using microsecond timestamps. If more than 300ms has passed,
//|         the filter resets to the input value to avoid stale state."""
//|         ...

static mp_obj_t simplefoc_lowpassfilter_filter(mp_obj_t self_in, mp_obj_t x_obj) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float x = mp_obj_get_float(x_obj);
    
    float result = common_hal_simplefoc_lowpassfilter_filter(self, x);
    return mp_obj_new_float(result);
}
static MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_lowpassfilter_filter_obj, simplefoc_lowpassfilter_filter);

//|     def reset(self) -> None:
//|         """Reset the filter state
//|
//|         Clears the previous output value and resets the timestamp.
//|         The next call to filter() will return the input value directly."""
//|         ...

static mp_obj_t simplefoc_lowpassfilter_reset(mp_obj_t self_in) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_simplefoc_lowpassfilter_reset(self);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_lowpassfilter_reset_obj, simplefoc_lowpassfilter_reset);

//|     time_constant: float
//|     """Time constant Tf of the filter in seconds (read/write)
//|
//|     Smaller values give faster response with less smoothing.
//|     Larger values give slower response with more smoothing."""

static mp_obj_t simplefoc_lowpassfilter_get_time_constant(mp_obj_t self_in) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_lowpassfilter_get_Tf(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_lowpassfilter_get_time_constant_obj, simplefoc_lowpassfilter_get_time_constant);

static mp_obj_t simplefoc_lowpassfilter_set_time_constant(mp_obj_t self_in, mp_obj_t Tf_obj) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float Tf = mp_obj_get_float(Tf_obj);
    common_hal_simplefoc_lowpassfilter_set_Tf(self, Tf);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_lowpassfilter_set_time_constant_obj, simplefoc_lowpassfilter_set_time_constant);

MP_PROPERTY_GETSET(simplefoc_lowpassfilter_time_constant_obj,
                   (mp_obj_t)&simplefoc_lowpassfilter_get_time_constant_obj,
                   (mp_obj_t)&simplefoc_lowpassfilter_set_time_constant_obj);

static const mp_rom_map_elem_t simplefoc_lowpassfilter_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_filter), MP_ROM_PTR(&simplefoc_lowpassfilter_filter_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&simplefoc_lowpassfilter_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_time_constant), MP_ROM_PTR(&simplefoc_lowpassfilter_time_constant_obj) },
};
static MP_DEFINE_CONST_DICT(simplefoc_lowpassfilter_locals_dict, simplefoc_lowpassfilter_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    simplefoc_lowpassfilter_type,
    MP_QSTR_LowPassFilter,
    MP_TYPE_FLAG_NONE,
    make_new, simplefoc_lowpassfilter_make_new,
    locals_dict, &simplefoc_lowpassfilter_locals_dict
);
