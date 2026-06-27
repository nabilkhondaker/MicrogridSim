#include "controllers/mppt/IncrementalConductance.hpp"
#include <cmath>
#include <algorithm>

namespace Microgrid::Controllers::MPPT {

IncrementalConductance::IncrementalConductance(double initialDuty, double step) {
    currentDuty = initialDuty;
    stepSize = step;
}

double IncrementalConductance::calculateDutyCycle(double voltage, double current) {
    double dV = voltage - prevVoltage;
    double dI = current - prevCurrent;

    if (std::abs(dV) < 0.01) {
        if (dI > 0) currentDuty -= stepSize;
        else if (dI < 0) currentDuty += stepSize;
    } else {
        double incCond = dI / dV;
        double cond = -current / voltage;

        if (incCond > cond) {
            currentDuty -= stepSize;
        } else if (incCond < cond) {
            currentDuty += stepSize;
        }
    }

    prevVoltage = voltage;
    prevCurrent = current;

    currentDuty = std::clamp(currentDuty, 0.1, 0.9);
    return currentDuty;
}

}
