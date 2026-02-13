#include "shared-bindings/simplefoc/SpeedCalculator.h"
#include "shared-module/simplefoc/SpeedCalculator.h"

#include "py/objproperty.h"
#include "py/runtime.h"

//| class SpeedCalculator:
//|     """Calculate velocity from encoder position
//|
//|     Works seamlessly with CircuitPython's rotaryio.IncrementalEncoder.
//|     Implements SimpleFOC's velocity calculation algorithm."""
//|
//|     def __init__(self, pulses_per_revolution: int, min_dt: float = 0.001) -> None:
//|         """Create a speed calculator
//|
//|         :param int pulses_per_revolution: Encoder pulses per revolution (PPR)
//|         :param float min_dt: Minimum time between updates (seconds)
//|
//|         Example::
//|
//|             import simplefoc
//|             import rotaryio
//|             import board
//|
//|             # Use standard rotaryio encoder
//|             encoder = rotaryio.IncrementalEncoder(board.D2, board.D3)
//|
//|             # Add SimpleFOC speed calculation
//|             speed_calc = simplefoc.SpeedCalculator(pulses_per_revolution=600)
//|         """
//|         ...
//|
static mp_obj_t simplefoc_speedcalculator_make_new(const mp_obj_type_t *type,
                                                     size_t n_args,
                                                     size_t n_kw,
                                                     const mp_obj_t *all_args) {
    enum { ARG_ppr, ARG_min_dt };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_pulses_per_revolution, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_min_dt, MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    
    simplefoc_speedcalculator_obj_t *self = m_new_obj(simplefoc_speedcalculator_obj_t);
    self->base.type = &simplefoc_speedcalculator_type;
    
    int ppr = args[ARG_ppr].u_int;
    float min_dt = (args[ARG_min_dt].u_obj == mp_const_none) ? 0.001f : mp_obj_get_float(args[ARG_min_dt].u_obj);
    
    common_hal_simplefoc_speedcalculator_construct(self, ppr, min_dt);
    
    return MP_OBJ_FROM_PTR(self);
}

//|     def calculate(self, position: int, dt: float) -> float:
//|         """Calculate velocity from position
//|
//|         :param int position: Current encoder position (from rotaryio)
//|         :param float dt: Time since last update (seconds)
//|         :return: Velocity in revolutions per second
//|
//|         Example::
//|
//|             import time
//|             last_time = time.monotonic()
//|
//|             while True:
//|                 current_time = time.monotonic()
//|                 dt = current_time - last_time
//|
//|                 position = encoder.position
//|                 velocity = speed_calc.calculate(position, dt)
//|
//|                 print(f"Speed: {velocity * 60:.1f} RPM")
//|
//|                 last_time = current_time
//|                 time.sleep(0.01)
//|         """
//|         ...
//|
static mp_obj_t simplefoc_speedcalculator_calculate(size_t n_args, const mp_obj_t *args) {
    simplefoc_speedcalculator_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    int position = mp_obj_get_int(args[1]);
    float dt = mp_obj_get_float(args[2]);
    
    float velocity = common_hal_simplefoc_speedcalculator_calculate(self, position, dt);
    return mp_obj_new_float(velocity);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(simplefoc_speedcalculator_calculate_obj, 3, 3, simplefoc_speedcalculator_calculate);

//|     def get_rpm(self, position: int, dt: float) -> float:
//|         """Calculate velocity in RPM
//|
//|         :param int position: Current encoder position
//|         :param float dt: Time since last update (seconds)
//|         :return: Velocity in revolutions per minute
//|         """
//|         ...
//|
static mp_obj_t simplefoc_speedcalculator_get_rpm(size_t n_args, const mp_obj_t *args) {
    simplefoc_speedcalculator_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    int position = mp_obj_get_int(args[1]);
    float dt = mp_obj_get_float(args[2]);
    
    float velocity_rps = common_hal_simplefoc_speedcalculator_calculate(self, position, dt);
    float velocity_rpm = velocity_rps * 60.0f;
    return mp_obj_new_float(velocity_rpm);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(simplefoc_speedcalculator_get_rpm_obj, 3, 3, simplefoc_speedcalculator_get_rpm);

//|     def reset(self) -> None:
//|         """Reset calculator state"""
//|         ...
//|

static mp_obj_t simplefoc_speedcalculator_reset(mp_obj_t self_in) {
    simplefoc_speedcalculator_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_simplefoc_speedcalculator_reset(self);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_speedcalculator_reset_obj, simplefoc_speedcalculator_reset);

static const mp_rom_map_elem_t simplefoc_speedcalculator_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_calculate), MP_ROM_PTR(&simplefoc_speedcalculator_calculate_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_rpm), MP_ROM_PTR(&simplefoc_speedcalculator_get_rpm_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&simplefoc_speedcalculator_reset_obj) },
};
static MP_DEFINE_CONST_DICT(simplefoc_speedcalculator_locals_dict, simplefoc_speedcalculator_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    simplefoc_speedcalculator_type,
    MP_QSTR_SpeedCalculator,
    MP_TYPE_FLAG_NONE,
    make_new, simplefoc_speedcalculator_make_new,
    locals_dict, &simplefoc_speedcalculator_locals_dict
);
