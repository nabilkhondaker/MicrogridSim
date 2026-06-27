#pragma once

namespace Microgrid::Config {
    constexpr double TIME_STEP_S = 1.0;          // Simulation time step in seconds
    constexpr double DC_BUS_VOLTAGE = 400.0;     // Fixed DC Microgrid Bus Voltage (V)
    constexpr double PWM_FREQ_HZ = 10000.0;      // Converter Switching Frequency
    constexpr double TEMP_REF_C = 25.0;          // Reference Temperature
    constexpr double IRR_REF_WM2 = 1000.0;       // Reference Irradiance
}
