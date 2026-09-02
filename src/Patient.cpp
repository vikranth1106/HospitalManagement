// ============================================================
// Patient.cpp
// Implementation of the Patient class.
// ============================================================

#include "Patient.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

// Initialise static ID counter
int Patient::idCounter = 1;

// ============================================================
// Helper free functions
// ============================================================

std::string priorityToString(ClinicalPriority p) {
    switch (p) {
        case ClinicalPriority::LOW:      return "Low";
        case ClinicalPriority::MEDIUM:   return "Medium";
        case ClinicalPriority::HIGH:     return "High";
        case ClinicalPriority::CRITICAL: return "CRITICAL";
        default:                         return "Unknown";
    }
}

std::string riskToString(RiskCategory r) {
    switch (r) {
        case RiskCategory::STABLE:   return "Stable";
        case RiskCategory::MODERATE: return "Moderate";
        case RiskCategory::HIGH:     return "High";
        case RiskCategory::EXTREME:  return "EXTREME";
        default:                     return "Unknown";
    }
}

// ============================================================
// Static ID generator
// ============================================================
std::string Patient::generateID() {
    std::ostringstream oss;
    oss << "PT-" << std::setw(3) << std::setfill('0') << idCounter++;
    return oss.str();
}

// ============================================================
// Constructors
// ============================================================

Patient::Patient()
    : patientID(generateID()),
      name("Unknown"),
      age(0),
      ward("General"),
      clinicalPriority(ClinicalPriority::LOW),
      riskCategory(RiskCategory::STABLE),
      estimatedTreatmentCostUSD(0.0) {}

Patient::Patient(const std::string& id,
                 const std::string& n,
                 int a,
                 const std::string& w,
                 ClinicalPriority priority,
                 RiskCategory risk)
    : patientID(id),
      name(n),
      age(a),
      ward(w),
      clinicalPriority(priority),
      riskCategory(risk),
      estimatedTreatmentCostUSD(0.0) {}

// ============================================================
// Equipment assignment
// ============================================================

bool Patient::assignEquipment(const std::string& equipmentID) {
    // Prevent duplicate allocation
    if (hasEquipment(equipmentID)) return false;
    assignedEquipmentIDs.push_back(equipmentID);
    return true;
}

bool Patient::releaseEquipment(const std::string& equipmentID) {
    auto it = std::find(assignedEquipmentIDs.begin(),
                        assignedEquipmentIDs.end(),
                        equipmentID);
    if (it == assignedEquipmentIDs.end()) return false;
    assignedEquipmentIDs.erase(it);
    return true;
}

bool Patient::hasEquipment(const std::string& equipmentID) const {
    return std::find(assignedEquipmentIDs.begin(),
                     assignedEquipmentIDs.end(),
                     equipmentID) != assignedEquipmentIDs.end();
}

bool Patient::hasAnyEquipment() const {
    return !assignedEquipmentIDs.empty();
}

// ============================================================
// Cost management
// ============================================================

void Patient::addTreatmentCost(double amount) {
    if (amount > 0.0) estimatedTreatmentCostUSD += amount;
}

// ============================================================
// Accessors
// ============================================================
std::string      Patient::getPatientID()                 const { return patientID; }
std::string      Patient::getName()                      const { return name; }
int              Patient::getAge()                       const { return age; }
std::string      Patient::getWard()                      const { return ward; }
ClinicalPriority Patient::getClinicalPriority()          const { return clinicalPriority; }
RiskCategory     Patient::getRiskCategory()              const { return riskCategory; }
double           Patient::getEstimatedTreatmentCostUSD() const { return estimatedTreatmentCostUSD; }
int              Patient::getAssignedEquipmentCount()    const { return static_cast<int>(assignedEquipmentIDs.size()); }

const std::vector<std::string>& Patient::getAssignedEquipmentIDs() const {
    return assignedEquipmentIDs;
}

// ============================================================
// Mutators
// ============================================================
void Patient::setName(const std::string& n)              { name             = n; }
void Patient::setAge(int a)                              { age              = (a >= 0 && a <= 150) ? a : 0; }
void Patient::setWard(const std::string& w)              { ward             = w; }
void Patient::setClinicalPriority(ClinicalPriority p)    { clinicalPriority = p; }
void Patient::setRiskCategory(RiskCategory r)            { riskCategory     = r; }
void Patient::setEstimatedTreatmentCostUSD(double cost)  { estimatedTreatmentCostUSD = (cost >= 0) ? cost : 0; }

// ============================================================
// Display
// ============================================================
void Patient::display() const {
    std::cout << "\n╔══════════════════════════════════════════╗\n";
    std::cout << "║           PATIENT RECORD                 ║\n";
    std::cout << "╠══════════════════════════════════════════╣\n";
    std::cout << "║ Patient ID  : " << std::left << std::setw(27) << patientID   << "║\n";
    std::cout << "║ Name        : " << std::left << std::setw(27) << name        << "║\n";
    std::cout << "║ Age         : " << std::left << std::setw(27) << age         << "║\n";
    std::cout << "║ Ward        : " << std::left << std::setw(27) << ward        << "║\n";
    std::cout << "║ Priority    : " << std::left << std::setw(27)
              << priorityToString(clinicalPriority) << "║\n";
    std::cout << "║ Risk        : " << std::left << std::setw(27)
              << riskToString(riskCategory)         << "║\n";
    std::cout << "║ Est. Cost   : $" << std::left << std::setw(26)
              << std::fixed << std::setprecision(2)
              << estimatedTreatmentCostUSD << "║\n";
    std::cout << "║ Equipment   : " << std::left << std::setw(27)
              << assignedEquipmentIDs.size() << "║\n";
    if (!assignedEquipmentIDs.empty()) {
        for (const auto& eqID : assignedEquipmentIDs) {
            std::cout << "║   -> " << std::left << std::setw(36) << eqID << "║\n";
        }
    }
    std::cout << "╚══════════════════════════════════════════╝\n";
}
