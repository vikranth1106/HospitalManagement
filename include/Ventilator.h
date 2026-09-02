// ============================================================
// Ventilator.h
//
// Concrete class derived from HospitalResource.
// Represents a mechanical ventilator for respiratory support.
//
// Inheritance role:
//   - HIERARCHICAL child of HospitalResource
//   - Also a BASE for HybridCriticalCareDevice (MULTILEVEL)
// ============================================================

#pragma once

#include "HospitalResource.h"
#include <string>

// ============================================================
// VentilationMode – modes of mechanical ventilation
// ============================================================
enum class VentilationMode {
    VOLUME_CONTROL,   // Fixed tidal volume
    PRESSURE_CONTROL, // Fixed inspiratory pressure
    SIMV,             // Synchronised Intermittent Mandatory
    CPAP,             // Continuous Positive Airway Pressure
    BIPAP             // Bilevel Positive Airway Pressure
};

// Helper: convert enum to string
std::string ventModeToString(VentilationMode mode);

// ============================================================
// Ventilator
// Delivers controlled breaths to patients with respiratory failure.
// ============================================================
class Ventilator : public virtual ResourceIdentity,
                   public virtual HospitalResource {
private:
    VentilationMode ventMode;         // Current ventilation mode
    double          tidalVolumeMl;    // Tidal volume per breath (mL)
    int             respiratoryRate;  // Breaths per minute (BPM)
    double          fiO2Percent;      // Fraction of inspired O2 (21-100%)
    double          peepCmH2O;        // Positive end-expiratory pressure
    double          peakPressureCmH2O;// Peak airway pressure
    bool            isVentilating;    // Is the ventilator actively running?
    bool            highPressureAlarm;// True if peak pressure exceeded
    double          minuteVolumeL;    // tidal volume × rate / 1000

protected:
    // Ventilator-specific calculations (accessible to derived classes)
    void recalculateMinuteVolume();

public:
    // Default constructor
    Ventilator();

    // Parameterised constructor
    Ventilator(const std::string& id,
               const std::string& name,
               const std::string& mfr,
               const std::string& ward,
               double dailyCost);

    // Virtual destructor (needed as base for HybridCriticalCareDevice)
    virtual ~Ventilator() = default;

    // ---- Pure virtual overrides ----
    void        operate()           override;  // Deliver a breath cycle
    void        runDiagnostics()    override;  // Check pressures, alarms
    std::string getEquipmentType()  const override;
    void        generateReport()    const override;

    // ---- Ventilator-specific functions (Requirement 10) ----

    // Start ventilation (called via Ventilator* pointer)
    void startVentilation();

    // Stop ventilation safely
    void stopVentilation();

    // Set ventilation parameters
    void setVentilationParameters(VentilationMode mode,
                                  double tidalVol,
                                  int rate,
                                  double fio2,
                                  double peep);

    // Acknowledge and reset high-pressure alarm
    void acknowledgeAlarm();

    // Run a spontaneous breathing trial
    void runSpontaneousBreathingTrial();

    // ---- Accessors ----
    VentilationMode getVentMode()           const;
    double          getTidalVolumeMl()      const;
    int             getRespiratoryRate()    const;
    double          getFiO2Percent()        const;
    double          getPeepCmH2O()         const;
    double          getPeakPressureCmH2O() const;
    bool            getIsVentilating()      const;
    bool            getHighPressureAlarm()  const;
    double          getMinuteVolumeL()     const;

    // Override display
    void display() const override;
};
