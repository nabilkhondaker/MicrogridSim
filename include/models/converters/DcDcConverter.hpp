#pragma once

namespace Microgrid::Models {
    class DcDcConverter {
    public:
        enum class Topology { BUCK, BOOST, BUCK_BOOST };

        DcDcConverter(Topology type, double efficiencyBase);

        // Applies duty cycle D (0 to 1), maps input V to output V
        [[nodiscard]] double calculateInputVoltage(double vOutBus, double dutyCycle) const;
        [[nodiscard]] double calculateEfficiency(double inputPower) const;

    private:
        Topology topology;
        double baseEff;
    };
}
