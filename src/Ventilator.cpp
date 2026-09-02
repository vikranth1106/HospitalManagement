// ============================================================
// Ventilator.cpp
// Implementation of the Ventilator class.
// ============================================================

#include "Ventilator.h"
#include <iostream>
#include <iomanip>

// ============================================================
// Helper: convert VentilationMode to string
// ============================================================
std::string ventModeToString(VentilationMode mode) {
    switch (mode) {
        case VentilationMode::VOLUME_CONTROL:   return "Volume Control (VC)";
        case VentilationMode::PRESSURE_CONTROL: return "Pressure Control (PC)";
        case VentilationMode::SIMV:             return "SIMV";
        case VentilationMode::CPAP:             return "CPAP";
        case VentilationMode::BIPAP:            return "BiPAP";
        default:                                return "Unknown";
    }
}

// ============================================================
// Constructors
// ============================================================

Ventilator::Ventilator()
    : ResourceIdentity("VT-000", "Ventilator", "RespiroMed"),
      HospitalResource(),
      ventMode(VentilationMode::VOLUME_CONTROL),
      tidalVolumeMl(500.0),
      respiratoryRate(14),
      fiO2Percent(40.0),
      peepCmH2O(5.0),
      peakPressureCmH2O(20.0),
      isVentilating(false),
      highPressureAlarm(false),
      minuteVolumeL(0.0) {
    recalculateMinuteVolume();
}

Ventilator::Ventilator(const std::string& id,
                       const std::string& name,
                       const std::string& mfr,
                       const std::string& w,
                       double dailyCost)
    : ResourceIdentity(id, name, mfr),
      HospitalResource(id, name, mfr, w, dailyCost),
      ventMode(VentilationMode::VOLUME_CONTROL),
      tidalVolumeMl(500.0),
      respiratoryRate(14),
      fiO2Percent(40.0),
      peepCmH2O(5.0),
      peakPressureCmH2O(20.0),
      isVentilating(false),
      highPressureAlarm(false),
      minuteVolumeL(0.0) {
    recalculateMinuteVolume();
}

// ============================================================
// Protected helper
// ============================================================
void Ventilator::recalculateMinuteVolume() {
    minuteVolumeL = (tidalVolumeMl * respiratoryRate) / 1000.0;
}

// ============================================================
// Pure virtual overrides
// ============================================================

void Ventilator::operate() {
    if (!isVentilating) {
        std::cout << "  [Ventilator] " << resourceName
                  << " – not ventilating. Call startVentilation() first.\n";
        return;
    }
    if (highPressureAlarm) {
        std::cout << "  [Ventilator] HIGH PRESSURE ALARM active! Acknowledge first.\n";
        return;
    }
    std::cout << "  [Ventilator] " << resourceName << " delivering breath:\n"
              << "    Mode : " << ventModeToString(ventMode) << "\n"
              << "    Vt   : " << tidalVolumeMl << " mL | RR: "
              << respiratoryRate << " /min | FiO2: "
              << fiO2Percent << "% | PEEP: " << peepCmH2O << " cmH2O\n"
              << "    MV   : " << std::fixed << std::setprecision(2)
              << minuteVolumeL << " L/min\n";
    addUsageHours(1);
    getBatteryModuleRef().discharge(1.0);
}

void Ventilator::runDiagnostics() {
    std::cout << "  [Ventilator] Running diagnostics on " << resourceName << "...\n"
              << "    Mode       : " << ventModeToString(ventMode) << "\n"
              << "    Peak Press : " << peakPressureCmH2O << " cmH2O\n"
              << "    PEEP       : " << peepCmH2O << " cmH2O\n"
              << "    FiO2       : " << fiO2Percent << "%\n"
              << "    Alarm      : " << (highPressureAlarm ? "HIGH PRESSURE" : "None") << "\n"
              << "    Battery    : " << battery.getChargeLevel() << "%\n";
    bool ok = !highPressureAlarm && !battery.isLow() &&
               calibration.getIsCalibrated();
    std::cout << "  Result: " << (ok ? "PASS" : "FAIL") << "\n";
}

std::string Ventilator::getEquipmentType() const {
    return "Ventilator";
}

void Ventilator::generateReport() const {
    std::cout << "\n  === Ventilator Report: " << resourceName << " ===\n";
    displayIdentity();
    std::cout << "  Ward         : " << ward << "\n"
              << "  Status       : " << operatingStatus << "\n"
              << "  Available    : " << (isAvailable ? "Yes" : "No") << "\n"
              << "  Ventilating  : " << (isVentilating ? "Yes" : "No") << "\n"
              << "  Mode         : " << ventModeToString(ventMode) << "\n"
              << "  Tidal Vol    : " << tidalVolumeMl << " mL\n"
              << "  Rate         : " << respiratoryRate << " /min\n"
              << "  FiO2         : " << fiO2Percent << "%\n"
              << "  PEEP         : " << peepCmH2O << " cmH2O\n"
              << "  Peak Press   : " << peakPressureCmH2O << " cmH2O\n"
              << "  Minute Vol   : " << minuteVolumeL << " L/min\n"
              << "  Press Alarm  : " << (highPressureAlarm ? "ACTIVE" : "None") << "\n";
    battery.display();
    calibration.display();
    maintenance.display();
}

// ============================================================
// Ventilator-specific functions
// ============================================================

void Ventilator::startVentilation() {
    isVentilating = true;
    std::cout << "  [Ventilator] " << resourceName
              << " ventilation STARTED in " << ventModeToString(ventMode) << " mode.\n";
}

void Ventilator::stopVentilation() {
    isVentilating = false;
    std::cout << "  [Ventilator] " << resourceName
              << " ventilation STOPPED safely.\n";
}

void Ventilator::setVentilationParameters(VentilationMode mode,
                                          double tidalVol,
                                          int rate,
                                          double fio2,
                                          double peep) {
    ventMode          = mode;
    tidalVolumeMl     = tidalVol;
    respiratoryRate   = rate;
    fiO2Percent       = fio2;
    peepCmH2O        = peep;
    recalculateMinuteVolume();
    std::cout << "  [Ventilator] Parameters updated on " << resourceName << ".\n";
}

void Ventilator::acknowledgeAlarm() {
    highPressureAlarm = false;
    std::cout << "  [Ventilator] High-pressure alarm acknowledged on "
              << resourceName << ".\n";
}

void Ventilator::runSpontaneousBreathingTrial() {
    std::cout << "  [Ventilator] Running Spontaneous Breathing Trial (SBT) on "
              << resourceName << "...\n"
              << "  Reducing support: FiO2->21%, PEEP->5 cmH2O, CPAP mode.\n"
              << "  Monitor patient for 30 minutes before extubation decision.\n";
}

// ============================================================
// Accessors
// ============================================================
VentilationMode Ventilator::getVentMode()           const { return ventMode; }
double          Ventilator::getTidalVolumeMl()      const { return tidalVolumeMl; }
int             Ventilator::getRespiratoryRate()    const { return respiratoryRate; }
double          Ventilator::getFiO2Percent()        const { return fiO2Percent; }
double          Ventilator::getPeepCmH2O()         const { return peepCmH2O; }
double          Ventilator::getPeakPressureCmH2O() const { return peakPressureCmH2O; }
bool            Ventilator::getIsVentilating()      const { return isVentilating; }
bool            Ventilator::getHighPressureAlarm()  const { return highPressureAlarm; }
double          Ventilator::getMinuteVolumeL()     const { return minuteVolumeL; }

// Display override
void Ventilator::display() const {
    HospitalResource::display();
    std::cout << "  -- Ventilator Settings --\n"
              << "  Mode   : " << ventModeToString(ventMode) << "\n"
              << "  Vt     : " << tidalVolumeMl << " mL | RR: "
              << respiratoryRate << " /min\n"
              << "  FiO2   : " << fiO2Percent << "% | PEEP: "
              << peepCmH2O << " cmH2O\n"
              << "  MV     : " << minuteVolumeL << " L/min\n"
              << "  Active : " << (isVentilating ? "Yes" : "No") << "\n";
}
