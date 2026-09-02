// ============================================================
// MedicalEquipment.h
//
// Standalone equipment class demonstrating:
//   - Default, parameterised, overloaded, and copy constructors
//   - Deep copy (dynamic memory for service history array)
//   - Destructor with proper memory release
//   - operator+  (combine usage durations / cost)
//   - operator<  (compare allocation suitability)
//   - operator<< (formatted report output)
//   - Chainable update functions (this-pointer demo)
//   - Composition of BatteryModule, CalibrationModule,
//     MaintenanceModule
//
// Note: MedicalEquipment is a concrete standalone class used
// for demonstrating deep-copy and operator overloading.
// The polymorphic hierarchy uses HospitalResource* pointers.
// ============================================================

#pragma once

#include "Modules.h"
#include <string>
#include <vector>
#include <iostream>

// ============================================================
// MedicalEquipment
// Represents a general medical device with deep-copy semantics
// and overloaded operators.
// ============================================================
class MedicalEquipment {
private:
    // ----- Core identity fields -----
    std::string equipmentID;
    std::string equipmentName;
    std::string equipmentType;    // e.g., "Monitor", "Pump", "Ventilator"
    std::string ward;
    std::string manufacturer;

    // ----- Operational fields -----
    std::string operatingStatus;  // "Active", "Inactive", "Under Maintenance"
    bool        isAvailable;
    double      dailyCostUSD;
    int         usageDurationHours;

    // ----- Composition modules -----
    BatteryModule     battery;
    CalibrationModule calibration;
    MaintenanceModule maintenance;

    // ----- Dynamic memory (deep copy demo) -----
    // serviceHistory is heap-allocated to demonstrate deep copy.
    // Each entry is a string description of a service event.
    std::string* serviceHistory;  // Raw pointer – managed manually
    int          serviceHistoryCapacity;
    int          serviceHistorySize;

    // ----- Private helpers -----
    void allocateHistory(int capacity);
    void copyHistory(const MedicalEquipment& other);
    void releaseHistory();

public:
    // ---- Constructors ----

    // (1) Default constructor – minimal equipment object
    MedicalEquipment();

    // (2) Parameterised constructor – core fields
    MedicalEquipment(const std::string& id,
                     const std::string& name,
                     const std::string& type,
                     const std::string& ward,
                     double dailyCost);

    // (3) Overloaded constructor – full detail including modules
    MedicalEquipment(const std::string& id,
                     const std::string& name,
                     const std::string& type,
                     const std::string& ward,
                     double dailyCost,
                     const BatteryModule& bat,
                     const CalibrationModule& cal,
                     const MaintenanceModule& mnt);

    // (4) Copy constructor – performs DEEP COPY of serviceHistory
    MedicalEquipment(const MedicalEquipment& other);

    // Copy-assignment operator – also performs deep copy
    MedicalEquipment& operator=(const MedicalEquipment& other);

    // Destructor – releases heap-allocated serviceHistory
    ~MedicalEquipment();

    // ---- Operator Overloading (Requirement 2) ----

    // operator+ : Returns a new object whose usageDurationHours and
    //             dailyCostUSD are the sum of both operands.
    //             Useful for aggregating equipment statistics.
    MedicalEquipment operator+(const MedicalEquipment& other) const;

    // operator< : Returns true if THIS equipment is MORE SUITABLE for
    //             allocation than 'other' (higher battery, calibrated,
    //             available, active).
    bool operator<(const MedicalEquipment& other) const;

    // operator== : Equality by equipment ID
    bool operator==(const MedicalEquipment& other) const;

    // operator<< : Prints a formatted equipment report to an ostream
    friend std::ostream& operator<<(std::ostream& os,
                                    const MedicalEquipment& eq);

    // ---- Chainable update functions (this-pointer demo) ----

    // Update battery charge level and return *this for chaining
    MedicalEquipment& updateBattery(double chargeLevel);

    // Update daily cost and return *this for chaining
    MedicalEquipment& updateCost(double cost);

    // Update ward assignment and return *this for chaining
    MedicalEquipment& updateWard(const std::string& newWard);

    // Update operating status and return *this for chaining
    MedicalEquipment& updateStatus(const std::string& status);

    // ---- Service History ----

    // Add a service record to the dynamic history array
    void addServiceRecord(const std::string& record);

    // Display all service history records
    void displayServiceHistory() const;

    // ---- Accessors ----
    std::string getEquipmentID()        const;
    std::string getEquipmentName()      const;
    std::string getEquipmentType()      const;
    std::string getWard()               const;
    std::string getManufacturer()       const;
    std::string getOperatingStatus()    const;
    bool        getIsAvailable()        const;
    double      getDailyCostUSD()       const;
    int         getUsageDurationHours() const;
    int         getServiceHistorySize() const;

    const BatteryModule&     getBatteryModule()     const;
    const CalibrationModule& getCalibrationModule() const;
    const MaintenanceModule& getMaintenanceModule() const;

    // Non-const references for modifying module state
    BatteryModule&     getBatteryModuleRef();
    CalibrationModule& getCalibrationModuleRef();
    MaintenanceModule& getMaintenanceModuleRef();

    // ---- Mutators ----
    void setEquipmentID(const std::string& id);
    void setEquipmentName(const std::string& name);
    void setWard(const std::string& w);
    void setManufacturer(const std::string& mfr);
    void setIsAvailable(bool avail);
    void setUsageDurationHours(int hours);
    void addUsageHours(int hours);

    // Full display (uses operator<<)
    void display() const;

    // Compute allocation suitability score (0-100)
    int computeSuitabilityScore() const;
};
