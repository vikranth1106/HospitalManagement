// ============================================================
// HospitalResource.h
//
// Abstract base class for all medical equipment in the hospital.
// Inherits virtually from ResourceIdentity to support the
// diamond inheritance pattern used by HybridCriticalCareDevice.
//
// Inheritance role: ABSTRACT BASE CLASS (virtual child of ResourceIdentity)
// Pure virtual functions force every derived class to provide
// concrete implementations of core equipment operations.
// ============================================================

#pragma once

#include "ResourceIdentity.h"
#include "Modules.h"
#include <string>
#include <vector>
#include <iostream>

// ============================================================
// HospitalResource
// Abstract base class representing a generic hospital resource.
// Contains composition modules and declares pure virtual
// interface functions.
// ============================================================
class HospitalResource : public virtual ResourceIdentity {
protected:
    // ----- Core equipment attributes -----
    std::string ward;           // Ward where equipment is located (e.g., "ICU")
    std::string operatingStatus;// "Active", "Inactive", "Under Maintenance"
    bool        isAvailable;    // True if not currently assigned to a patient
    double      dailyCostUSD;   // Daily operating cost in USD
    int         usageDurationHours; // Total hours the device has been used

    // ----- Composition member classes (Requirement 6) -----
    BatteryModule     battery;
    CalibrationModule calibration;
    MaintenanceModule maintenance;

public:
    // Default constructor
    HospitalResource();

    // Parameterised constructor
    HospitalResource(const std::string& id,
                     const std::string& name,
                     const std::string& mfr,
                     const std::string& ward,
                     double dailyCost);

    // Virtual destructor – mandatory for polymorphic base classes
    virtual ~HospitalResource() = default;

    // =========================================================
    // Pure Virtual Functions – must be implemented by every
    // concrete derived class (Requirement 3).
    // =========================================================

    // Perform the device's primary clinical operation
    virtual void operate() = 0;

    // Run a self-diagnostic routine
    virtual void runDiagnostics() = 0;

    // Return the equipment category/type string
    virtual std::string getEquipmentType() const = 0;

    // Generate a device-specific status report
    virtual void generateReport() const = 0;

    // ----- Common virtual function (can be overridden) -----
    // Display full equipment details
    virtual void display() const;

    // ----- Accessors -----
    std::string getWard()               const;
    std::string getOperatingStatus()    const;
    bool        getIsAvailable()        const;
    double      getDailyCostUSD()       const;
    int         getUsageDurationHours() const;

    // Module accessors (const reference – avoids copying)
    const BatteryModule&     getBatteryModule()     const;
    const CalibrationModule& getCalibrationModule() const;
    const MaintenanceModule& getMaintenanceModule() const;

    // Non-const references allow external modules to modify state
    BatteryModule&     getBatteryModuleRef();
    CalibrationModule& getCalibrationModuleRef();
    MaintenanceModule& getMaintenanceModuleRef();

    // ----- Mutators -----
    void setWard(const std::string& w);
    void setOperatingStatus(const std::string& status);
    void setIsAvailable(bool avail);
    void setDailyCostUSD(double cost);
    void addUsageHours(int hours);

    // Convenience: mark as under maintenance
    void startMaintenance();
    void completeMaintenance();
};
