// ============================================================
// PatientMonitor.cpp
// Implementation of the PatientMonitor derived class.
// ============================================================

#include "PatientMonitor.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// Default constructor
PatientMonitor::PatientMonitor()
    : ResourceIdentity("PM-000", "Patient Monitor", "MedTech"),
      HospitalResource(),
      heartRateBPM(75.0),
      spO2Percent(98.0),
      systolicBP(120.0),
      diastolicBP(80.0),
      temperatureCelsius(37.0),
      alarmEnabled(true),
      refreshRateSeconds(5) {
    compatibleWards = {"ICU", "General", "Emergency", "Cardiology"};
}

// Parameterised constructor
PatientMonitor::PatientMonitor(const std::string& id,
                               const std::string& name,
                               const std::string& mfr,
                               const std::string& w,
                               double dailyCost)
    : ResourceIdentity(id, name, mfr),
      HospitalResource(id, name, mfr, w, dailyCost),
      heartRateBPM(75.0),
      spO2Percent(98.0),
      systolicBP(120.0),
      diastolicBP(80.0),
      temperatureCelsius(37.0),
      alarmEnabled(true),
      refreshRateSeconds(5) {
    compatibleWards = {"ICU", "General", "Emergency", "Cardiology"};
}

// ============================================================
// Pure virtual overrides
// ============================================================

// operate() – start the monitoring cycle
void PatientMonitor::operate() {
    std::cout << "  [PatientMonitor] " << resourceName
              << " (" << resourceID << ") started monitoring.\n"
              << "  HR: " << heartRateBPM << " bpm | SpO2: "
              << spO2Percent << "% | BP: "
              << systolicBP << "/" << diastolicBP << " mmHg | Temp: "
              << temperatureCelsius << " °C\n";
    addUsageHours(1);
    getBatteryModuleRef().discharge(0.5);
}

// runDiagnostics() – check sensor calibration
void PatientMonitor::runDiagnostics() {
    std::cout << "  [PatientMonitor] Running diagnostics on " << resourceName << "...\n";
    bool battOK  = !battery.isLow();
    bool calOK   = calibration.getIsCalibrated();
    bool statOK  = (operatingStatus == "Active");
    std::cout << "    Battery  : " << (battOK  ? "OK" : "LOW") << "\n"
              << "    Calibration: " << (calOK  ? "Valid" : "REQUIRED") << "\n"
              << "    Status   : " << (statOK ? "Active" : operatingStatus) << "\n"
              << "  Result: " << ((battOK && calOK && statOK) ? "PASS" : "FAIL") << "\n";
}

std::string PatientMonitor::getEquipmentType() const {
    return "Patient Monitor";
}

void PatientMonitor::generateReport() const {
    std::cout << "\n  === Patient Monitor Report: " << resourceName << " ===\n";
    displayIdentity();
    std::cout << "  Ward       : " << ward << "\n"
              << "  Status     : " << operatingStatus << "\n"
              << "  Available  : " << (isAvailable ? "Yes" : "No") << "\n"
              << "  HR         : " << heartRateBPM << " bpm\n"
              << "  SpO2       : " << spO2Percent << "%\n"
              << "  BP         : " << systolicBP << "/" << diastolicBP << " mmHg\n"
              << "  Temp       : " << temperatureCelsius << " °C\n"
              << "  Alarm      : " << (alarmEnabled ? "Enabled" : "Disabled") << "\n";
    battery.display();
    calibration.display();
    maintenance.display();
}

// ============================================================
// PatientMonitor-specific functions
// ============================================================

void PatientMonitor::setVitals(double hr, double spo2,
                               double sys, double dia, double temp) {
    heartRateBPM       = hr;
    spO2Percent        = spo2;
    systolicBP         = sys;
    diastolicBP        = dia;
    temperatureCelsius = temp;
}

void PatientMonitor::triggerAlarm(const std::string& reason) {
    if (alarmEnabled) {
        std::cout << "  *** ALARM [" << resourceName << "]: " << reason << " ***\n";
    }
}

void PatientMonitor::setAlarmEnabled(bool enabled) { alarmEnabled = enabled; }

void PatientMonitor::addCompatibleWard(const std::string& w) {
    compatibleWards.push_back(w);
}

bool PatientMonitor::isCompatibleWith(const std::string& w) const {
    return std::find(compatibleWards.begin(), compatibleWards.end(), w)
           != compatibleWards.end();
}

// ============================================================
// Accessors
// ============================================================
double PatientMonitor::getHeartRateBPM()       const { return heartRateBPM; }
double PatientMonitor::getSpO2Percent()        const { return spO2Percent; }
double PatientMonitor::getSystolicBP()         const { return systolicBP; }
double PatientMonitor::getDiastolicBP()        const { return diastolicBP; }
double PatientMonitor::getTemperatureCelsius() const { return temperatureCelsius; }
bool   PatientMonitor::getAlarmEnabled()       const { return alarmEnabled; }

// ============================================================
// Display override
// ============================================================
void PatientMonitor::display() const {
    HospitalResource::display(); // call base display
    std::cout << "  -- Monitor Vitals --\n"
              << "  HR       : " << heartRateBPM << " bpm\n"
              << "  SpO2     : " << spO2Percent << "%\n"
              << "  BP       : " << systolicBP << "/" << diastolicBP << " mmHg\n"
              << "  Temp     : " << temperatureCelsius << " °C\n"
              << "  Alarm    : " << (alarmEnabled ? "On" : "Off") << "\n";
}
