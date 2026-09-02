// ============================================================
// Patient.h
//
// Represents a hospital patient with clinical information,
// ward assignment, priority classification, and a list of
// currently assigned equipment resource IDs.
//
// The Patient class does NOT own equipment objects – it stores
// only the IDs of assigned equipment to prevent ownership
// conflicts and dangling pointers.
// ============================================================

#pragma once

#include <string>
#include <vector>
#include <iostream>

// ============================================================
// ClinicalPriority – enum for patient urgency
// ============================================================
enum class ClinicalPriority {
    LOW      = 1,
    MEDIUM   = 2,
    HIGH     = 3,
    CRITICAL = 4
};

// Helper: convert enum to string
std::string priorityToString(ClinicalPriority p);

// ============================================================
// RiskCategory – enum for patient risk level
// ============================================================
enum class RiskCategory {
    STABLE   = 1,
    MODERATE = 2,
    HIGH     = 3,
    EXTREME  = 4
};

// Helper: convert enum to string
std::string riskToString(RiskCategory r);

// ============================================================
// Patient
// Core patient entity with clinical metadata and equipment
// assignment tracking.
// ============================================================
class Patient {
private:
    std::string      patientID;          // Unique patient ID (e.g., "PT-001")
    std::string      name;               // Full name
    int              age;                // Age in years
    std::string      ward;               // Assigned ward (e.g., "ICU", "General")
    ClinicalPriority clinicalPriority;   // Urgency level
    RiskCategory     riskCategory;       // Risk classification
    double           estimatedTreatmentCostUSD; // Accumulated cost

    // Equipment IDs currently assigned to this patient.
    // Stored as strings (not raw pointers) to avoid ownership issues.
    std::vector<std::string> assignedEquipmentIDs;

    // Static counter for auto-generating patient IDs
    static int idCounter;

public:
    // Default constructor
    Patient();

    // Parameterised constructor
    Patient(const std::string& id,
            const std::string& name,
            int age,
            const std::string& ward,
            ClinicalPriority priority,
            RiskCategory risk);

    // Copy constructor (default memberwise copy is safe here)
    Patient(const Patient& other) = default;

    // Copy-assignment
    Patient& operator=(const Patient& other) = default;

    // Destructor (trivial – no heap memory owned)
    ~Patient() = default;

    // ----- Equipment assignment -----

    // Assign an equipment ID to this patient (returns false if already assigned)
    bool assignEquipment(const std::string& equipmentID);

    // Release a specific equipment ID
    bool releaseEquipment(const std::string& equipmentID);

    // Check if a specific equipment ID is already assigned
    bool hasEquipment(const std::string& equipmentID) const;

    // Check if patient has any equipment assigned
    bool hasAnyEquipment() const;

    // ----- Cost management -----

    // Add to the estimated treatment cost
    void addTreatmentCost(double amount);

    // ----- Accessors -----
    std::string              getPatientID()                 const;
    std::string              getName()                      const;
    int                      getAge()                       const;
    std::string              getWard()                      const;
    ClinicalPriority         getClinicalPriority()          const;
    RiskCategory             getRiskCategory()              const;
    double                   getEstimatedTreatmentCostUSD() const;
    const std::vector<std::string>& getAssignedEquipmentIDs() const;
    int                      getAssignedEquipmentCount()    const;

    // ----- Mutators -----
    void setName(const std::string& n);
    void setAge(int a);
    void setWard(const std::string& w);
    void setClinicalPriority(ClinicalPriority p);
    void setRiskCategory(RiskCategory r);
    void setEstimatedTreatmentCostUSD(double cost);

    // ----- Display -----
    void display() const;

    // Generate a unique patient ID
    static std::string generateID();
};
