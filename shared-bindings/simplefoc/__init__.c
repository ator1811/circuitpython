#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/simplefoc/__init__.h"
#include "shared-bindings/simplefoc/PIDController.h"
#include "shared-bindings/simplefoc/LowPassFilter.h"
#include "shared-bindings/simplefoc/SpeedCalculator.h"

//| """SimpleFOC Motor Control Library
//|
//| High-performance C implementation of SimpleFOC algorithms for CircuitPython.
//| Provides PID control, low-pass filtering, and velocity calculation."""

static const mp_rom_map_elem_t simplefoc_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_simplefoc) },
    { MP_ROM_QSTR(MP_QSTR_PIDController), MP_ROM_PTR(&simplefoc_pidcontroller_type) },
    { MP_ROM_QSTR(MP_QSTR_LowPassFilter), MP_ROM_PTR(&simplefoc_lowpassfilter_type) },
    { MP_ROM_QSTR(MP_QSTR_SpeedCalculator), MP_ROM_PTR(&simplefoc_speedcalculator_type) },
};

static MP_DEFINE_CONST_DICT(simplefoc_module_globals, simplefoc_module_globals_table);

const mp_obj_module_t simplefoc_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&simplefoc_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_simplefoc, simplefoc_module);