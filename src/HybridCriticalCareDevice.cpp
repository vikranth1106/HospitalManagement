// ============================================================
// HybridCriticalCareDevice.cpp
// Implementation of the HybridCriticalCareDevice class.
//
// The virtual base ResourceIdentity is initialised explicitly
// in this most-derived class constructor, which is required
// by C++ when using virtual inheritance.
// ============================================================

#include "HybridCriticalCareDevice.h"
#include <iostream>
#include <iomanip>

// ============================================================
// Constructor
// Must call virtual base ResourceIdentity constructor directly.
// ============================================================
HybridCriticalCareDevice::HybridCriticalCareDevice(
        const std::string& id,
        const std::string& name,
        const std::string& mfr,
        const std::string& ward,
        double dailyCost)
    // Virtual bases are initialised first, in declaration order
    : ResourceIdentity(id, name, mfr),
      HospitalResource(id, name, mfr, ward, dailyCost),
      // Then intermediate concrete bases
      Ventilator(id, name, mfr, ward, dailyCost),
      InfusionPump(id, name, mfr, ward, dailyCost),
      // Then our own fields
      deviceProfile("Standard ICU Protocol"),
      synchronisedMode(false),
      icuBedNumber(0),
      totalCombinedCost(dailyCost * 2.0)
{}

// ============================================================
// Pure virtual overrides (resolves ambiguity between both bases)
// ============================================================

void HybridCriticalCareDevice::operate() {
    std::cout << "  [HybridDevice] " << resourceName
              << " running COMBINED operation:\n";
    // Call ventilation operation
    Ventilator::operate();
    // If synchronised mode: also dispense infusion
    if (synchronisedMode) {
        std::cout << "  [HybridDevice] Synchronised infusion triggered.\n";
        InfusionPump::operate();
    }
    totalCombinedCost += (dailyCostUSD / 24.0); // hourly cost
}

void HybridCriticalCareDevice::runDiagnostics() {
    std::cout << "  [HybridDevice] === Combined Diagnostics: "
              << resourceName << " ===\n";
    std::cout << "  -- Ventilator subsystem --\n";
    Ventilator::runDiagnostics();
    std::cout << "  -- Infusion subsystem --\n";
    InfusionPump::runDiagnostics();
    std::cout << "  -- Hybrid-specific --\n"
              << "    Synchronised Mode : " << (synchronisedMode ? "On" : "Off") << "\n"
              << "    ICU Bed           : " << icuBedNumber << "\n"
              << "    Clinical Profile  : " << deviceProfile << "\n";
}

std::string HybridCriticalCareDevice::getEquipmentType() const {
    return "Hybrid Critical Care Device";
}

void HybridCriticalCareDevice::generateReport() const {
    std::cout << "\n  === Hybrid Critical Care Device Report: "
              << resourceName << " ===\n";
    displayIdentity();
    std::cout << "  Ward             : " << ward << "\n"
              << "  ICU Bed          : " << icuBedNumber << "\n"
              << "  Device Profile   : " << deviceProfile << "\n"
              << "  Synchronised     : " << (synchronisedMode ? "Yes" : "No") << "\n"
              << "  Combined Cost    : $" << std::fixed << std::setprecision(2)
              << totalCombinedCost << "\n"
              << "  Ventilation Mode : " << ventModeToString(getVentMode()) << "\n"
              << "  Tidal Volume     : " << getTidalVolumeMl() << " mL\n"
              << "  Respiratory Rate : " << getRespiratoryRate() << " /min\n"
              << "  FiO2             : " << getFiO2Percent() << "%\n"
              << "  Medication       : " << getMedicationName() << "\n"
              << "  Flow Rate        : " << getFlowRateMlPerHour() << " mL/hr\n";
    battery.display();
    calibration.display();
    maintenance.display();
}

// ============================================================
// HybridCriticalCareDevice-specific functions
// ============================================================

void HybridCriticalCareDevice::activateSynchronisedMode() {
    synchronisedMode = true;
    std::cout << "  [HybridDevice] Synchronised mode ACTIVATED on "
              << resourceName << ". Infusion will trigger with each breath.\n";
}

void HybridCriticalCareDevice::deactivateSynchronisedMode() {
    synchronisedMode = false;
    std::cout << "  [HybridDevice] Synchronised mode DEACTIVATED on "
              << resourceName << ".\n";
}

void HybridCriticalCareDevice::runCombinedProtocol() {
    std::cout << "  [HybridDevice] Running Combined Protocol: " << deviceProfile << "\n";
    startVentilation();  // Ventilator method
    startInfusion();     // InfusionPump method
    if (synchronisedMode) activateSynchronisedMode();
    std::cout << "  [HybridDevice] Combined protocol running.\n";
}

void HybridCriticalCareDevice::assignToICUBed(int bedNumber) {
    icuBedNumber = bedNumber;
    std::cout << "  [HybridDevice] " << resourceName
              << " assigned to ICU Bed #" << bedNumber << ".\n";
}

void HybridCriticalCareDevice::loadClinicalProfile(const std::string& profile) {
    deviceProfile = profile;
    std::cout << "  [HybridDevice] Clinical profile loaded: " << profile << "\n";
}

// ============================================================
// Accessors
// ============================================================
std::string HybridCriticalCareDevice::getDeviceProfile()     const { return deviceProfile; }
bool        HybridCriticalCareDevice::getSynchronisedMode()  const { return synchronisedMode; }
int         HybridCriticalCareDevice::getIcuBedNumber()      const { return icuBedNumber; }
double      HybridCriticalCareDevice::getTotalCombinedCost() const { return totalCombinedCost; }

// Display override
void HybridCriticalCareDevice::display() const {
    HospitalResource::display(); // Use HospitalResource's display (shared base)
    std::cout << "  -- Hybrid Device Details --\n"
              << "  Profile   : " << deviceProfile << "\n"
              << "  ICU Bed   : " << icuBedNumber << "\n"
              << "  SyncMode  : " << (synchronisedMode ? "On" : "Off") << "\n"
              << "  Combined Cost: $" << std::fixed << std::setprecision(2)
              << totalCombinedCost << "\n";
}
