#include "models/storage/Battery.hpp"
#include <algorithm>

namespace Microgrid::Models {

Battery::Battery(double capacityWh, double maxChargePower, double maxDischargePower, double initialSoc)
    : capacityJoules(capacityWh * 3600.0), 
      pMaxCharge(maxChargePower), 
      pMaxDischarge(maxDischargePower) {
    currentEnergyJoules = capacityJoules * std::clamp(initialSoc, 0.0, 1.0);
}

double Battery::step(double requestedPower, double dt_seconds) {
    double actualPower = 0.0;

    if (requestedPower > 0) { // Charging
        actualPower = std::min(requestedPower, pMaxCharge);
        double energyToAdd = actualPower * dt_seconds * chargeEfficiency;
        if (currentEnergyJoules + energyToAdd > capacityJoules) {
            energyToAdd = capacityJoules - currentEnergyJoules;
            actualPower = energyToAdd / (dt_seconds * chargeEfficiency);
        }
        currentEnergyJoules += energyToAdd;
    } 
    else if (requestedPower < 0) { // Discharging
        double dischargeReq = -requestedPower;
        double maxAvailableP = std::min(dischargeReq, pMaxDischarge);
        
        double energyToTake = (maxAvailableP * dt_seconds) / dischargeEfficiency;
        if (currentEnergyJoules - energyToTake < 0.05 * capacityJoules) { // 5% DoD limit
            energyToTake = currentEnergyJoules - (0.05 * capacityJoules);
            if(energyToTake < 0) energyToTake = 0;
            maxAvailableP = (energyToTake * dischargeEfficiency) / dt_seconds;
        }
        currentEnergyJoules -= energyToTake;
        actualPower = -maxAvailableP;
    }

    return actualPower;
}

double Battery::getSoc() const {
    return currentEnergyJoules / capacityJoules;
}

}
