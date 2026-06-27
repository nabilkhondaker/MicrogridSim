#pragma once

namespace Microgrid::Controllers::MPPT {
    class MpptAlgorithm {
    public:
        virtual ~MpptAlgorithm() = default;
        
        // Feed current V and I, returns new calculated Duty Cycle
        virtual double calculateDutyCycle(double voltage, double current) = 0;
        
    protected:
        double currentDuty = 0.5;
        double stepSize = 0.01;
    };
}
