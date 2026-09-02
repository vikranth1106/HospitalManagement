// ============================================================
// Modules.h
// Composition classes used inside every MedicalEquipment object.
//
// Three member classes:
//   BatteryModule      – tracks charge level and low-battery state
//   CalibrationModule  – tracks calibration date and validity window
//   MaintenanceModule  – tracks last service date and due intervals
//
// These classes demonstrate the OOP principle of Composition:
// "Has-A" relationship rather than "Is-A" (inheritance).
// ============================================================

#pragma once

#include <string>
#include <iostream>

// ============================================================
// BatteryModule
// Represents the battery subsystem of a medical device.
// ============================================================
class BatteryModule {
private:
    double  chargeLevel;       // Current charge percentage (0-100)
    double  lowBatteryThreshold; // Alert threshold (default 20%)
    bool    isCharging;        // Whether the device is on charge
    std::string batteryType;   // e.g., "Li-Ion", "NiMH"

public:
    // Default constructor – sets a fully charged new battery
    BatteryModule();

    // Parameterised constructor
    BatteryModule(double charge, double threshold,
                  bool charging, const std::string& type);

    // Copy constructor (shallow copy is safe – all POD / std::string)
    BatteryModule(const BatteryModule& other);

    // Copy-assignment operator
    BatteryModule& operator=(const BatteryModule& other);

    // ----- Accessors -----
    double      getChargeLevel()        const;
    double      getLowBatteryThreshold()const;
    bool        getIsCharging()         const;
    std::string getBatteryType()        const;
    bool        isLow()                 const; // true if charge <= threshold

    // ----- Mutators (chainable via reference-return for this-pointer demo) -----
    BatteryModule& setChargeLevel(double level);
    BatteryModule& setIsCharging(bool charging);
    BatteryModule& setBatteryType(const std::string& type);

    // Discharge the battery by a given amount (simulates usage)
    BatteryModule& discharge(double amount);

    // Display battery status
    void display() const;
};


// ============================================================
// CalibrationModule
// Tracks device calibration status and validity.
// ============================================================
class CalibrationModule {
private:
    std::string lastCalibrationDate;   // "YYYY-MM-DD"
    int         validityPeriodDays;    // How many days calibration is valid
    bool        isCalibrated;          // Current calibration flag
    std::string calibratedBy;          // Technician name

public:
    // Default constructor – uncalibrated state
    CalibrationModule();

    // Parameterised constructor
    CalibrationModule(const std::string& date, int validDays,
                      bool calibrated, const std::string& tech);

    // Copy constructor
    CalibrationModule(const CalibrationModule& other);

    // Copy-assignment operator
    CalibrationModule& operator=(const CalibrationModule& other);

    // ----- Accessors -----
    std::string getLastCalibrationDate() const;
    int         getValidityPeriodDays()  const;
    bool        getIsCalibrated()        const;
    std::string getCalibratedBy()        const;

    // ----- Mutators -----
    CalibrationModule& setLastCalibrationDate(const std::string& date);
    CalibrationModule& setIsCalibrated(bool status);
    CalibrationModule& setCalibratedBy(const std::string& tech);
    CalibrationModule& setValidityPeriodDays(int days);

    // Perform a calibration update
    CalibrationModule& calibrate(const std::string& date, const std::string& tech);

    // Display calibration status
    void display() const;
};


// ============================================================
// MaintenanceModule
// Tracks service history and upcoming maintenance schedule.
// ============================================================
class MaintenanceModule {
private:
    std::string lastServiceDate;    // "YYYY-MM-DD"
    int         serviceIntervalDays;// How often service is required
    int         serviceCount;       // Total number of services performed
    bool        maintenanceDue;     // Flag: maintenance overdue?
    std::string assignedTechnician; // Responsible technician

public:
    // Default constructor
    MaintenanceModule();

    // Parameterised constructor
    MaintenanceModule(const std::string& date, int interval,
                      int count, bool due, const std::string& tech);

    // Copy constructor
    MaintenanceModule(const MaintenanceModule& other);

    // Copy-assignment operator
    MaintenanceModule& operator=(const MaintenanceModule& other);

    // ----- Accessors -----
    std::string getLastServiceDate()     const;
    int         getServiceIntervalDays() const;
    int         getServiceCount()        const;
    bool        isMaintenanceDue()       const;
    std::string getAssignedTechnician()  const;

    // ----- Mutators -----
    MaintenanceModule& setLastServiceDate(const std::string& date);
    MaintenanceModule& setMaintenanceDue(bool due);
    MaintenanceModule& setAssignedTechnician(const std::string& tech);

    // Record a new service event
    MaintenanceModule& recordService(const std::string& date, const std::string& tech);

    // Display maintenance status
    void display() const;
};
