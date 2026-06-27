#include "utils/Logger.hpp"
#include <chrono>
#include <iomanip>

namespace Microgrid::Utils {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setOutputFile(const std::string& filename) {
    fileStream.open(filename + ".log", std::ios::out | std::ios::app);
    csvStream.open(filename + ".csv", std::ios::out);
    csvStream << "Time,Irradiance,Temp,PV_V,PV_I,PV_P,MPPT_Duty,Bat_P,Bat_SOC,Load_P,Net_Grid\n";
}

void Logger::log(Level level, const std::string& component, const std::string& message) {
    std::string lvlStr;
    switch(level) {
        case Level::INFO: lvlStr = "[INFO]"; break;
        case Level::WARNING: lvlStr = "[WARN]"; break;
        case Level::ERROR: lvlStr = "[ERR ]"; break;
        case Level::DEBUG: lvlStr = "[DBUG]"; break;
    }
    std::cout << lvlStr << " [" << component << "] " << message << "\n";
    if (fileStream.is_open()) {
        fileStream << lvlStr << " [" << component << "] " << message << "\n";
    }
}

void Logger::logCsv(const std::string& csvData) {
    if (csvStream.is_open()) {
        csvStream << csvData << "\n";
    }
}

Logger::~Logger() {
    if (fileStream.is_open()) fileStream.close();
    if (csvStream.is_open()) csvStream.close();
}

}
