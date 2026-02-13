#include "shared-bindings/simplefoc/LowPassFilter.h"
#include "py/runtime.h"
#include "py/objproperty.h"

//| class LowPassFilter:
//|     """First-order low pass filter for signal smoothing
//|
//|     This implements the SimpleFOC low pass filter algorithm."""
//|
//|     def __init__(self, time_constant: float) -> None:
//|         """Create a low pass filter
//|
//|         :param float time_constant: Filter time constant (Tf) in seconds
//|
//|         Example::
//|
//|             import simplefoc
//|             lpf = simplefoc.LowPassFilter(time_constant=0.01)  # 10ms
//|         """
//|         ...
//|
STATIC mp_obj_t simplefoc_lowpassfilter_make_new(const mp_obj_type_t *type,
                                                   size_t n_args,
                                                   size_t n_kw,
                                                   const mp_obj_t *all_args) {
    enum { ARG_time_constant };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_time_constant, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    
    simplefoc_lowpassfilter_obj_t *self = m_new_obj(simplefoc_lowpassfilter_obj_t);
    self->base.type = &simplefoc_lowpassfilter_type;
    
    float Tf = mp_obj_get_float(args[ARG_time_constant].u_obj);
    
    common_hal_simplefoc_lowpassfilter_construct(self, Tf);
    
    return MP_OBJ_FROM_PTR(self);
}

//|     def filter(self, value: float, dt: float) -> float:
//|         """Apply low pass filter to a value
//|
//|         :param float value: Input value to filter
//|         :param float dt: Time step in seconds
//|         :return: Filtered output
//|
//|         Example::
//|
//|             filtered_speed = lpf.filter(raw_speed, dt=0.01)
//|         """
//|         ...
//|
STATIC mp_obj_t simplefoc_lowpassfilter_filter(size_t n_args, const mp_obj_t *args) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    float value = mp_obj_get_float(args[1]);
    float dt = mp_obj_get_float(args[2]);
    
    float output = common_hal_simplefoc_lowpassfilter_filter(self, value, dt);
    return mp_obj_new_float(output);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(simplefoc_lowpassfilter_filter_obj, 3, 3, simplefoc_lowpassfilter_filter);

//|     def reset(self) -> None:
//|         """Reset filter state"""
//|         ...
//|
STATIC mp_obj_t simplefoc_lowpassfilter_reset(mp_obj_t self_in) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_simplefoc_lowpassfilter_reset(self);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_lowpassfilter_reset_obj, simplefoc_lowpassfilter_reset);

//|     time_constant: float
//|     """Filter time constant (Tf) in seconds"""
STATIC mp_obj_t simplefoc_lowpassfilter_get_time_constant(mp_obj_t self_in) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_lowpassfilter_get_Tf(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_lowpassfilter_get_time_constant_obj, simplefoc_lowpassfilter_get_time_constant);

STATIC mp_obj_t simplefoc_lowpassfilter_set_time_constant(mp_obj_t self_in, mp_obj_t Tf_obj) {
    simplefoc_lowpassfilter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float Tf = mp_obj_get_float(Tf_obj);
    common_hal_simplefoc_lowpassfilter_set_Tf(self, Tf);
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

const mp_obj_type_t simplefoc_lowpassfilter_type = {
    { &mp_type_type },
    .name = MP_QSTR_LowPassFilter,
    .make_new = simplefoc_lowpassfilter_make_new,
    .locals_dict = (mp_obj_dict_t *)&simplefoc_lowpassfilter_locals_dict,
};
