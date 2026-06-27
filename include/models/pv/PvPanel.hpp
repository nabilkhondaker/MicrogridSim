#pragma once

namespace Microgrid::Models {
    class PvPanel {
    public:
        struct Specs {
            double Voc_ref;     // Open circuit voltage at STC
            double Isc_ref;     // Short circuit current at STC
            double Pmax_ref;    // Max power
            double tempCoeff_V; // V/C
            double tempCoeff_I; // A/C
            double idealityFact;// Diode ideality
        };

        explicit PvPanel(Specs specifications);
        
        void updateEnvironment(double irradiance, double temperature);
        
        // Returns the current [A] given the operating voltage [V]
        [[nodiscard]] double getCurrent(double operatingVoltage) const;
        [[nodiscard]] double getPower(double operatingVoltage) const;

    private:
        Specs specs;
        double currentIrradiance;
        double currentTemp;
        
        // Dynamic variables adjusted for temp and irradiance
        double Voc_adj;
        double Isc_adj;
        double Vt; // Thermal voltage
    };
}
