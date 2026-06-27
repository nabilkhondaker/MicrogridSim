#include "models/pv/PvPanel.hpp"
#include "utils/Config.hpp"
#include <cmath>
#include <algorithm>

namespace Microgrid::Models {

PvPanel::PvPanel(Specs specifications) 
    : specs(specifications), currentIrradiance(0.0), currentTemp(25.0) {}

void PvPanel::updateEnvironment(double irradiance, double temperature) {
    currentIrradiance = std::max(0.0, irradiance);
    currentTemp = temperature;

    double deltaT = currentTemp - Config::TEMP_REF_C;
    
    // Adjust short circuit current (scales with irradiance)
    Isc_adj = (specs.Isc_ref + specs.tempCoeff_I * deltaT) * (currentIrradiance / Config::IRR_REF_WM2);
    
    // Adjust open circuit voltage
    Voc_adj = specs.Voc_ref + specs.tempCoeff_V * deltaT;
    
    // Thermal voltage calculation (Vt = Ns * k * T / q). Simplified empirical a * Vt
    Vt = (specs.idealityFact * 1.38e-23 * (currentTemp + 273.15)) / 1.6e-19; 
    // Scale up for series cells in a panel approx 60 cells
    Vt *= 60.0; 
}

double PvPanel::getCurrent(double operatingVoltage) const {
    if (currentIrradiance < 10.0 || operatingVoltage > Voc_adj) return 0.0;
    if (operatingVoltage < 0.0) return Isc_adj;

    // Simplified explicit single-diode model evaluation
    double expTerm = std::exp((operatingVoltage - Voc_adj) / Vt);
    double I = Isc_adj * (1.0 - expTerm);
    
    return std::max(0.0, I);
}

double PvPanel::getPower(double operatingVoltage) const {
    return operatingVoltage * getCurrent(operatingVoltage);
}

}
