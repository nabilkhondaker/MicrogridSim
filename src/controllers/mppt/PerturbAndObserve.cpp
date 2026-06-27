#include "controllers/mppt/PerturbAndObserve.hpp"
#include <cmath>
#include <algorithm>

namespace Microgrid::Controllers::MPPT {

PerturbAndObserve::PerturbAndObserve(double initialDuty, double step) {
    currentDuty = initialDuty;
    stepSize = step;
}

double PerturbAndObserve::calculateDutyCycle(double voltage, double current) {
    double power = voltage * current;
    double dP = power - prevPower;
    double dV = voltage - prevVoltage;

    if (std::abs(dP) > 0.01) { 
        if (dP > 0) {
            if (dV > 0) {
                currentDuty -= stepSize; // Decrease D to increase V_in (Boost converter)
            } else {
                currentDuty += stepSize;
            }
        } else {
            if (dV > 0) {
                currentDuty += stepSize;
            } else {
                currentDuty -= stepSize;
            }
        }
    }

    prevVoltage = voltage;
    prevPower = power;
    
    currentDuty = std::clamp(currentDuty, 0.1, 0.9);
    return currentDuty;
}

}
