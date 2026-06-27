#include "models/pv/PvPanel.hpp"
#include "models/storage/Battery.hpp"
#include "models/converters/DcDcConverter.hpp"
#include "controllers/mppt/PerturbAndObserve.hpp"
#include "controllers/mppt/IncrementalConductance.hpp"
#include "controllers/microgrid/DispatchController.hpp"
#include "utils/Logger.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

using namespace Microgrid;

struct ProfileData {
    double time, irradiance, temp, load;
};

std::vector<ProfileData> loadProfile(const std::string& filename) {
    std::vector<ProfileData> data;
    std::ifstream file(filename);
    std::string line;
    
    if(!file.is_open()) {
        Utils::Logger::getInstance().log(Utils::Logger::Level::ERROR, "System", "Could not open profile data.");
        // Fallback dummy data if file is missing
        data.push_back({0, 1000, 25, 1000}); 
        data.push_back({3600, 500, 20, 1500});
        return data;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string val;
        ProfileData pd;
        if(std::getline(ss, val, ',')) pd.time = std::stod(val);
        if(std::getline(ss, val, ',')) pd.irradiance = std::stod(val);
        if(std::getline(ss, val, ',')) pd.temp = std::stod(val);
        if(std::getline(ss, val, ',')) pd.load = std::stod(val);
        data.push_back(pd);
    }
    return data;
}

int main() {
    Utils::Logger::getInstance().setOutputFile("microgrid_sim_results");
    Utils::Logger::getInstance().log(Utils::Logger::Level::INFO, "Main", "Initializing Microgrid Simulator...");

    // 1. Configure Plant Models
    Models::PvPanel::Specs pvSpecs = {
        37.5,   // Voc
        8.5,    // Isc
        250.0,  // Pmax
        -0.12,  // tempCoeff_V
        0.004,  // tempCoeff_I
        1.2     // ideality
    };
    
    auto pvArray = std::make_shared<Models::PvPanel>(pvSpecs);
    
    // 5kWh battery, 2kW charge/discharge limits, starting at 50% SOC
    auto batteryStorage = std::make_shared<Models::Battery>(5000.0, 2000.0, 2000.0, 0.5);
    
    // Boost converter to step up PV to 400V DC Bus, 98% base efficiency
    auto dcDcConv = std::make_shared<Models::DcDcConverter>(Models::DcDcConverter::Topology::BOOST, 0.98);

    // 2. Configure Controllers (Swap P&O for IncCond here if desired)
    auto mpptAlgo = std::make_shared<Controllers::MPPT::PerturbAndObserve>(0.5, 0.02);
    // auto mpptAlgo = std::make_shared<Controllers::MPPT::IncrementalConductance>(0.5, 0.02);

    Controllers::DispatchController mgController(pvArray, batteryStorage, dcDcConv, mpptAlgo);

    // 3. Load Environment & Load Profile
    auto profile = loadProfile("data/daily_profile.csv");

    Utils::Logger::getInstance().log(Utils::Logger::Level::INFO, "Main", "Starting Simulation Loop...");

    // 4. Simulation Execution Loop
    for (const auto& step : profile) {
        // In a real sim, you would interpolate between these profile points based on Config::TIME_STEP_S
        // For demonstration, we just run the discrete profile points
        
        double gridExchange = mgController.step(step.time, step.irradiance, step.temp, step.load);
        
        std::cout << "Time: " << step.time << "s | "
                  << "Irradiance: " << step.irradiance << " W/m2 | "
                  << "Load: " << step.load << " W | "
                  << "Battery SOC: " << batteryStorage->getSoc() * 100.0 << "% | "
                  << "Grid Int: " << gridExchange << " W\n";
    }

    Utils::Logger::getInstance().log(Utils::Logger::Level::INFO, "Main", "Simulation Complete. Results saved to microgrid_sim_results.csv");
    
    return 0;
}
