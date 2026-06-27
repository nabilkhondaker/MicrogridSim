#include "models/converters/DcDcConverter.hpp"
#include <algorithm>
#include <cmath>

namespace Microgrid::Models {

DcDcConverter::DcDcConverter(Topology type, double efficiencyBase)
    : topology(type), baseEff(efficiencyBase) {}

double DcDcConverter::calculateInputVoltage(double vOutBus, double dutyCycle) const {
    double d = std::clamp(dutyCycle, 0.01, 0.99);
    switch (topology) {
        case Topology::BUCK:
            return vOutBus / d;
        case Topology::BOOST:
            return vOutBus * (1.0 - d);
        case Topology::BUCK_BOOST:
            return vOutBus * (1.0 - d) / d;
        default:
            return vOutBus;
    }
}

double DcDcConverter::calculateEfficiency(double inputPower) const {
    // Efficiency drops at very low power due to switching losses
    if (inputPower < 10.0) return baseEff * 0.5;
    double lossFactor = std::exp(-inputPower / 500.0) * 0.1; 
    return baseEff - lossFactor;
}

}
