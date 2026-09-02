// ============================================================
// InfusionPump.cpp
// Implementation of the InfusionPump class.
// ============================================================

#include "InfusionPump.h"
#include <iostream>
#include <iomanip>

// Default constructor
InfusionPump::InfusionPump()
    : ResourceIdentity("IP-000", "Infusion Pump", "MedFlow"),
      HospitalResource(),
      flowRateMlPerHour(0.0),
      volumeRemainingMl(500.0),
      totalVolumeToInfuseMl(500.0),
      medicationName("Saline"),
      medicationType("Fluid"),
      isRunning(false),
      occlusionAlarm(false),
      channelCount(1) {}

// Parameterised constructor
InfusionPump::InfusionPump(const std::string& id,
                           const std::string& name,
                           const std::string& mfr,
                           const std::string& w,
                           double dailyCost)
    : ResourceIdentity(id, name, mfr),
      HospitalResource(id, name, mfr, w, dailyCost),
      flowRateMlPerHour(50.0),
      volumeRemainingMl(500.0),
      totalVolumeToInfuseMl(500.0),
      medicationName("Saline 0.9%"),
      medicationType("Fluid"),
      isRunning(false),
      occlusionAlarm(false),
      channelCount(1) {}

// ============================================================
// Pure virtual overrides
// ============================================================

// operate() – perform an infusion cycle step
void InfusionPump::operate() {
    if (!isRunning) {
        std::cout << "  [InfusionPump] " << resourceName
                  << " is not running. Call startInfusion() first.\n";
        return;
    }
    if (volumeRemainingMl <= 0.0) {
        std::cout << "  [InfusionPump] " << resourceName
                  << " – infusion complete (volume exhausted).\n";
        isRunning = false;
        return;
    }
    double delivered = flowRateMlPerHour / 60.0; // per minute
    volumeRemainingMl -= delivered;
    if (volumeRemainingMl < 0.0) volumeRemainingMl = 0.0;
    std::cout << "  [InfusionPump] " << resourceName
              << " delivering " << std::fixed << std::setprecision(2)
              << delivered << " mL. Remaining: "
              << volumeRemainingMl << " mL\n";
    addUsageHours(0); // fractional usage tracked separately
    getBatteryModuleRef().discharge(0.2);
}

void InfusionPump::runDiagnostics() {
    std::cout << "  [InfusionPump] Diagnostics on " << resourceName << "...\n";
    std::cout << "    Flow Rate  : " << flowRateMlPerHour << " mL/hr\n"
              << "    Volume     : " << volumeRemainingMl << " mL remaining\n"
              << "    Occlusion  : " << (occlusionAlarm ? "DETECTED" : "None") << "\n"
              << "    Running    : " << (isRunning ? "Yes" : "No") << "\n"
              << "    Battery    : " << battery.getChargeLevel() << "%\n"
              << "  Result: " << (!occlusionAlarm && !battery.isLow() ? "PASS" : "FAIL") << "\n";
}

std::string InfusionPump::getEquipmentType() const {
    return "Infusion Pump";
}

void InfusionPump::generateReport() const {
    std::cout << "\n  === Infusion Pump Report: " << resourceName << " ===\n";
    displayIdentity();
    std::cout << "  Ward         : " << ward << "\n"
              << "  Status       : " << operatingStatus << "\n"
              << "  Available    : " << (isAvailable ? "Yes" : "No") << "\n"
              << "  Medication   : " << medicationName
              << " (" << medicationType << ")\n"
              << "  Flow Rate    : " << flowRateMlPerHour << " mL/hr\n"
              << "  Volume Left  : " << volumeRemainingMl << " mL\n"
              << "  Running      : " << (isRunning ? "Yes" : "No") << "\n"
              << "  Occlusion    : " << (occlusionAlarm ? "ALARM" : "None") << "\n"
              << "  ETA          : " << getRemainingTimeHours() << " hrs\n";
    battery.display();
    calibration.display();
    maintenance.display();
}

// ============================================================
// InfusionPump-specific functions
// ============================================================

void InfusionPump::setMedication(const std::string& medName,
                                  const std::string& medType,
                                  double totalVolumeMl,
                                  double flowRate) {
    medicationName          = medName;
    medicationType          = medType;
    totalVolumeToInfuseMl   = totalVolumeMl;
    volumeRemainingMl       = totalVolumeMl;
    flowRateMlPerHour       = flowRate;
}

void InfusionPump::startInfusion() {
    if (occlusionAlarm) {
        std::cout << "  [InfusionPump] Cannot start – occlusion alarm active!\n";
        return;
    }
    isRunning = true;
    std::cout << "  [InfusionPump] " << resourceName
              << " infusion STARTED. Delivering " << medicationName
              << " at " << flowRateMlPerHour << " mL/hr\n";
}

void InfusionPump::stopInfusion() {
    isRunning = false;
    std::cout << "  [InfusionPump] " << resourceName << " infusion STOPPED.\n";
}

void InfusionPump::clearOcclusionAlarm() {
    occlusionAlarm = false;
    std::cout << "  [InfusionPump] Occlusion alarm cleared on " << resourceName << ".\n";
}

bool InfusionPump::isOcclusionDetected() const { return occlusionAlarm; }

double InfusionPump::getRemainingTimeHours() const {
    if (flowRateMlPerHour <= 0.0) return -1.0; // undefined
    return volumeRemainingMl / flowRateMlPerHour;
}

// ============================================================
// Accessors
// ============================================================
double      InfusionPump::getFlowRateMlPerHour()     const { return flowRateMlPerHour; }
double      InfusionPump::getVolumeRemainingMl()     const { return volumeRemainingMl; }
double      InfusionPump::getTotalVolumeToInfuseMl() const { return totalVolumeToInfuseMl; }
std::string InfusionPump::getMedicationName()        const { return medicationName; }
std::string InfusionPump::getMedicationType()        const { return medicationType; }
bool        InfusionPump::getIsRunning()             const { return isRunning; }
int         InfusionPump::getChannelCount()          const { return channelCount; }

// Display override
void InfusionPump::display() const {
    HospitalResource::display();
    std::cout << "  -- Infusion Details --\n"
              << "  Medication : " << medicationName << " (" << medicationType << ")\n"
              << "  Flow Rate  : " << flowRateMlPerHour << " mL/hr\n"
              << "  Volume     : " << volumeRemainingMl << " / "
              << totalVolumeToInfuseMl << " mL\n"
              << "  Running    : " << (isRunning ? "Yes" : "No") << "\n"
              << "  Occlusion  : " << (occlusionAlarm ? "ALARM" : "None") << "\n";
}
