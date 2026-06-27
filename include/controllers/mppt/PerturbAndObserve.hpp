#pragma once
#include "MpptAlgorithm.hpp"

namespace Microgrid::Controllers::MPPT {
    class PerturbAndObserve : public MpptAlgorithm {
    public:
        PerturbAndObserve(double initialDuty, double step);
        double calculateDutyCycle(double voltage, double current) override;

    private:
        double prevVoltage = 0.0;
        double prevPower = 0.0;
    };
}
