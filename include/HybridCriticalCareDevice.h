// ============================================================
// HybridCriticalCareDevice.h
//
// Demonstrates MULTIPLE INHERITANCE and the virtual base class
// solution to the diamond problem.
//
// Inheritance chain:
//
//   ResourceIdentity (virtual base)
//         |          |
//   InfusionPump  Ventilator
//         \          /
//    HybridCriticalCareDevice
//
// Because both InfusionPump and Ventilator inherit VIRTUALLY
// from ResourceIdentity, HybridCriticalCareDevice contains
// exactly ONE copy of resourceID / resourceName / manufacturer.
//
// Inheritance role:
//   - MULTIPLE (Ventilator + InfusionPump)
//   - MULTILEVEL (ResourceIdentity -> HospitalResource ->
//                 Ventilator -> HybridCriticalCareDevice)
// ============================================================

#pragma once

#include "Ventilator.h"
#include "InfusionPump.h"
#include <string>

// ============================================================
// HybridCriticalCareDevice
// An advanced ICU device that combines mechanical ventilation
// with integrated drug infusion capabilities.
// ============================================================
class HybridCriticalCareDevice : public Ventilator,
                                  public InfusionPump {
private:
    std::string deviceProfile;     // Clinical profile (e.g., "ARDS Protocol")
    bool        synchronisedMode;  // True: infusion synchronised with breath cycle
    int         icuBedNumber;      // Which ICU bed this device is assigned to
    double      totalCombinedCost; // Aggregate cost for ventilation + infusion

public:
    // Constructor – must explicitly initialise the virtual base
    HybridCriticalCareDevice(const std::string& id,
                              const std::string& name,
                              const std::string& mfr,
                              const std::string& ward,
                              double dailyCost);

    // Virtual destructor
    virtual ~HybridCriticalCareDevice() = default;

    // ---- Override pure virtuals – resolves ambiguity from both bases ----
    void        operate()           override;
    void        runDiagnostics()    override;
    std::string getEquipmentType()  const override;
    void        generateReport()    const override;

    // ---- HybridCriticalCareDevice-specific functions ----

    // Activate synchronised breath-triggered infusion bolus
    void activateSynchronisedMode();
    void deactivateSynchronisedMode();

    // Run combined protocol (ventilation + infusion together)
    void runCombinedProtocol();

    // Assign to an ICU bed
    void assignToICUBed(int bedNumber);

    // Load a predefined clinical profile
    void loadClinicalProfile(const std::string& profile);

    // ---- Accessors ----
    std::string getDeviceProfile()     const;
    bool        getSynchronisedMode()  const;
    int         getIcuBedNumber()      const;
    double      getTotalCombinedCost() const;

    // Override display to show both subsystem states
    void display() const override;
};
