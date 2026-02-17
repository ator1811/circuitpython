#include "shared-bindings/simplefoc/Encoder.h"
#include "shared-module/simplefoc/Encoder.h"

#include "py/objproperty.h"
#include "py/runtime.h"

//| class Encoder:
//|     """SimpleFOC Encoder with velocity calculation
//|
//|     Wraps a rotaryio.IncrementalEncoder and adds SimpleFOC's sophisticated
//|     mixed time/frequency velocity calculation algorithm. Provides position
//|     in radians and velocity in rad/s, matching the SimpleFOC Arduino API.
//|
//|     Example::
//|
//|       import rotaryio
//|       import board
//|       import simplefoc
//|
//|       # Create hardware encoder
//|       hw_encoder = rotaryio.IncrementalEncoder(board.D9, board.D10)
//|
//|       # Wrap with SimpleFOC velocity calculator
//|       encoder = simplefoc.Encoder(hw_encoder, counts_per_revolution=562)
//|
//|       while True:
//|           encoder.update()
//|           angle = encoder.get_angle()       # radians (unbounded)
//|           velocity = encoder.get_velocity() # rad/s
//|
//|           # Zero the position when it gets large
//|           if abs(angle) > 100.0:
//|               encoder.set_angle(0.0)
//|     """
//|
//|     def __init__(self, encoder: rotaryio.IncrementalEncoder,
//|                  counts_per_revolution: int) -> None:
//|         """Create a SimpleFOC Encoder
//|
//|         :param rotaryio.IncrementalEncoder encoder: Hardware encoder object
//|         :param int counts_per_revolution: Encoder CPR (counts per revolution)
//|
//|         The encoder automatically tracks timing and calculates velocity using
//|         SimpleFOC's mixed time/frequency algorithm."""
//|         ...

mp_obj_t simplefoc_encoder_make_new(const mp_obj_type_t *type,
                                     size_t n_args,
                                     size_t n_kw,
                                     const mp_obj_t *all_args) {
    enum { ARG_encoder, ARG_cpr };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_encoder, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_counts_per_revolution, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    simplefoc_encoder_obj_t *self = mp_obj_malloc(simplefoc_encoder_obj_t, &simplefoc_encoder_type);
    self->base.type = &simplefoc_encoder_type;

    mp_obj_t encoder_obj = args[ARG_encoder].u_obj;
    int32_t cpr = args[ARG_cpr].u_int;

    common_hal_simplefoc_encoder_construct(self, encoder_obj, cpr);

    return MP_OBJ_FROM_PTR(self);
}

//|     def update(self) -> None:
//|         """Update encoder state
//|
//|         Call this method regularly (every control loop iteration) to track
//|         position changes and maintain accurate velocity calculation. The
//|         SimpleFOC algorithm requires regular updates for best accuracy."""
//|         ...

static mp_obj_t simplefoc_encoder_update(mp_obj_t self_in) {
    simplefoc_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_simplefoc_encoder_update(self);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_encoder_update_obj, simplefoc_encoder_update);

//|     def get_angle(self) -> float:
//|         """Get current shaft angle
//|
//|         :return: Angle in radians (unbounded - accumulates full rotations)
//|
//|         Returns the current shaft position in radians. Unlike getSensorAngle()
//|         in SimpleFOC Arduino (which wraps 0..2π), this returns the total
//|         accumulated angle including multiple full rotations.
//|
//|         Use set_angle() to reset or preset the position counter."""
//|         ...

static mp_obj_t simplefoc_encoder_get_angle(mp_obj_t self_in) {
    simplefoc_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float angle = common_hal_simplefoc_encoder_get_angle(self);
    return mp_obj_new_float(angle);
}
static MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_encoder_get_angle_obj, simplefoc_encoder_get_angle);

//|     def get_velocity(self) -> float:
//|         """Get current shaft velocity
//|
//|         :return: Velocity in radians per second
//|
//|         Returns velocity calculated using SimpleFOC's mixed time/frequency
//|         algorithm. More accurate than simple differentiation, especially at
//|         low speeds. Automatically returns 0 if no movement for >100ms.
//|
//|         Note: velocity is reset to 0 after calling set_angle() since the
//|         position jump makes the previous velocity meaningless."""
//|         ...

static mp_obj_t simplefoc_encoder_get_velocity(mp_obj_t self_in) {
    simplefoc_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float velocity = common_hal_simplefoc_encoder_get_velocity(self);
    return mp_obj_new_float(velocity);
}
static MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_encoder_get_velocity_obj, simplefoc_encoder_get_velocity);

//|     def reset(self) -> None:
//|         """Reset velocity calculation history
//|
//|         Clears velocity calculation state and resets timestamps.
//|         The physical position reading is NOT changed.
//|         Use set_angle() if you also want to change the position."""
//|         ...

static mp_obj_t simplefoc_encoder_reset(mp_obj_t self_in) {
    simplefoc_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_simplefoc_encoder_reset(self);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_encoder_reset_obj, simplefoc_encoder_reset);

//|     def set_angle(self, angle: float) -> None:
//|         """Set the current shaft angle to a desired value
//|
//|         :param float angle: Desired angle in radians
//|
//|         Presets the encoder position counter to the given angle without
//|         physically moving the shaft. Useful for:
//|
//|         - Zeroing the position counter (set_angle(0.0))
//|         - Preventing counter overflow after many rotations
//|         - Setting a known reference position
//|         - Implementing multi-turn position tracking with periodic reset
//|
//|         After calling set_angle():
//|         - get_angle() returns the new value immediately
//|         - get_velocity() returns 0 (position jump clears velocity state)
//|         - Velocity resumes normal calculation on the next update() cycle
//|
//|         Example::
//|
//|           # Zero the position at startup
//|           encoder.set_angle(0.0)
//|
//|           # In control loop: reset when angle grows too large
//|           if abs(encoder.get_angle()) > 100.0:
//|               encoder.set_angle(0.0)
//|               position_offset += previous_angle  # track cumulative travel
//|         """
//|         ...

static mp_obj_t simplefoc_encoder_set_angle(mp_obj_t self_in, mp_obj_t angle_obj) {
    simplefoc_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    float angle = mp_obj_get_float(angle_obj);
    common_hal_simplefoc_encoder_set_angle(self, angle);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(simplefoc_encoder_set_angle_obj, simplefoc_encoder_set_angle);

//|     position: int
//|     """Current encoder position in counts (read only)
//|
//|     Direct access to the underlying hardware encoder count.
//|     Use get_angle() for position in radians."""

static mp_obj_t simplefoc_encoder_get_position(mp_obj_t self_in) {
    simplefoc_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    int32_t position = common_hal_simplefoc_encoder_get_position(self);
    return mp_obj_new_int(position);
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_encoder_get_position_obj, simplefoc_encoder_get_position);

MP_PROPERTY_GETTER(simplefoc_encoder_position_obj,
                   (mp_obj_t)&simplefoc_encoder_get_position_obj);

//|     counts_per_revolution: int
//|     """Encoder counts per revolution (CPR) - read only"""

static mp_obj_t simplefoc_encoder_get_cpr(mp_obj_t self_in) {
    simplefoc_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(self->cpr);
}
MP_DEFINE_CONST_FUN_OBJ_1(simplefoc_encoder_get_cpr_obj, simplefoc_encoder_get_cpr);

MP_PROPERTY_GETTER(simplefoc_encoder_cpr_obj,
                   (mp_obj_t)&simplefoc_encoder_get_cpr_obj);

static const mp_rom_map_elem_t simplefoc_encoder_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_update),        MP_ROM_PTR(&simplefoc_encoder_update_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_angle),     MP_ROM_PTR(&simplefoc_encoder_get_angle_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_velocity),  MP_ROM_PTR(&simplefoc_encoder_get_velocity_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset),         MP_ROM_PTR(&simplefoc_encoder_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_angle),     MP_ROM_PTR(&simplefoc_encoder_set_angle_obj) },
    { MP_ROM_QSTR(MP_QSTR_position),      MP_ROM_PTR(&simplefoc_encoder_position_obj) },
    { MP_ROM_QSTR(MP_QSTR_counts_per_revolution), MP_ROM_PTR(&simplefoc_encoder_cpr_obj) },
};
static MP_DEFINE_CONST_DICT(simplefoc_encoder_locals_dict, simplefoc_encoder_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    simplefoc_encoder_type,
    MP_QSTR_Encoder,
    MP_TYPE_FLAG_NONE,
    make_new, simplefoc_encoder_make_new,
    locals_dict, &simplefoc_encoder_locals_dict
);
