// ============================================================
// InfusionPump.h
//
// Concrete class derived from HospitalResource.
// Represents an IV infusion pump for delivering fluids/drugs.
//
// Inheritance role: HIERARCHICAL (child of HospitalResource)
// Also serves as a base for HybridCriticalCareDevice
// (MULTILEVEL inheritance).
// ============================================================

#pragma once

#include "HospitalResource.h"
#include <string>

// ============================================================
// InfusionPump
// Controls IV fluid/drug delivery rate and dosage.
// ============================================================
class InfusionPump : public virtual ResourceIdentity,
                     public virtual HospitalResource {
private:
    double flowRateMlPerHour;   // Current infusion rate (mL/hr)
    double volumeRemainingMl;   // Volume of medication remaining
    double totalVolumeToInfuseMl; // Prescribed total volume
    std::string medicationName; // Name of drug/fluid being infused
    std::string medicationType; // "Antibiotic", "Analgesic", "Saline", etc.
    bool   isRunning;           // Pump actively delivering?
    bool   occlusionAlarm;      // True if tube is blocked
    int    channelCount;        // Number of independent infusion channels

public:
    // Default constructor
    InfusionPump();

    // Parameterised constructor
    InfusionPump(const std::string& id,
                 const std::string& name,
                 const std::string& mfr,
                 const std::string& ward,
                 double dailyCost);

    // Virtual destructor
    virtual ~InfusionPump() = default;

    // ---- Pure virtual overrides ----
    void        operate()          override; // Start infusion cycle
    void        runDiagnostics()   override; // Check line, flow rate
    std::string getEquipmentType() const override;
    void        generateReport()   const override;

    // ---- InfusionPump-specific functions ----
    void setMedication(const std::string& medName,
                       const std::string& medType,
                       double totalVolumeMl,
                       double flowRate);
    void startInfusion();
    void stopInfusion();
    void clearOcclusionAlarm();
    bool isOcclusionDetected() const;
    double getRemainingTimeHours() const;  // Estimated time to completion

    // ---- Accessors ----
    double      getFlowRateMlPerHour()     const;
    double      getVolumeRemainingMl()     const;
    double      getTotalVolumeToInfuseMl() const;
    std::string getMedicationName()        const;
    std::string getMedicationType()        const;
    bool        getIsRunning()             const;
    int         getChannelCount()          const;

    // Override display
    void display() const override;
};
