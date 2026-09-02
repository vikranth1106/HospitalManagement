// ============================================================
// PatientMonitor.h
//
// Concrete class derived from HospitalResource.
// Represents a bedside patient-monitoring device.
//
// Inheritance role: HIERARCHICAL (child of HospitalResource)
// Provides concrete implementations of all pure virtual
// functions declared in HospitalResource.
// ============================================================

#pragma once

#include "HospitalResource.h"
#include <string>

// ============================================================
// PatientMonitor
// Monitors patient vital signs: heart rate, SpO2, blood pressure.
// ============================================================
class PatientMonitor : public virtual ResourceIdentity,
                       public HospitalResource {
private:
    double heartRateBPM;      // Current heart rate (beats per minute)
    double spO2Percent;       // Oxygen saturation percentage
    double systolicBP;        // Systolic blood pressure (mmHg)
    double diastolicBP;       // Diastolic blood pressure (mmHg)
    double temperatureCelsius;// Patient body temperature
    bool   alarmEnabled;      // Whether audible alarms are active
    int    refreshRateSeconds;// Measurement refresh rate

    // Ward compatibility list for this device
    std::vector<std::string> compatibleWards;

public:
    // Default constructor
    PatientMonitor();

    // Parameterised constructor
    PatientMonitor(const std::string& id,
                   const std::string& name,
                   const std::string& mfr,
                   const std::string& ward,
                   double dailyCost);

    // Virtual destructor
    virtual ~PatientMonitor() = default;

    // ---- Pure virtual overrides (Requirement 3) ----
    void        operate()           override;  // Start monitoring cycle
    void        runDiagnostics()    override;  // Check sensor accuracy
    std::string getEquipmentType()  const override;
    void        generateReport()    const override;

    // ---- PatientMonitor-specific functions ----
    void setVitals(double hr, double spo2, double sys, double dia, double temp);
    void triggerAlarm(const std::string& reason);
    void setAlarmEnabled(bool enabled);
    void addCompatibleWard(const std::string& w);
    bool isCompatibleWith(const std::string& w) const;

    // ---- Accessors ----
    double getHeartRateBPM()       const;
    double getSpO2Percent()        const;
    double getSystolicBP()         const;
    double getDiastolicBP()        const;
    double getTemperatureCelsius() const;
    bool   getAlarmEnabled()       const;

    // Override display to include vitals
    void display() const override;
};
