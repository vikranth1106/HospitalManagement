// ============================================================
// MedicalEquipment.cpp
// Implementation of MedicalEquipment class including:
//   - Four constructors (default, parameterised, overloaded, copy)
//   - Deep copy via raw pointer management
//   - Destructor
//   - Operator overloading (+, <, ==, <<)
//   - Chainable updaters using 'this' pointer
//   - Service history with dynamic array
// ============================================================

#include "MedicalEquipment.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// ---- Private helpers ----

// Allocate the service history array with given capacity
void MedicalEquipment::allocateHistory(int capacity) {
    serviceHistoryCapacity = (capacity > 0) ? capacity : 10;
    serviceHistory         = new std::string[serviceHistoryCapacity];
    serviceHistorySize     = 0;
}

// Deep-copy the service history from another object
void MedicalEquipment::copyHistory(const MedicalEquipment& other) {
    allocateHistory(other.serviceHistoryCapacity);
    serviceHistorySize = other.serviceHistorySize;
    for (int i = 0; i < serviceHistorySize; ++i) {
        serviceHistory[i] = other.serviceHistory[i];
    }
}

// Release heap memory
void MedicalEquipment::releaseHistory() {
    delete[] serviceHistory;
    serviceHistory         = nullptr;
    serviceHistoryCapacity = 0;
    serviceHistorySize     = 0;
}


// ============================================================
// (1) Default Constructor
// ============================================================
MedicalEquipment::MedicalEquipment()
    : equipmentID("EQ-000"),
      equipmentName("Unknown Equipment"),
      equipmentType("General"),
      ward("General"),
      manufacturer("Unknown"),
      operatingStatus("Inactive"),
      isAvailable(true),
      dailyCostUSD(0.0),
      usageDurationHours(0),
      battery(),
      calibration(),
      maintenance(),
      serviceHistory(nullptr),
      serviceHistoryCapacity(0),
      serviceHistorySize(0) {
    allocateHistory(10);
}

// ============================================================
// (2) Parameterised Constructor – core fields only
// ============================================================
MedicalEquipment::MedicalEquipment(const std::string& id,
                                   const std::string& name,
                                   const std::string& type,
                                   const std::string& w,
                                   double dailyCost)
    : equipmentID(id),
      equipmentName(name),
      equipmentType(type),
      ward(w),
      manufacturer("Generic"),
      operatingStatus("Active"),
      isAvailable(true),
      dailyCostUSD(dailyCost),
      usageDurationHours(0),
      battery(),
      calibration(),
      maintenance(),
      serviceHistory(nullptr),
      serviceHistoryCapacity(0),
      serviceHistorySize(0) {
    allocateHistory(10);
}

// ============================================================
// (3) Overloaded Constructor – includes full module initialisation
// ============================================================
MedicalEquipment::MedicalEquipment(const std::string& id,
                                   const std::string& name,
                                   const std::string& type,
                                   const std::string& w,
                                   double dailyCost,
                                   const BatteryModule& bat,
                                   const CalibrationModule& cal,
                                   const MaintenanceModule& mnt)
    : equipmentID(id),
      equipmentName(name),
      equipmentType(type),
      ward(w),
      manufacturer("Generic"),
      operatingStatus("Active"),
      isAvailable(true),
      dailyCostUSD(dailyCost),
      usageDurationHours(0),
      battery(bat),
      calibration(cal),
      maintenance(mnt),
      serviceHistory(nullptr),
      serviceHistoryCapacity(0),
      serviceHistorySize(0) {
    allocateHistory(10);
}

// ============================================================
// (4) Copy Constructor – DEEP COPY
// Allocates a separate heap array and copies all entries.
// ============================================================
MedicalEquipment::MedicalEquipment(const MedicalEquipment& other)
    : equipmentID(other.equipmentID),
      equipmentName(other.equipmentName),
      equipmentType(other.equipmentType),
      ward(other.ward),
      manufacturer(other.manufacturer),
      operatingStatus(other.operatingStatus),
      isAvailable(other.isAvailable),
      dailyCostUSD(other.dailyCostUSD),
      usageDurationHours(other.usageDurationHours),
      battery(other.battery),
      calibration(other.calibration),
      maintenance(other.maintenance),
      serviceHistory(nullptr),
      serviceHistoryCapacity(0),
      serviceHistorySize(0) {
    // Deep copy the dynamically allocated service history
    copyHistory(other);
}

// ============================================================
// Copy-Assignment Operator – DEEP COPY with self-assignment guard
// ============================================================
MedicalEquipment& MedicalEquipment::operator=(const MedicalEquipment& other) {
    if (this == &other) return *this; // Guard against self-assignment

    // Copy all value-type fields
    equipmentID        = other.equipmentID;
    equipmentName      = other.equipmentName;
    equipmentType      = other.equipmentType;
    ward               = other.ward;
    manufacturer       = other.manufacturer;
    operatingStatus    = other.operatingStatus;
    isAvailable        = other.isAvailable;
    dailyCostUSD       = other.dailyCostUSD;
    usageDurationHours = other.usageDurationHours;
    battery            = other.battery;
    calibration        = other.calibration;
    maintenance        = other.maintenance;

    // Release old heap memory, then deep-copy from other
    releaseHistory();
    copyHistory(other);

    return *this;
}

// ============================================================
// Destructor – releases heap-allocated serviceHistory
// ============================================================
MedicalEquipment::~MedicalEquipment() {
    releaseHistory();
}


// ============================================================
// operator+ : Combine two equipment objects.
// The result has summed usageDurationHours and dailyCostUSD,
// and uses the ID/name of the left-hand operand.
// ============================================================
MedicalEquipment MedicalEquipment::operator+(const MedicalEquipment& other) const {
    MedicalEquipment result(*this);  // deep copy of *this
    result.usageDurationHours += other.usageDurationHours;
    result.dailyCostUSD       += other.dailyCostUSD;
    result.equipmentName       = equipmentName + " + " + other.equipmentName;
    return result;
}

// ============================================================
// operator< : Allocation suitability comparison.
// Returns true if *this is MORE suitable than 'other'.
// Criteria (in priority order):
//   1. Calibration status (calibrated > uncalibrated)
//   2. Availability (available > unavailable)
//   3. Operating status (Active > other)
//   4. Battery level (higher is better)
// ============================================================
bool MedicalEquipment::operator<(const MedicalEquipment& other) const {
    return computeSuitabilityScore() > other.computeSuitabilityScore();
}

// ============================================================
// operator== : Equality by equipment ID
// ============================================================
bool MedicalEquipment::operator==(const MedicalEquipment& other) const {
    return equipmentID == other.equipmentID;
}

// ============================================================
// operator<< : Formatted equipment report
// ============================================================
std::ostream& operator<<(std::ostream& os, const MedicalEquipment& eq) {
    os << "\n╔══════════════════════════════════════════════╗\n";
    os << "║         MEDICAL EQUIPMENT REPORT             ║\n";
    os << "╠══════════════════════════════════════════════╣\n";
    os << "║ ID          : " << std::left << std::setw(31) << eq.equipmentID   << "║\n";
    os << "║ Name        : " << std::left << std::setw(31) << eq.equipmentName << "║\n";
    os << "║ Type        : " << std::left << std::setw(31) << eq.equipmentType << "║\n";
    os << "║ Ward        : " << std::left << std::setw(31) << eq.ward          << "║\n";
    os << "║ Manufacturer: " << std::left << std::setw(31) << eq.manufacturer  << "║\n";
    os << "║ Status      : " << std::left << std::setw(31) << eq.operatingStatus << "║\n";
    os << "║ Available   : " << std::left << std::setw(31)
       << (eq.isAvailable ? "Yes" : "No") << "║\n";
    os << "║ Daily Cost  : $" << std::left << std::setw(30)
       << std::fixed << std::setprecision(2) << eq.dailyCostUSD << "║\n";
    os << "║ Usage Hours : " << std::left << std::setw(31) << eq.usageDurationHours << "║\n";
    {
        // Build battery string first so setw pads the whole thing
        std::ostringstream bat;
        bat << std::fixed << std::setprecision(1) << eq.battery.getChargeLevel() << "%";
        os << "║ Battery     : " << std::left << std::setw(31) << bat.str() << "║\n";
    }
    os << "║ Calibrated  : " << std::left << std::setw(31)
       << (eq.calibration.getIsCalibrated() ? "Yes" : "No") << "║\n";
    os << "║ Maint. Due  : " << std::left << std::setw(31)
       << (eq.maintenance.isMaintenanceDue() ? "YES" : "No") << "║\n";
    os << "║ Suitability : " << std::left << std::setw(28)
       << eq.computeSuitabilityScore() << "/100║\n";
    os << "╚══════════════════════════════════════════════╝\n";
    return os;
}


// ============================================================
// Chainable update functions – return *this (this-pointer demo)
// ============================================================

MedicalEquipment& MedicalEquipment::updateBattery(double chargeLevel) {
    battery.setChargeLevel(chargeLevel);
    return *this;  // return *this enables method chaining
}

MedicalEquipment& MedicalEquipment::updateCost(double cost) {
    dailyCostUSD = (cost >= 0.0) ? cost : 0.0;
    return *this;
}

MedicalEquipment& MedicalEquipment::updateWard(const std::string& newWard) {
    ward = newWard;
    return *this;
}

MedicalEquipment& MedicalEquipment::updateStatus(const std::string& status) {
    operatingStatus = status;
    return *this;
}


// ============================================================
// Service History
// ============================================================

void MedicalEquipment::addServiceRecord(const std::string& record) {
    if (serviceHistorySize >= serviceHistoryCapacity) {
        // Grow the array (double capacity) – manual dynamic memory management
        int newCapacity      = serviceHistoryCapacity * 2;
        std::string* newArr  = new std::string[newCapacity];
        for (int i = 0; i < serviceHistorySize; ++i) {
            newArr[i] = serviceHistory[i];
        }
        delete[] serviceHistory;
        serviceHistory         = newArr;
        serviceHistoryCapacity = newCapacity;
    }
    serviceHistory[serviceHistorySize++] = record;
}

void MedicalEquipment::displayServiceHistory() const {
    std::cout << "\n  --- Service History for " << equipmentName << " ---\n";
    if (serviceHistorySize == 0) {
        std::cout << "  (No service records)\n";
        return;
    }
    for (int i = 0; i < serviceHistorySize; ++i) {
        std::cout << "  [" << (i + 1) << "] " << serviceHistory[i] << "\n";
    }
}


// ============================================================
// Accessors
// ============================================================
std::string MedicalEquipment::getEquipmentID()        const { return equipmentID; }
std::string MedicalEquipment::getEquipmentName()      const { return equipmentName; }
std::string MedicalEquipment::getEquipmentType()      const { return equipmentType; }
std::string MedicalEquipment::getWard()               const { return ward; }
std::string MedicalEquipment::getManufacturer()       const { return manufacturer; }
std::string MedicalEquipment::getOperatingStatus()    const { return operatingStatus; }
bool        MedicalEquipment::getIsAvailable()        const { return isAvailable; }
double      MedicalEquipment::getDailyCostUSD()       const { return dailyCostUSD; }
int         MedicalEquipment::getUsageDurationHours() const { return usageDurationHours; }
int         MedicalEquipment::getServiceHistorySize() const { return serviceHistorySize; }

const BatteryModule&     MedicalEquipment::getBatteryModule()     const { return battery; }
const CalibrationModule& MedicalEquipment::getCalibrationModule() const { return calibration; }
const MaintenanceModule& MedicalEquipment::getMaintenanceModule() const { return maintenance; }

BatteryModule&     MedicalEquipment::getBatteryModuleRef()     { return battery; }
CalibrationModule& MedicalEquipment::getCalibrationModuleRef() { return calibration; }
MaintenanceModule& MedicalEquipment::getMaintenanceModuleRef() { return maintenance; }


// ============================================================
// Mutators
// ============================================================
void MedicalEquipment::setEquipmentID(const std::string& id)     { equipmentID   = id; }
void MedicalEquipment::setEquipmentName(const std::string& name) { equipmentName = name; }
void MedicalEquipment::setWard(const std::string& w)             { ward          = w; }
void MedicalEquipment::setManufacturer(const std::string& mfr)   { manufacturer  = mfr; }
void MedicalEquipment::setIsAvailable(bool avail)                { isAvailable   = avail; }
void MedicalEquipment::setUsageDurationHours(int hours)          { usageDurationHours = hours; }
void MedicalEquipment::addUsageHours(int hours)                  { usageDurationHours += hours; }


// ============================================================
// display() – uses operator<<
// ============================================================
void MedicalEquipment::display() const {
    std::cout << *this;
    displayServiceHistory();
}


// ============================================================
// computeSuitabilityScore
// Returns 0-100 based on:
//   - Battery level       (40 pts max)
//   - Calibrated          (25 pts)
//   - Available           (20 pts)
//   - Active status       (15 pts)
// ============================================================
int MedicalEquipment::computeSuitabilityScore() const {
    int score = 0;
    score += static_cast<int>(battery.getChargeLevel() * 0.40); // max 40
    if (calibration.getIsCalibrated())         score += 25;
    if (isAvailable)                           score += 20;
    if (operatingStatus == "Active")           score += 15;
    return score;
}
