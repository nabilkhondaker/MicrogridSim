#pragma once
#include "MpptAlgorithm.hpp"

namespace Microgrid::Controllers::MPPT {
    class IncrementalConductance : public MpptAlgorithm {
    public:
        IncrementalConductance(double initialDuty, double step);
        double calculateDutyCycle(double voltage, double current) override;

    private:
        double prevVoltage = 0.0;
        double prevCurrent = 0.0;
    };
}
