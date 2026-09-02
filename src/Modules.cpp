// ============================================================
// Modules.cpp
// Implementation of BatteryModule, CalibrationModule,
// and MaintenanceModule.
// ============================================================

#include "Modules.h"
#include <iostream>
#include <iomanip>

// ============================================================
//  BatteryModule – Implementation
// ============================================================

BatteryModule::BatteryModule()
    : chargeLevel(100.0), lowBatteryThreshold(20.0),
      isCharging(false), batteryType("Li-Ion") {}

BatteryModule::BatteryModule(double charge, double threshold,
                             bool charging, const std::string& type)
    : chargeLevel(charge), lowBatteryThreshold(threshold),
      isCharging(charging), batteryType(type) {}

BatteryModule::BatteryModule(const BatteryModule& other)
    : chargeLevel(other.chargeLevel),
      lowBatteryThreshold(other.lowBatteryThreshold),
      isCharging(other.isCharging),
      batteryType(other.batteryType) {}

BatteryModule& BatteryModule::operator=(const BatteryModule& other) {
    if (this != &other) {
        chargeLevel          = other.chargeLevel;
        lowBatteryThreshold  = other.lowBatteryThreshold;
        isCharging           = other.isCharging;
        batteryType          = other.batteryType;
    }
    return *this;
}

// Accessors
double      BatteryModule::getChargeLevel()        const { return chargeLevel; }
double      BatteryModule::getLowBatteryThreshold()const { return lowBatteryThreshold; }
bool        BatteryModule::getIsCharging()         const { return isCharging; }
std::string BatteryModule::getBatteryType()        const { return batteryType; }
bool        BatteryModule::isLow()                 const { return chargeLevel <= lowBatteryThreshold; }

// Mutators – return *this for chaining (this-pointer demo)
BatteryModule& BatteryModule::setChargeLevel(double level) {
    chargeLevel = (level < 0.0) ? 0.0 : (level > 100.0) ? 100.0 : level;
    return *this;
}
BatteryModule& BatteryModule::setIsCharging(bool charging) {
    isCharging = charging;
    return *this;
}
BatteryModule& BatteryModule::setBatteryType(const std::string& type) {
    batteryType = type;
    return *this;
}
BatteryModule& BatteryModule::discharge(double amount) {
    chargeLevel -= amount;
    if (chargeLevel < 0.0) chargeLevel = 0.0;
    return *this;
}

void BatteryModule::display() const {
    std::cout << "  [Battery] Type: " << batteryType
              << " | Charge: " << std::fixed << std::setprecision(1)
              << chargeLevel << "%"
              << " | Charging: " << (isCharging ? "Yes" : "No")
              << " | Status: " << (isLow() ? "LOW BATTERY" : "OK")
              << "\n";
}


// ============================================================
//  CalibrationModule – Implementation
// ============================================================

CalibrationModule::CalibrationModule()
    : lastCalibrationDate("N/A"), validityPeriodDays(365),
      isCalibrated(false), calibratedBy("N/A") {}

CalibrationModule::CalibrationModule(const std::string& date, int validDays,
                                     bool calibrated, const std::string& tech)
    : lastCalibrationDate(date), validityPeriodDays(validDays),
      isCalibrated(calibrated), calibratedBy(tech) {}

CalibrationModule::CalibrationModule(const CalibrationModule& other)
    : lastCalibrationDate(other.lastCalibrationDate),
      validityPeriodDays(other.validityPeriodDays),
      isCalibrated(other.isCalibrated),
      calibratedBy(other.calibratedBy) {}

CalibrationModule& CalibrationModule::operator=(const CalibrationModule& other) {
    if (this != &other) {
        lastCalibrationDate = other.lastCalibrationDate;
        validityPeriodDays  = other.validityPeriodDays;
        isCalibrated        = other.isCalibrated;
        calibratedBy        = other.calibratedBy;
    }
    return *this;
}

// Accessors
std::string CalibrationModule::getLastCalibrationDate() const { return lastCalibrationDate; }
int         CalibrationModule::getValidityPeriodDays()  const { return validityPeriodDays; }
bool        CalibrationModule::getIsCalibrated()        const { return isCalibrated; }
std::string CalibrationModule::getCalibratedBy()        const { return calibratedBy; }

// Mutators
CalibrationModule& CalibrationModule::setLastCalibrationDate(const std::string& date) {
    lastCalibrationDate = date; return *this;
}
CalibrationModule& CalibrationModule::setIsCalibrated(bool status) {
    isCalibrated = status; return *this;
}
CalibrationModule& CalibrationModule::setCalibratedBy(const std::string& tech) {
    calibratedBy = tech; return *this;
}
CalibrationModule& CalibrationModule::setValidityPeriodDays(int days) {
    validityPeriodDays = (days > 0) ? days : 1; return *this;
}
CalibrationModule& CalibrationModule::calibrate(const std::string& date,
                                                 const std::string& tech) {
    lastCalibrationDate = date;
    calibratedBy        = tech;
    isCalibrated        = true;
    return *this;
}

void CalibrationModule::display() const {
    std::cout << "  [Calibration] Last: " << lastCalibrationDate
              << " | Valid for: " << validityPeriodDays << " days"
              << " | Calibrated: " << (isCalibrated ? "Yes" : "No")
              << " | By: " << calibratedBy
              << "\n";
}


// ============================================================
//  MaintenanceModule – Implementation
// ============================================================

MaintenanceModule::MaintenanceModule()
    : lastServiceDate("N/A"), serviceIntervalDays(90),
      serviceCount(0), maintenanceDue(false),
      assignedTechnician("Unassigned") {}

MaintenanceModule::MaintenanceModule(const std::string& date, int interval,
                                     int count, bool due,
                                     const std::string& tech)
    : lastServiceDate(date), serviceIntervalDays(interval),
      serviceCount(count), maintenanceDue(due),
      assignedTechnician(tech) {}

MaintenanceModule::MaintenanceModule(const MaintenanceModule& other)
    : lastServiceDate(other.lastServiceDate),
      serviceIntervalDays(other.serviceIntervalDays),
      serviceCount(other.serviceCount),
      maintenanceDue(other.maintenanceDue),
      assignedTechnician(other.assignedTechnician) {}

MaintenanceModule& MaintenanceModule::operator=(const MaintenanceModule& other) {
    if (this != &other) {
        lastServiceDate     = other.lastServiceDate;
        serviceIntervalDays = other.serviceIntervalDays;
        serviceCount        = other.serviceCount;
        maintenanceDue      = other.maintenanceDue;
        assignedTechnician  = other.assignedTechnician;
    }
    return *this;
}

// Accessors
std::string MaintenanceModule::getLastServiceDate()     const { return lastServiceDate; }
int         MaintenanceModule::getServiceIntervalDays() const { return serviceIntervalDays; }
int         MaintenanceModule::getServiceCount()        const { return serviceCount; }
bool        MaintenanceModule::isMaintenanceDue()       const { return maintenanceDue; }
std::string MaintenanceModule::getAssignedTechnician()  const { return assignedTechnician; }

// Mutators
MaintenanceModule& MaintenanceModule::setLastServiceDate(const std::string& date) {
    lastServiceDate = date; return *this;
}
MaintenanceModule& MaintenanceModule::setMaintenanceDue(bool due) {
    maintenanceDue = due; return *this;
}
MaintenanceModule& MaintenanceModule::setAssignedTechnician(const std::string& tech) {
    assignedTechnician = tech; return *this;
}
MaintenanceModule& MaintenanceModule::recordService(const std::string& date,
                                                     const std::string& tech) {
    lastServiceDate    = date;
    assignedTechnician = tech;
    ++serviceCount;
    maintenanceDue     = false;
    return *this;
}

void MaintenanceModule::display() const {
    std::cout << "  [Maintenance] Last Service: " << lastServiceDate
              << " | Interval: " << serviceIntervalDays << " days"
              << " | Services Done: " << serviceCount
              << " | Due: " << (maintenanceDue ? "YES" : "No")
              << " | Tech: " << assignedTechnician
              << "\n";
}
