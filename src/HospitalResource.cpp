// ============================================================
// HospitalResource.cpp
// Implementation of the HospitalResource abstract base class.
// ============================================================

#include "HospitalResource.h"
#include <iomanip>

// Default constructor – initialises all fields to safe defaults
HospitalResource::HospitalResource()
    : ResourceIdentity(),
      ward("General"),
      operatingStatus("Inactive"),
      isAvailable(true),
      dailyCostUSD(0.0),
      usageDurationHours(0),
      battery(),
      calibration(),
      maintenance() {}

// Parameterised constructor
HospitalResource::HospitalResource(const std::string& id,
                                   const std::string& name,
                                   const std::string& mfr,
                                   const std::string& w,
                                   double dailyCost)
    : ResourceIdentity(id, name, mfr),
      ward(w),
      operatingStatus("Active"),
      isAvailable(true),
      dailyCostUSD(dailyCost),
      usageDurationHours(0),
      battery(),
      calibration(),
      maintenance() {}

// ----- Accessors -----
std::string HospitalResource::getWard()               const { return ward; }
std::string HospitalResource::getOperatingStatus()    const { return operatingStatus; }
bool        HospitalResource::getIsAvailable()        const { return isAvailable; }
double      HospitalResource::getDailyCostUSD()       const { return dailyCostUSD; }
int         HospitalResource::getUsageDurationHours() const { return usageDurationHours; }

const BatteryModule&     HospitalResource::getBatteryModule()     const { return battery; }
const CalibrationModule& HospitalResource::getCalibrationModule() const { return calibration; }
const MaintenanceModule& HospitalResource::getMaintenanceModule() const { return maintenance; }

BatteryModule&     HospitalResource::getBatteryModuleRef()     { return battery; }
CalibrationModule& HospitalResource::getCalibrationModuleRef() { return calibration; }
MaintenanceModule& HospitalResource::getMaintenanceModuleRef() { return maintenance; }

// ----- Mutators -----
void HospitalResource::setWard(const std::string& w)              { ward = w; }
void HospitalResource::setOperatingStatus(const std::string& s)   { operatingStatus = s; }
void HospitalResource::setIsAvailable(bool avail)                 { isAvailable = avail; }
void HospitalResource::setDailyCostUSD(double cost)               { dailyCostUSD = (cost >= 0) ? cost : 0; }
void HospitalResource::addUsageHours(int hours)                   { usageDurationHours += hours; }

void HospitalResource::startMaintenance() {
    operatingStatus = "Under Maintenance";
    isAvailable     = false;
}

void HospitalResource::completeMaintenance() {
    operatingStatus = "Active";
    isAvailable     = true;
    maintenance.setMaintenanceDue(false);
}

// ----- Display (common base output, overridden by derived classes) -----
void HospitalResource::display() const {
    std::cout << "\n========================================\n";
    std::cout << "  Equipment Type : " << getEquipmentType() << "\n";
    displayIdentity();
    std::cout << "  Ward           : " << ward << "\n"
              << "  Status         : " << operatingStatus << "\n"
              << "  Available      : " << (isAvailable ? "Yes" : "No") << "\n"
              << "  Daily Cost     : $" << std::fixed << std::setprecision(2)
              << dailyCostUSD << "\n"
              << "  Usage Hours    : " << usageDurationHours << " hrs\n";
    battery.display();
    calibration.display();
    maintenance.display();
    std::cout << "========================================\n";
}
