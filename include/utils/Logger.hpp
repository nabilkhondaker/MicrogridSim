#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace Microgrid::Utils {
    class Logger {
    public:
        enum class Level { INFO, WARNING, ERROR, DEBUG };
        
        static Logger& getInstance();
        void setOutputFile(const std::string& filename);
        void log(Level level, const std::string& component, const std::string& message);
        void logCsv(const std::string& csvData);

    private:
        Logger() = default;
        ~Logger();
        std::ofstream fileStream;
        std::ofstream csvStream;
    };
}
