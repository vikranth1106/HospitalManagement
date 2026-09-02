// ============================================================
// Hospital.cpp
// Implementation of the Hospital management class.
// Contains:
//  - Full CRUD for patients and equipment
//  - Smart allocation engine with 6 criteria
//  - 4 detailed reports
//  - Complete 12-option menu system
//  - All demonstration functions
// ============================================================

#include "Hospital.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>

// ============================================================
// Constructor
// ============================================================
Hospital::Hospital(const std::string& name, const std::string& loc)
    : hospitalName(name), location(loc) {}

// ============================================================
// Private helpers
// ============================================================

int Hospital::findPatientIndex(const std::string& patientID) const {
    for (int i = 0; i < static_cast<int>(patients.size()); ++i) {
        if (patients[i].getPatientID() == patientID) return i;
    }
    return -1;
}

int Hospital::findEquipmentIndex(const std::string& equipmentID) const {
    for (int i = 0; i < static_cast<int>(equipment.size()); ++i) {
        if (equipment[i]->getResourceID() == equipmentID) return i;
    }
    return -1;
}

// ============================================================
// Allocation eligibility check (Requirement 8)
// Criteria:
//   1. Equipment must be available
//   2. Battery must be > 20%
//   3. Must be calibrated
//   4. Must be in "Active" status
//   5. Ward must match patient's ward
//   6. Equipment must not already be assigned to this patient
// ============================================================
bool Hospital::canAllocate(const HospitalResource* res,
                            const Patient& patient) const {
    if (!res->getIsAvailable()) {
        std::cout << "  REJECT: Equipment not available.\n"; return false;
    }
    if (res->getBatteryModule().getChargeLevel() <= 20.0) {
        std::cout << "  REJECT: Battery too low ("
                  << res->getBatteryModule().getChargeLevel() << "%).\n"; return false;
    }
    if (!res->getCalibrationModule().getIsCalibrated()) {
        std::cout << "  REJECT: Equipment not calibrated.\n"; return false;
    }
    if (res->getOperatingStatus() != "Active") {
        std::cout << "  REJECT: Equipment status is "
                  << res->getOperatingStatus() << ".\n"; return false;
    }
    if (res->getWard() != patient.getWard()) {
        std::cout << "  REJECT: Ward mismatch – device in "
                  << res->getWard() << ", patient in "
                  << patient.getWard() << ".\n"; return false;
    }
    return true;
}

void Hospital::printHeader(const std::string& title) const {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║  " << std::left << std::setw(48) << title << "║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n";
}

// ============================================================
// Public: Add / Remove / Find – Patients
// ============================================================

bool Hospital::addPatient(const Patient& p) {
    if (findPatientIndex(p.getPatientID()) != -1) {
        std::cout << "  ERROR: Patient ID " << p.getPatientID()
                  << " already exists.\n";
        return false;
    }
    patients.push_back(p);
    std::cout << "  Patient " << p.getName()
              << " (" << p.getPatientID() << ") added.\n";
    return true;
}

bool Hospital::removePatient(const std::string& patientID) {
    int idx = findPatientIndex(patientID);
    if (idx == -1) { std::cout << "  ERROR: Patient not found.\n"; return false; }
    patients.erase(patients.begin() + idx);
    return true;
}

Patient* Hospital::findPatient(const std::string& patientID) {
    int idx = findPatientIndex(patientID);
    return (idx != -1) ? &patients[idx] : nullptr;
}

// ============================================================
// Public: Add / Remove / Find – Equipment
// Takes ownership of the raw pointer.
// ============================================================

bool Hospital::addEquipment(HospitalResource* resource) {
    if (!resource) return false;
    if (findEquipmentIndex(resource->getResourceID()) != -1) {
        std::cout << "  ERROR: Equipment ID " << resource->getResourceID()
                  << " already exists.\n";
        delete resource; // prevent leak if we reject it
        return false;
    }
    equipment.emplace_back(resource); // takes ownership
    std::cout << "  Equipment " << resource->getResourceName()
              << " (" << resource->getResourceID() << ") added.\n";
    return true;
}

bool Hospital::removeEquipment(const std::string& equipmentID) {
    int idx = findEquipmentIndex(equipmentID);
    if (idx == -1) { std::cout << "  ERROR: Equipment not found.\n"; return false; }
    equipment.erase(equipment.begin() + idx);
    return true;
}

HospitalResource* Hospital::findEquipment(const std::string& equipmentID) {
    int idx = findEquipmentIndex(equipmentID);
    return (idx != -1) ? equipment[idx].get() : nullptr;
}

// ============================================================
// Allocate Equipment (Requirement 8)
// ============================================================

bool Hospital::allocateEquipment(const std::string& patientID,
                                  const std::string& equipmentID) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        std::cout << "  ERROR: Patient " << patientID << " not found.\n";
        return false;
    }
    HospitalResource* res = findEquipment(equipmentID);
    if (!res) {
        std::cout << "  ERROR: Equipment " << equipmentID << " not found.\n";
        return false;
    }
    // Prevent duplicate allocation
    if (patient->hasEquipment(equipmentID)) {
        std::cout << "  ERROR: Equipment already assigned to this patient.\n";
        return false;
    }
    // Run eligibility checks
    if (!canAllocate(res, *patient)) return false;

    // Battery conservation rule:
    // If battery is between 20-50%, only HIGH or CRITICAL priority patients
    // may use the equipment. LOW/MEDIUM patients are redirected to wait for
    // a fully charged device (prevents depleting scarce battery resources).
    double batteryLevel = res->getBatteryModule().getChargeLevel();
    ClinicalPriority patientPriority = patient->getClinicalPriority();
    if (batteryLevel < 50.0 &&
        patientPriority != ClinicalPriority::CRITICAL &&
        patientPriority != ClinicalPriority::HIGH) {
        std::cout << "  WARN: Equipment battery at " << std::fixed
                  << std::setprecision(1) << batteryLevel
                  << "%. Only HIGH/CRITICAL priority patients allowed."
                     " Current patient priority: "
                  << priorityToString(patientPriority) << ".\n";
        return false;
    }

    // Perform allocation
    res->setIsAvailable(false);
    patient->assignEquipment(equipmentID);
    patient->addTreatmentCost(res->getDailyCostUSD());

    std::cout << "  SUCCESS: " << res->getResourceName()
              << " allocated to " << patient->getName() << ".\n";
    return true;
}

// ============================================================
// Release Equipment
// ============================================================

bool Hospital::releaseEquipment(const std::string& patientID,
                                 const std::string& equipmentID) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        std::cout << "  ERROR: Patient " << patientID << " not found.\n";
        return false;
    }
    if (!patient->hasEquipment(equipmentID)) {
        std::cout << "  ERROR: Equipment not assigned to this patient.\n";
        return false;
    }
    HospitalResource* res = findEquipment(equipmentID);
    if (!res) {
        std::cout << "  ERROR: Equipment " << equipmentID << " not found.\n";
        return false;
    }
    patient->releaseEquipment(equipmentID);
    res->setIsAvailable(true);
    std::cout << "  SUCCESS: " << res->getResourceName()
              << " released from " << patient->getName() << ".\n";
    return true;
}

// ============================================================
// Report: Equipment Availability
// ============================================================
void Hospital::reportEquipmentAvailability() const {
    printHeader("EQUIPMENT AVAILABILITY REPORT");
    if (equipment.empty()) {
        std::cout << "  No equipment registered.\n"; return;
    }
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(25) << "Name"
              << std::setw(16) << "Type"
              << std::setw(12) << "Ward"
              << std::setw(12) << "Available"
              << std::setw(10) << "Battery"
              << std::setw(12) << "Status"
              << "\n";
    std::cout << std::string(97, '-') << "\n";
    for (const auto& eq : equipment) {
        std::cout << std::left
                  << std::setw(10) << eq->getResourceID()
                  << std::setw(25) << eq->getResourceName()
                  << std::setw(16) << eq->getEquipmentType()
                  << std::setw(12) << eq->getWard()
                  << std::setw(12) << (eq->getIsAvailable() ? "Yes" : "No")
                  << std::setw(9)
                  << std::fixed << std::setprecision(1)
                  << eq->getBatteryModule().getChargeLevel() << "%"
                  << " " << eq->getOperatingStatus()
                  << "\n";
    }
}

// ============================================================
// Report: Patient Allocation
// ============================================================
void Hospital::reportPatientAllocation() const {
    printHeader("PATIENT ALLOCATION REPORT");
    if (patients.empty()) {
        std::cout << "  No patients registered.\n"; return;
    }
    for (const auto& p : patients) {
        std::cout << "\n  Patient: " << p.getName()
                  << " | ID: " << p.getPatientID()
                  << " | Ward: " << p.getWard()
                  << " | Priority: " << priorityToString(p.getClinicalPriority())
                  << "\n";
        const auto& ids = p.getAssignedEquipmentIDs();
        if (ids.empty()) {
            std::cout << "    -> No equipment assigned.\n";
        } else {
            for (const auto& eqID : ids) {
                int idx = findEquipmentIndex(eqID);
                if (idx != -1) {
                    std::cout << "    -> " << eqID << " | "
                              << equipment[idx]->getResourceName()
                              << " [" << equipment[idx]->getEquipmentType() << "]\n";
                } else {
                    std::cout << "    -> " << eqID << " (details not found)\n";
                }
            }
        }
    }
}

// ============================================================
// Report: Maintenance Due
// ============================================================
void Hospital::reportMaintenanceDue() const {
    printHeader("MAINTENANCE DUE REPORT");
    bool anyDue = false;
    for (const auto& eq : equipment) {
        const MaintenanceModule& mnt = eq->getMaintenanceModule();
        if (mnt.isMaintenanceDue() ||
            !eq->getCalibrationModule().getIsCalibrated()) {
            anyDue = true;
            std::cout << "  [!] " << eq->getResourceName()
                      << " (" << eq->getResourceID() << ")\n"
                      << "      Maintenance Due  : "
                      << (mnt.isMaintenanceDue() ? "YES" : "No") << "\n"
                      << "      Calibrated       : "
                      << (eq->getCalibrationModule().getIsCalibrated() ? "Yes" : "NO") << "\n"
                      << "      Last Service     : " << mnt.getLastServiceDate() << "\n"
                      << "      Technician       : " << mnt.getAssignedTechnician() << "\n\n";
        }
    }
    if (!anyDue) std::cout << "  All equipment is up to date. No maintenance required.\n";
}

// ============================================================
// Report: Cost Summary
// ============================================================
void Hospital::reportCostSummary() const {
    printHeader("COST SUMMARY REPORT");
    double totalEquipCost = 0.0;
    for (const auto& eq : equipment) {
        totalEquipCost += eq->getDailyCostUSD();
        std::cout << "  " << std::left << std::setw(25) << eq->getResourceName()
                  << " - $" << std::fixed << std::setprecision(2)
                  << eq->getDailyCostUSD() << "/day\n";
    }
    std::cout << "\n  Total Equipment Cost/Day : $"
              << std::fixed << std::setprecision(2) << totalEquipCost << "\n\n";

    double totalPatientCost = 0.0;
    for (const auto& p : patients) {
        totalPatientCost += p.getEstimatedTreatmentCostUSD();
        std::cout << "  Patient " << std::left << std::setw(20) << p.getName()
                  << " (" << p.getPatientID() << ") : $"
                  << std::fixed << std::setprecision(2)
                  << p.getEstimatedTreatmentCostUSD() << "\n";
    }
    std::cout << "\n  Total Patient Treatment Cost : $"
              << std::fixed << std::setprecision(2) << totalPatientCost << "\n";
    std::cout << "  Grand Total Cost             : $"
              << std::fixed << std::setprecision(2)
              << (totalEquipCost + totalPatientCost) << "\n";
}

// ============================================================
// Polymorphism demo – calls operate() through base pointer
// ============================================================
void Hospital::demonstratePolymorphism() const {
    printHeader("RUNTIME POLYMORPHISM DEMO");
    std::cout << "  Calling operate() on each HospitalResource* (base pointer):\n\n";
    for (const auto& eq : equipment) {
        // eq is unique_ptr<HospitalResource> – *eq is HospitalResource&
        // Virtual dispatch ensures the correct derived operate() runs
        std::cout << "  >> " << eq->getEquipmentType()
                  << " [" << eq->getResourceID() << "]\n";
        eq->operate();
        std::cout << "\n";
    }
}

// ============================================================
// Ventilator-pointer demo (Requirement 10)
// Uses dynamic_cast safely – avoids unsafe C-style casts.
// ============================================================
void Hospital::demonstrateVentilatorPointer() const {
    printHeader("VENTILATOR-SPECIFIC POINTER DEMO");
    std::cout << "  Searching for a Ventilator to call ventilator-specific functions...\n\n";
    bool found = false;
    for (const auto& eq : equipment) {
        // Safe downcast using dynamic_cast (no risk of undefined behaviour)
        Ventilator* vent = dynamic_cast<Ventilator*>(eq.get());
        if (vent != nullptr) {
            found = true;
            std::cout << "  Found Ventilator: " << vent->getResourceName()
                      << " [" << vent->getResourceID() << "]\n";
            // Call ventilator-specific function via Ventilator*
            vent->runSpontaneousBreathingTrial();
            std::cout << "  Current Mode   : "
                      << ventModeToString(vent->getVentMode()) << "\n"
                      << "  Tidal Volume   : " << vent->getTidalVolumeMl() << " mL\n"
                      << "  Minute Volume  : " << vent->getMinuteVolumeL() << " L/min\n";
            break;
        }
    }
    if (!found) {
        std::cout << "  No Ventilator found in equipment list. Add one first.\n";
    }
}

// ============================================================
// this-pointer chaining demo (Requirement 11)
// ============================================================
void Hospital::demonstrateMethodChaining() {
    printHeader("METHOD CHAINING DEMO (this POINTER)");

    // Demonstration of chainable MedicalEquipment updates
    MedicalEquipment demo("EQ-DEMO", "Demo IV Monitor",
                           "Monitor", "ICU", 250.0);
    std::cout << "  Initial state:\n" << demo << "\n";

    // Method chaining: updateBattery(...).updateCost(...).updateWard(...)
    std::cout << "  Applying chained updates:\n"
              << "    equipment.updateBattery(90).updateCost(4500).updateWard(\"ICU\")\n\n";
    demo.updateBattery(90)
        .updateCost(4500)
        .updateWard("ICU")
        .updateStatus("Active");

    std::cout << "  After chained updates:\n" << demo;
    demoEquipment.push_back(demo); // store for later demos
}

// ============================================================
// Operator Overloading demo (Requirement 11)
// ============================================================
void Hospital::demonstrateOperatorOverloading() {
    printHeader("OPERATOR OVERLOADING DEMO");

    // Create two equipment objects for demonstration
    MedicalEquipment eq1("OP-001", "ICU Monitor A", "Monitor", "ICU", 300.0);
    MedicalEquipment eq2("OP-002", "ICU Monitor B", "Monitor", "ICU", 200.0);

    // Set up module states manually to make the demo meaningful
    eq1.getBatteryModuleRef().setChargeLevel(85.0);
    eq1.getCalibrationModuleRef().setIsCalibrated(true);
    eq1.addUsageHours(48);
    eq1.addServiceRecord("2026-08-01: Full service by Ravi Kumar");
    eq1.addServiceRecord("2026-08-15: Calibration check");

    eq2.getBatteryModuleRef().setChargeLevel(40.0);
    eq2.getCalibrationModuleRef().setIsCalibrated(false);
    eq2.addUsageHours(120);

    std::cout << "\n  === Equipment 1 ===\n" << eq1;
    std::cout << "\n  === Equipment 2 ===\n" << eq2;

    // operator+
    std::cout << "\n  -- operator+ (Combine usage statistics) --\n";
    MedicalEquipment combined = eq1 + eq2;
    std::cout << "  Combined Usage Hours : " << combined.getUsageDurationHours() << "\n"
              << "  Combined Daily Cost  : $" << std::fixed << std::setprecision(2)
              << combined.getDailyCostUSD() << "\n";

    // operator<
    std::cout << "\n  -- operator< (Allocation Suitability) --\n";
    bool eq1Better = (eq1 < eq2);
    std::cout << "  Suitability score EQ1: " << eq1.computeSuitabilityScore() << "/100\n"
              << "  Suitability score EQ2: " << eq2.computeSuitabilityScore() << "/100\n";
    std::cout << "  eq1 < eq2 result: " << (eq1Better ? "EQ1 is MORE suitable" : "EQ2 is more suitable") << "\n";

    // operator<<
    std::cout << "\n  -- operator<< (Formatted Report) --\n";
    std::cout << eq1;

    // operator==
    std::cout << "\n  -- operator== (ID Equality) --\n";
    std::cout << "  eq1 == eq1 : " << ((eq1 == eq1) ? "true" : "false") << "\n"
              << "  eq1 == eq2 : " << ((eq1 == eq2) ? "true" : "false") << "\n";

    // Deep-copy demonstration
    std::cout << "\n  -- Copy Constructor (Deep Copy Demo) --\n";
    MedicalEquipment copiedEq(eq1); // invokes copy constructor
    copiedEq.addServiceRecord("2026-09-01: Post-copy service record");
    std::cout << "  Original service records: " << eq1.getServiceHistorySize() << "\n";
    std::cout << "  Copied   service records: " << copiedEq.getServiceHistorySize()
              << " (copy has its own independent history)\n";
    eq1.displayServiceHistory();
    copiedEq.displayServiceHistory();
}

// ============================================================
// Seed Demo Data – prepopulates system for testing
// ============================================================
void Hospital::seedDemoData() {
    printHeader("SEEDING DEMONSTRATION DATA");

    // --- Patients ---
    addPatient(Patient("PT-001", "Arjun Sharma",    45, "ICU",       ClinicalPriority::CRITICAL, RiskCategory::EXTREME));
    addPatient(Patient("PT-002", "Priya Nair",      32, "General",   ClinicalPriority::MEDIUM,   RiskCategory::MODERATE));
    addPatient(Patient("PT-003", "Mohan Reddy",     67, "Emergency", ClinicalPriority::HIGH,     RiskCategory::HIGH));
    addPatient(Patient("PT-004", "Sunita Verma",    28, "ICU",       ClinicalPriority::CRITICAL, RiskCategory::EXTREME));
    addPatient(Patient("PT-005", "Rajesh Kumar",    55, "Cardiology",ClinicalPriority::HIGH,     RiskCategory::HIGH));

    // --- Patient Monitors ---
    PatientMonitor* pm1 = new PatientMonitor("PM-001", "Bedside Monitor Alpha", "PhilipsMed", "ICU", 150.0);
    pm1->getBatteryModuleRef().setChargeLevel(92.0);
    pm1->getCalibrationModuleRef().calibrate("2026-08-01", "Dr. Rajan");
    pm1->setVitals(78, 99, 122, 82, 37.1);

    PatientMonitor* pm2 = new PatientMonitor("PM-002", "Bedside Monitor Beta", "GEHealthcare", "General", 120.0);
    pm2->getBatteryModuleRef().setChargeLevel(75.0);
    pm2->getCalibrationModuleRef().calibrate("2026-07-15", "Tech. Priya");

    PatientMonitor* pm3 = new PatientMonitor("PM-003", "Emergency Monitor", "DragerMed", "Emergency", 180.0);
    pm3->getBatteryModuleRef().setChargeLevel(60.0);
    pm3->getCalibrationModuleRef().calibrate("2026-08-20", "Tech. Ramesh");

    addEquipment(pm1);
    addEquipment(pm2);
    addEquipment(pm3);

    // --- Infusion Pumps ---
    InfusionPump* ip1 = new InfusionPump("IP-001", "Infusion Pump Gamma", "BraunMed", "ICU", 200.0);
    ip1->getBatteryModuleRef().setChargeLevel(88.0);
    ip1->getCalibrationModuleRef().calibrate("2026-08-10", "Tech. Suresh");
    ip1->setMedication("Norepinephrine", "Vasopressor", 250.0, 5.0);

    InfusionPump* ip2 = new InfusionPump("IP-002", "Infusion Pump Delta", "BaxterMed", "General", 160.0);
    ip2->getBatteryModuleRef().setChargeLevel(15.0); // LOW battery – cannot allocate
    ip2->getCalibrationModuleRef().setIsCalibrated(false); // Uncalibrated
    ip2->getMaintenanceModuleRef().setMaintenanceDue(true);

    addEquipment(ip1);
    addEquipment(ip2);

    // --- Ventilators ---
    Ventilator* vt1 = new Ventilator("VT-001", "Ventilator Prime", "DragerVent", "ICU", 500.0);
    vt1->getBatteryModuleRef().setChargeLevel(95.0);
    vt1->getCalibrationModuleRef().calibrate("2026-08-05", "Dr. Mehta");
    vt1->setVentilationParameters(VentilationMode::VOLUME_CONTROL, 450, 16, 50.0, 5.0);

    Ventilator* vt2 = new Ventilator("VT-002", "Ventilator Apex", "Maquet", "Emergency", 480.0);
    vt2->getBatteryModuleRef().setChargeLevel(70.0);
    vt2->getCalibrationModuleRef().calibrate("2026-07-20", "Tech. Anand");

    addEquipment(vt1);
    addEquipment(vt2);

    // --- Hybrid Critical Care Device ---
    HybridCriticalCareDevice* hybrid = new HybridCriticalCareDevice(
        "HB-001", "HybridCare Elite", "SiemensHealthineers", "ICU", 800.0);
    hybrid->getBatteryModuleRef().setChargeLevel(90.0);
    hybrid->getCalibrationModuleRef().calibrate("2026-08-12", "Dr. Kapoor");
    hybrid->loadClinicalProfile("ARDS Lung-Protective Protocol");
    hybrid->assignToICUBed(3);

    addEquipment(hybrid);

    // --- Perform some allocations ---
    std::cout << "\n  Performing initial allocations...\n";
    allocateEquipment("PT-001", "PM-001");
    allocateEquipment("PT-001", "VT-001");
    allocateEquipment("PT-001", "IP-001");
    allocateEquipment("PT-002", "PM-002");
    allocateEquipment("PT-003", "PM-003");

    std::cout << "\n  Demo data seeded successfully.\n";
}

// ============================================================
// Menu sub-handlers
// ============================================================

static std::string getStringInput(const std::string& prompt) {
    std::cout << "  " << prompt;
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

static int getIntInput(const std::string& prompt, int lo, int hi) {
    int val;
    while (true) {
        std::cout << "  " << prompt;
        if (std::cin >> val && val >= lo && val <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cout << "  Invalid input. Enter a number between "
                  << lo << " and " << hi << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static double getDoubleInput(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << "  " << prompt;
        if (std::cin >> val && val >= 0.0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cout << "  Invalid input. Enter a positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// ---- Add Patient menu ----
void Hospital::menuAddPatient() {
    printHeader("ADD PATIENT");
    std::string id   = getStringInput("Patient ID (e.g. PT-010): ");
    std::string name = getStringInput("Full Name: ");
    int age          = getIntInput("Age (0-150): ", 0, 150);
    std::string ward = getStringInput("Ward (ICU/General/Emergency/Cardiology): ");

    std::cout << "  Priority: 1=Low 2=Medium 3=High 4=Critical\n";
    int pri = getIntInput("Priority: ", 1, 4);
    std::cout << "  Risk: 1=Stable 2=Moderate 3=High 4=Extreme\n";
    int risk = getIntInput("Risk: ", 1, 4);

    Patient p(id, name, age, ward,
              static_cast<ClinicalPriority>(pri),
              static_cast<RiskCategory>(risk));
    addPatient(p);
}

// ---- Display Patients menu ----
void Hospital::menuDisplayPatients() {
    printHeader("ALL PATIENTS");
    if (patients.empty()) { std::cout << "  No patients registered.\n"; return; }
    for (const auto& p : patients) p.display();
}

// ---- Add Equipment menu ----
void Hospital::menuAddEquipment() {
    printHeader("ADD EQUIPMENT");
    std::cout << "  Select type:\n"
              << "  1. Patient Monitor\n"
              << "  2. Infusion Pump\n"
              << "  3. Ventilator\n"
              << "  4. Hybrid Critical Care Device\n";
    int type = getIntInput("Type: ", 1, 4);

    std::string id   = getStringInput("Equipment ID (e.g. PM-010): ");
    std::string name = getStringInput("Equipment Name: ");
    std::string mfr  = getStringInput("Manufacturer: ");
    std::string ward = getStringInput("Ward: ");
    double cost      = getDoubleInput("Daily Cost (USD): ");
    double battery   = getDoubleInput("Battery Level (0-100): ");
    int    calStatus = getIntInput("Is Calibrated? 1=Yes 0=No: ", 0, 1);

    HospitalResource* res = nullptr;
    switch (type) {
        case 1: {
            PatientMonitor* pm = new PatientMonitor(id, name, mfr, ward, cost);
            pm->getBatteryModuleRef().setChargeLevel(battery);
            pm->getCalibrationModuleRef().setIsCalibrated(calStatus == 1);
            res = pm;
            break;
        }
        case 2: {
            InfusionPump* ip = new InfusionPump(id, name, mfr, ward, cost);
            ip->getBatteryModuleRef().setChargeLevel(battery);
            ip->getCalibrationModuleRef().setIsCalibrated(calStatus == 1);
            res = ip;
            break;
        }
        case 3: {
            Ventilator* vt = new Ventilator(id, name, mfr, ward, cost);
            vt->getBatteryModuleRef().setChargeLevel(battery);
            vt->getCalibrationModuleRef().setIsCalibrated(calStatus == 1);
            res = vt;
            break;
        }
        case 4: {
            HybridCriticalCareDevice* hb =
                new HybridCriticalCareDevice(id, name, mfr, ward, cost);
            hb->getBatteryModuleRef().setChargeLevel(battery);
            hb->getCalibrationModuleRef().setIsCalibrated(calStatus == 1);
            res = hb;
            break;
        }
    }
    if (res) addEquipment(res);
}

// ---- Display Equipment menu ----
void Hospital::menuDisplayEquipment() {
    printHeader("ALL EQUIPMENT");
    if (equipment.empty()) { std::cout << "  No equipment registered.\n"; return; }
    for (const auto& eq : equipment) {
        eq->generateReport(); // Virtual dispatch – correct type's report
    }
}

// ---- Allocate Equipment menu ----
void Hospital::menuAllocateEquipment() {
    printHeader("ALLOCATE EQUIPMENT");
    std::string pid = getStringInput("Patient ID: ");
    std::string eid = getStringInput("Equipment ID: ");
    allocateEquipment(pid, eid);
}

// ---- Release Equipment menu ----
void Hospital::menuReleaseEquipment() {
    printHeader("RELEASE EQUIPMENT");
    std::string pid = getStringInput("Patient ID: ");
    std::string eid = getStringInput("Equipment ID: ");
    releaseEquipment(pid, eid);
}

// ---- Maintenance Report menu ----
void Hospital::menuMaintenanceReport() {
    reportMaintenanceDue();
}

// ---- Availability Report menu ----
void Hospital::menuAvailabilityReport() {
    reportEquipmentAvailability();
}

// ---- Patient Allocation Report menu ----
void Hospital::menuPatientAllocationReport() {
    reportPatientAllocation();
}

// ---- Cost Report menu ----
void Hospital::menuCostReport() {
    reportCostSummary();
}

// ---- Demonstrate Operators ----
void Hospital::menuDemonstrateOperators() {
    std::cout << "\n  Select demonstration:\n"
              << "  1. Operator Overloading (+, <, ==, <<)\n"
              << "  2. Method Chaining (this pointer)\n"
              << "  3. Runtime Polymorphism (operate() via base pointer)\n"
              << "  4. Ventilator-Specific Pointer (dynamic_cast)\n";
    int choice = getIntInput("Choice: ", 1, 4);
    switch (choice) {
        case 1: demonstrateOperatorOverloading(); break;
        case 2: demonstrateMethodChaining();      break;
        case 3: demonstratePolymorphism();        break;
        case 4: demonstrateVentilatorPointer();   break;
    }
}

// ============================================================
// run() – Main Menu Loop
// ============================================================
void Hospital::run() {
    // Print welcome banner
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║    SMART HOSPITAL PATIENT & EQUIPMENT MANAGEMENT SYSTEM  ║\n";
    std::cout << "║    " << std::left << std::setw(54) << hospitalName << "║\n";
    std::cout << "║    " << std::left << std::setw(54) << location     << "║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    bool running = true;
    while (running) {
        std::cout << "\n┌──────────────────────────────────────────┐\n"
                  << "│                MAIN MENU                 │\n"
                  << "├──────────────────────────────────────────┤\n"
                  << "│  1.  Add Patient                         │\n"
                  << "│  2.  Display Patients                    │\n"
                  << "│  3.  Add Equipment                       │\n"
                  << "│  4.  Display Equipment                   │\n"
                  << "│  5.  Allocate Equipment                  │\n"
                  << "│  6.  Release Equipment                   │\n"
                  << "│  7.  Maintenance Report                  │\n"
                  << "│  8.  Equipment Availability Report       │\n"
                  << "│  9.  Patient Allocation Report           │\n"
                  << "│  10. Cost Report                         │\n"
                  << "│  11. Demonstrate Operators & Polymorphism│\n"
                  << "│  12. Exit                                │\n"
                  << "└──────────────────────────────────────────┘\n";

        int choice = getIntInput("Enter choice (1-12): ", 1, 12);
        switch (choice) {
            case 1:  menuAddPatient();              break;
            case 2:  menuDisplayPatients();         break;
            case 3:  menuAddEquipment();            break;
            case 4:  menuDisplayEquipment();        break;
            case 5:  menuAllocateEquipment();       break;
            case 6:  menuReleaseEquipment();        break;
            case 7:  menuMaintenanceReport();       break;
            case 8:  menuAvailabilityReport();      break;
            case 9:  menuPatientAllocationReport(); break;
            case 10: menuCostReport();              break;
            case 11: menuDemonstrateOperators();    break;
            case 12:
                std::cout << "\n  Thank you for using the Hospital Management System.\n"
                          << "  All memory has been safely released.\n\n";
                running = false;
                break;
        }
    }
}
