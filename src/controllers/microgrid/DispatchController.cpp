#include "controllers/microgrid/DispatchController.hpp"
#include "utils/Config.hpp"
#include "utils/Logger.hpp"
#include <sstream>

namespace Microgrid::Controllers {

DispatchController::DispatchController(
    std::shared_ptr<Models::PvPanel> pv,
    std::shared_ptr<Models::Battery> bat,
    std::shared_ptr<Models::DcDcConverter> conv,
    std::shared_ptr<MPPT::MpptAlgorithm> mppt)
    : pvSystem(pv), battery(bat), pvConverter(conv), mpptController(mppt) {
    // Initial guess for PV voltage
    pvVoltageBus = 30.0; 
}

double DispatchController::step(double time, double irradiance, double temp, double loadPower) {
    // 1. Environmental update
    pvSystem->updateEnvironment(irradiance, temp);

    // 2. Hardware Measurements (Sensor equivalents)
    double pvCurrent = pvSystem->getCurrent(pvVoltageBus);
    double pvPowerRaw = pvVoltageBus * pvCurrent;

    // 3. MPPT Algorithm execution
    double dutyCycle = mpptController->calculateDutyCycle(pvVoltageBus, pvCurrent);
    
    // 4. Power Electronics (DC-DC mapping)
    // The MPPT changes Duty Cycle, which mechanically alters the PV bus voltage based on fixed DC Bus
    pvVoltageBus = pvConverter->calculateInputVoltage(Config::DC_BUS_VOLTAGE, dutyCycle);
    
    // Apply efficiency
    double convEff = pvConverter->calculateEfficiency(pvPowerRaw);
    double pvPowerInjected = pvPowerRaw * convEff;

    // 5. Microgrid Energy Balance Logic
    double netPower = pvPowerInjected - loadPower;
    
    // If Net > 0, we have excess, charge battery. If Net < 0, we have deficit, discharge battery.
    double batteryActualP = battery->step(netPower, Config::TIME_STEP_S);
    
    // Any remaining imbalance goes to/from the main Grid (or represents dumped/blackout power)
    double gridPower = netPower - batteryActualP;

    // 6. Logging
    std::stringstream csvRow;
    csvRow << time << "," << irradiance << "," << temp << "," 
           << pvVoltageBus << "," << pvCurrent << "," << pvPowerInjected << ","
           << dutyCycle << "," << batteryActualP << "," << battery->getSoc() << ","
           << loadPower << "," << gridPower;
           
    Utils::Logger::getInstance().logCsv(csvRow.str());

    return gridPower;
}

}
