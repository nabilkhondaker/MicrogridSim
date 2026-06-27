#pragma once

namespace Microgrid::Models {
    class Battery {
    public:
        Battery(double capacityWh, double maxChargePower, double maxDischargePower, double initialSoc = 0.5);

        // Attempts to command P (positive=charge, negative=discharge)
        // Returns ACTUAL power accepted/delivered based on limits and SOC
        double step(double requestedPower, double dt_seconds);
        [[nodiscard]] double getSoc() const;

    private:
        double capacityJoules;
        double currentEnergyJoules;
        double pMaxCharge;
        double pMaxDischarge;
        double chargeEfficiency = 0.95;
        double dischargeEfficiency = 0.95;
    };
}
