#pragma once
#include "models/pv/PvPanel.hpp"
#include "models/storage/Battery.hpp"
#include "models/converters/DcDcConverter.hpp"
#include "controllers/mppt/MpptAlgorithm.hpp"
#include <memory>

namespace Microgrid::Controllers {
    class DispatchController {
    public:
        DispatchController(
            std::shared_ptr<Models::PvPanel> pv,
            std::shared_ptr<Models::Battery> bat,
            std::shared_ptr<Models::DcDcConverter> conv,
            std::shared_ptr<MPPT::MpptAlgorithm> mppt);

        // Runs a single timestep control loop. Returns the grid interaction power (excess/deficit)
        double step(double time, double irradiance, double temp, double loadPower);

    private:
        std::shared_ptr<Models::PvPanel> pvSystem;
        std::shared_ptr<Models::Battery> battery;
        std::shared_ptr<Models::DcDcConverter> pvConverter;
        std::shared_ptr<MPPT::MpptAlgorithm> mpptController;

        double pvVoltageBus = 0.0; // Input side of the converter
        double prevDuty = 0.5;
    };
}
