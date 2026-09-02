// ============================================================
// Hospital.h
//
// The Hospital class manages all patients and equipment.
// It provides:
//   - CRUD for patients and equipment (using HospitalResource*)
//   - Equipment allocation / release logic
//   - 4 report generators
//   - Menu-driven interface (12 options)
//   - Operator overloading demonstration
//   - Virtual function (polymorphism) demonstration
//   - Derived-class pointer (Ventilator*) demonstration
//   - this-pointer chaining demonstration
// ============================================================

#pragma once

#include "Patient.h"
#include "MedicalEquipment.h"
#include "PatientMonitor.h"
#include "InfusionPump.h"
#include "Ventilator.h"
#include "HybridCriticalCareDevice.h"

#include <vector>
#include <memory>
#include <string>

// ============================================================
// Hospital
// Central management class for the entire system.
// Owns patients and equipment via smart / raw pointers.
// ============================================================
class Hospital {
private:
    std::string hospitalName;
    std::string location;

    // Patient registry (value semantics – vector of Patient objects)
    std::vector<Patient> patients;

    // Equipment registry (polymorphic – stores HospitalResource* pointers)
    // Using unique_ptr for automatic memory management and no leaks
    std::vector<std::unique_ptr<HospitalResource>> equipment;

    // Standalone MedicalEquipment for operator-overloading demos
    std::vector<MedicalEquipment> demoEquipment;

    // ----- Private helpers -----

    // Find patient index by ID (-1 if not found)
    int findPatientIndex(const std::string& patientID) const;

    // Find equipment index by ID (-1 if not found)
    int findEquipmentIndex(const std::string& equipmentID) const;

    // Check allocation eligibility
    bool canAllocate(const HospitalResource* res,
                     const Patient& patient) const;

    // Print a section header
    void printHeader(const std::string& title) const;

    // ----- Menu sub-handlers -----
    void menuAddPatient();
    void menuDisplayPatients();
    void menuAddEquipment();
    void menuDisplayEquipment();
    void menuAllocateEquipment();
    void menuReleaseEquipment();
    void menuMaintenanceReport();
    void menuAvailabilityReport();
    void menuPatientAllocationReport();
    void menuCostReport();
    void menuDemonstrateOperators();

public:
    // Constructor
    Hospital(const std::string& name, const std::string& loc);

    // Destructor (unique_ptr handles equipment cleanup automatically)
    ~Hospital() = default;

    // ----- Main menu entry point -----
    void run();

    // ----- Patient management -----
    bool addPatient(const Patient& p);
    bool removePatient(const std::string& patientID);
    Patient* findPatient(const std::string& patientID);

    // ----- Equipment management -----
    // Takes ownership of the raw pointer (moves into unique_ptr)
    bool addEquipment(HospitalResource* resource);
    bool removeEquipment(const std::string& equipmentID);
    HospitalResource* findEquipment(const std::string& equipmentID);

    // ----- Allocation -----
    bool allocateEquipment(const std::string& patientID,
                           const std::string& equipmentID);
    bool releaseEquipment(const std::string& patientID,
                          const std::string& equipmentID);

    // ----- Reports -----
    void reportEquipmentAvailability()  const;
    void reportPatientAllocation()      const;
    void reportMaintenanceDue()         const;
    void reportCostSummary()            const;

    // ----- Polymorphism demo -----
    // Calls each equipment's virtual operate() through base pointer
    void demonstratePolymorphism() const;

    // ----- Ventilator pointer demo (Requirement 10) -----
    void demonstrateVentilatorPointer() const;

    // ----- this-pointer chaining demo (Requirement 11) -----
    void demonstrateMethodChaining();

    // ----- Operator overloading demo (Requirement 11) -----
    void demonstrateOperatorOverloading();

    // ----- Seed with demo data -----
    void seedDemoData();
};
