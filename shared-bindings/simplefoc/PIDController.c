#include "shared-bindings/simplefoc/PIDController.h"
#include "shared-module/simplefoc/PIDController.h"

#include "py/objproperty.h"
#include "py/runtime.h"

//| class PIDController:
//|     """PID controller for motor control
//|
//|     This is a C implementation of the SimpleFOC PID algorithm for
//|     high-performance motor control. Automatically handles timing using
//|     microsecond timestamps."""
//|
//|     def __init__(self, P: float, I: float, D: float, ramp: float = 0.0, 
//|                  limit: float = 0.0) -> None:
//|         """Create a PID controller
//|
//|         :param float P: Proportional gain
//|         :param float I: Integral gain
//|         :param float D: Derivative gain
//|         :param float ramp: Output ramp rate (units/sec), 0 = unlimited
//|         :param float limit: Output limit (absolute), 0 = unlimited
//|
//|         The controller automatically tracks timing between calls using
//|         microsecond timestamps."""
//|         ...

mp_obj_t simplefoc_pidcontroller_make_new(const mp_obj_type_t *type,
                                           size_t n_args,
                                           size_t n_kw,
                                           const mp_obj_t *all_args) {
    enum { ARG_P, ARG_I, ARG_D, ARG_ramp, ARG_limit };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_P, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_I, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_D, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_ramp, MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_limit, MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    
    simplefoc_pidcontroller_obj_t *self = mp_obj_malloc(simplefoc_pidcontroller_obj_t, &simplefoc_pidcontroller_type);
    self->base.type = &simplefoc_pidcontroller_type;
    
    float P = mp_obj_get_float(args[ARG_P].u_obj);
    float I = mp_obj_get_float(args[ARG_I].u_obj);
    float D = mp_obj_get_float(args[ARG_D].u_obj);
    float ramp = (args[ARG_ramp].u_obj == mp_const_none) ? 0.0f : mp_obj_get_float(args[ARG_ramp].u_obj);
    float limit = (args[ARG_limit].u_obj == mp_const_none) ? 0.0f : mp_obj_get_float(args[ARG_limit].u_obj);
    
    common_hal_simplefoc_pidcontroller_construct(self, P, I, D, ramp, limit);
    
    return MP_OBJ_FROM_PTR(self);
}

//|     def calculate(self, error: float) -> float:
//|         """Calculate PID output based on error
//|
//|         :param float error: Tracking error (setpoint - measured)
//|         :return: Control output
//|
//|         This method automatically calculates the time step since the last
//|         call using microsecond timestamps. The first call or calls after
//|         long delays are handled safely."""
//|         ...

static mp_obj_t simplefoc_pidcontroller_calculate(mp_obj_t self_in, mp_obj_t error_obj) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float error = mp_obj_get_float(error_obj);
    
    float output = common_hal_simplefoc_pidcontroller_calculate(self, error);
    return mp_obj_new_float(output);
}
static MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_pidcontroller_calculate_obj, simplefoc_pidcontroller_calculate);

//|     def reset(self) -> None:
//|         """Reset PID controller state
//|
//|         Clears integral accumulator, error history, and output history.
//|         The timestamp is also reset to the current time."""
//|         ...

static mp_obj_t simplefoc_pidcontroller_reset(mp_obj_t self_in) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_simplefoc_pidcontroller_reset(self);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_pidcontroller_reset_obj, simplefoc_pidcontroller_reset);

//|     P: float
//|     """Proportional gain"""

static mp_obj_t simplefoc_pidcontroller_get_P(mp_obj_t self_in) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_pidcontroller_get_P(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_pidcontroller_get_P_obj, simplefoc_pidcontroller_get_P);

static mp_obj_t simplefoc_pidcontroller_set_P(mp_obj_t self_in, mp_obj_t P_obj) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float P = mp_obj_get_float(P_obj);
    common_hal_simplefoc_pidcontroller_set_P(self, P);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_pidcontroller_set_P_obj, simplefoc_pidcontroller_set_P);

MP_PROPERTY_GETSET(simplefoc_pidcontroller_P_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_get_P_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_set_P_obj);

//|     I: float
//|     """Integral gain"""

static mp_obj_t simplefoc_pidcontroller_get_I(mp_obj_t self_in) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_pidcontroller_get_I(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_pidcontroller_get_I_obj, simplefoc_pidcontroller_get_I);

static mp_obj_t simplefoc_pidcontroller_set_I(mp_obj_t self_in, mp_obj_t I_obj) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float I = mp_obj_get_float(I_obj);
    common_hal_simplefoc_pidcontroller_set_I(self, I);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_pidcontroller_set_I_obj, simplefoc_pidcontroller_set_I);

MP_PROPERTY_GETSET(simplefoc_pidcontroller_I_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_get_I_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_set_I_obj);

//|     D: float
//|     """Derivative gain"""

static mp_obj_t simplefoc_pidcontroller_get_D(mp_obj_t self_in) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_pidcontroller_get_D(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_pidcontroller_get_D_obj, simplefoc_pidcontroller_get_D);

static mp_obj_t simplefoc_pidcontroller_set_D(mp_obj_t self_in, mp_obj_t D_obj) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float D = mp_obj_get_float(D_obj);
    common_hal_simplefoc_pidcontroller_set_D(self, D);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_pidcontroller_set_D_obj, simplefoc_pidcontroller_set_D);

MP_PROPERTY_GETSET(simplefoc_pidcontroller_D_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_get_D_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_set_D_obj);

//|     output_ramp: float
//|     """Output ramp rate limit (units/sec), 0 = unlimited"""

static mp_obj_t simplefoc_pidcontroller_get_output_ramp(mp_obj_t self_in) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_pidcontroller_get_output_ramp(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_pidcontroller_get_output_ramp_obj, simplefoc_pidcontroller_get_output_ramp);

static mp_obj_t simplefoc_pidcontroller_set_output_ramp(mp_obj_t self_in, mp_obj_t ramp_obj) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float ramp = mp_obj_get_float(ramp_obj);
    common_hal_simplefoc_pidcontroller_set_output_ramp(self, ramp);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_pidcontroller_set_output_ramp_obj, simplefoc_pidcontroller_set_output_ramp);

MP_PROPERTY_GETSET(simplefoc_pidcontroller_output_ramp_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_get_output_ramp_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_set_output_ramp_obj);

//|     limit: float
//|     """Output limit (absolute value), 0 = unlimited"""

static mp_obj_t simplefoc_pidcontroller_get_limit(mp_obj_t self_in) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(common_hal_simplefoc_pidcontroller_get_limit(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_pidcontroller_get_limit_obj, simplefoc_pidcontroller_get_limit);

static mp_obj_t simplefoc_pidcontroller_set_limit(mp_obj_t self_in, mp_obj_t limit_obj) {
    simplefoc_pidcontroller_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float limit = mp_obj_get_float(limit_obj);
    common_hal_simplefoc_pidcontroller_set_limit(self, limit);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_pidcontroller_set_limit_obj, simplefoc_pidcontroller_set_limit);

MP_PROPERTY_GETSET(simplefoc_pidcontroller_limit_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_get_limit_obj,
                   (mp_obj_t)&simplefoc_pidcontroller_set_limit_obj);

static const mp_rom_map_elem_t simplefoc_pidcontroller_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_calculate), MP_ROM_PTR(&simplefoc_pidcontroller_calculate_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&simplefoc_pidcontroller_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_P), MP_ROM_PTR(&simplefoc_pidcontroller_P_obj) },
    { MP_ROM_QSTR(MP_QSTR_I), MP_ROM_PTR(&simplefoc_pidcontroller_I_obj) },
    { MP_ROM_QSTR(MP_QSTR_D), MP_ROM_PTR(&simplefoc_pidcontroller_D_obj) },
    { MP_ROM_QSTR(MP_QSTR_output_ramp), MP_ROM_PTR(&simplefoc_pidcontroller_output_ramp_obj) },
    { MP_ROM_QSTR(MP_QSTR_limit), MP_ROM_PTR(&simplefoc_pidcontroller_limit_obj) },
};
static MP_DEFINE_CONST_DICT(simplefoc_pidcontroller_locals_dict, simplefoc_pidcontroller_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    simplefoc_pidcontroller_type,
    MP_QSTR_PIDController,
    MP_TYPE_FLAG_HAS_SPECIAL_ACCESSORS,
    make_new, simplefoc_pidcontroller_make_new,
    locals_dict, &simplefoc_pidcontroller_locals_dict
);
