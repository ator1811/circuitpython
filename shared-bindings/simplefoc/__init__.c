#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/simplefoc/PIDController.h"
#include "shared-bindings/simplefoc/LowPassFilter.h"
#include "shared-bindings/simplefoc/SpeedCalculator.h"

//| """SimpleFOC motor control algorithms
//|
//| The `simplefoc` module provides high-performance motor control algorithms:
//| 
//| - PIDController: PID control algorithm
//| - LowPassFilter: First-order low pass filter for signal smoothing
//| - SpeedCalculator: Velocity calculation from rotary encoders
//|
//| These are C implementations for maximum performance, compatible with
//| CircuitPython's built-in modules like rotaryio."""
//|

STATIC const mp_rom_map_elem_t simplefoc_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_simplefoc) },
    { MP_ROM_QSTR(MP_QSTR_PIDController), MP_ROM_PTR(&simplefoc_pidcontroller_type) },
    { MP_ROM_QSTR(MP_QSTR_LowPassFilter), MP_ROM_PTR(&simplefoc_lowpassfilter_type) },
    { MP_ROM_QSTR(MP_QSTR_SpeedCalculator), MP_ROM_PTR(&simplefoc_speedcalculator_type) },
};

STATIC MP_DEFINE_CONST_DICT(simplefoc_module_globals, simplefoc_module_globals_table);

const mp_obj_module_t simplefoc_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&simplefoc_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_simplefoc, simplefoc_module);
