# Smart Hospital Patient & Medical Equipment Management System
## Complete Project Documentation

---

## 1. Pseudocode

```
PROGRAM SmartHospitalManagement

  CLASS ResourceIdentity:
      FIELDS: resourceID, resourceName, manufacturer
      METHODS: getters, setters, displayIdentity()

  CLASS HospitalResource EXTENDS ResourceIdentity (virtual):
      FIELDS: ward, operatingStatus, isAvailable, dailyCostUSD,
              usageDurationHours, battery, calibration, maintenance
      PURE VIRTUAL: operate(), runDiagnostics(), getEquipmentType(), generateReport()
      VIRTUAL: display()

  CLASS PatientMonitor EXTENDS HospitalResource:
      FIELDS: heartRateBPM, spO2, bloodPressure, temperature, alarm
      OVERRIDE: operate(), runDiagnostics(), getEquipmentType(), generateReport()

  CLASS InfusionPump EXTENDS HospitalResource:
      FIELDS: flowRate, volumeRemaining, medication, isRunning, occlusionAlarm
      OVERRIDE: operate(), runDiagnostics(), getEquipmentType(), generateReport()

  CLASS Ventilator EXTENDS HospitalResource:
      FIELDS: ventMode, tidalVolume, respiratoryRate, FiO2, PEEP
      OVERRIDE: operate(), runDiagnostics(), getEquipmentType(), generateReport()
      SPECIFIC: startVentilation(), stopVentilation(), runSBT()

  CLASS HybridCriticalCareDevice EXTENDS Ventilator, InfusionPump:
      CALLS ResourceIdentity constructor directly (diamond fix)
      FIELDS: deviceProfile, synchronisedMode, icuBedNumber
      OVERRIDE: operate(), runDiagnostics(), getEquipmentType(), generateReport()

  CLASS MedicalEquipment:
      FIELDS: id, name, type, ward, cost, battery, calibration, maintenance
              serviceHistory* (dynamic array)
      CONSTRUCTOR 1: Default
      CONSTRUCTOR 2: Core fields
      CONSTRUCTOR 3: All fields + modules
      CONSTRUCTOR 4 (COPY): Deep copy serviceHistory pointer
      DESTRUCTOR: delete[] serviceHistory
      operator+: return new object with summed usageHours and cost
      operator<: compare suitability scores
      operator==: compare by ID
      operator<<: print formatted report
      updateBattery(x).updateCost(x).updateWard(x): return *this

  CLASS Patient:
      FIELDS: patientID, name, age, ward, priority, risk, cost,
              assignedEquipmentIDs (vector<string>)
      METHODS: assignEquipment(), releaseEquipment(), hasEquipment()

  CLASS Hospital:
      FIELDS: patients (vector<Patient>),
              equipment (vector<unique_ptr<HospitalResource>>)
      METHODS:
        addPatient(), removePatient(), findPatient()
        addEquipment(), removeEquipment(), findEquipment()
        allocateEquipment(patientID, equipID):
          IF equipment not found -> ERROR
          IF patient not found -> ERROR
          IF already assigned -> PREVENT DUPLICATE
          IF battery <= 20% -> REJECT
          IF not calibrated -> REJECT
          IF status != Active -> REJECT
          IF ward mismatch -> REJECT
          ELSE -> mark isAvailable=false, assign to patient
        releaseEquipment()
        reportEquipmentAvailability()
        reportPatientAllocation()
        reportMaintenanceDue()
        reportCostSummary()
        run() -> main menu loop

  MAIN:
    CREATE hospital object
    CALL hospital.seedDemoData()
    CALL hospital.run()
    HANDLE exceptions (bad_alloc, std::exception)
    RETURN 0
```

---

## 2. UML Class Diagram (ASCII)

```
+-----------------------------+
|      <<Virtual Base>>       |
|      ResourceIdentity       |
+-----------------------------+
| # resourceID: string        |
| # resourceName: string      |
| # manufacturer: string      |
+-----------------------------+
| + getResourceID(): string   |
| + displayIdentity(): void   |
| + ~ResourceIdentity()       |
+------------+----------------+
             |  <<virtual>>
             |
+------------v----------------+
|   <<Abstract>>              |
|   HospitalResource          |
+-----------------------------+
| # ward: string              |
| # operatingStatus: string   |
| # isAvailable: bool         |
| # dailyCostUSD: double      |
| # battery: BatteryModule    |<--- HAS-A (Composition)
| # calibration: Calibration  |
| # maintenance: Maintenance  |
+-----------------------------+
| + operate() = 0             |  <- pure virtual
| + runDiagnostics() = 0      |  <- pure virtual
| + getEquipmentType() = 0    |  <- pure virtual
| + generateReport() = 0      |  <- pure virtual
| + display(): virtual        |
+--+----------+---------------+
   |          |          |
   |   Hierarchical inheritance
   |          |          |
+--v------+ +-v------+ +-v-------------------+
|Patient  | |Infusion| |    Ventilator        |
|Monitor  | |Pump    | +---------------------+
+---------+ +--------+ | ventMode            |
| HR, SpO2| |flowRate| | tidalVolume         |
| BP, Temp| |volume  | | FiO2, PEEP          |
| alarm   | |medName | +---------------------+
+---------+ |running | |+startVentilation()  |
|+operate()| +--------+ |+stopVentilation()  |
|+diagnose| |+operate| |+runSBT()            |
+---------+ |+diagnose| +----------+---------+
             +----+---+             | Multilevel
                  |                 |
                  |  Multiple Inheritance
                  +--------+--------+
                           |
             +-------------v------------------+
             |   HybridCriticalCareDevice      |
             +---------------------------------+
             | - deviceProfile: string         |
             | - synchronisedMode: bool        |
             | - icuBedNumber: int             |
             +---------------------------------+
             | + operate()  (resolves ambiguity)|
             | + runCombinedProtocol()         |
             | + activateSynchronisedMode()    |
             +---------------------------------+

Standalone Classes:

+----------------------------------+
|         MedicalEquipment         |
+----------------------------------+
| - equipmentID, name, type        |
| - ward, manufacturer             |
| - dailyCostUSD, usageHours       |
| - battery: BatteryModule         | <- HAS-A
| - calibration: CalibrationModule | <- HAS-A
| - maintenance: MaintenanceModule | <- HAS-A
| - serviceHistory*: string[]      | <- Dynamic (deep copy)
+----------------------------------+
| + MedicalEquipment()             | Default
| + MedicalEquipment(id,name,...)  | Parameterised
| + MedicalEquipment(...,bat,cal,..)| Overloaded
| + MedicalEquipment(other)        | Copy (deep)
| + operator=(other)               | Deep copy
| + ~MedicalEquipment()            | delete[] history
| + operator+(other): ME           | Combine stats
| + operator<(other): bool         | Suitability
| + operator==(other): bool        | ID equality
| + operator<<(os, eq)             | Report print
| + updateBattery(x): ME&          | Chainable
| + updateCost(x): ME&             | Chainable
| + updateWard(x): ME&             | Chainable
| + computeSuitabilityScore(): int |
+----------------------------------+

+----------------------------------+
|           Patient                |
+----------------------------------+
| - patientID, name, age, ward     |
| - clinicalPriority: enum         |
| - riskCategory: enum             |
| - estimatedCostUSD: double       |
| - assignedEquipmentIDs: vector   |
+----------------------------------+
| + assignEquipment(id): bool      |
| + releaseEquipment(id): bool     |
| + hasEquipment(id): bool         |
| + addTreatmentCost(x)            |
| + display()                      |
+----------------------------------+
```

---

## 3. Sample Execution Output

```
+----------------------------------------------------------+
|    SMART HOSPITAL PATIENT & EQUIPMENT MANAGEMENT SYSTEM  |
|    City Central Hospital                                  |
|    12 Medical Plaza, Hyderabad, India                     |
+----------------------------------------------------------+

SEEDING DEMONSTRATION DATA
  Patient Arjun Sharma (PT-001) added.
  Patient Priya Nair (PT-002) added.
  ...
  Equipment Bedside Monitor Alpha (PM-001) added.
  Equipment Ventilator Prime (VT-001) added.
  Equipment HybridCare Elite (HB-001) added.

  Performing initial allocations...
  SUCCESS: Bedside Monitor Alpha allocated to Arjun Sharma.
  SUCCESS: Ventilator Prime allocated to Arjun Sharma.
  SUCCESS: Infusion Pump Gamma allocated to Arjun Sharma.
  SUCCESS: Bedside Monitor Beta allocated to Priya Nair.
  SUCCESS: Emergency Monitor allocated to Mohan Reddy.

  Enter choice (1-12): 8

EQUIPMENT AVAILABILITY REPORT
ID        Name                     Type            Ward        Available Battery  Status
-----------------------------------------------------------------------------------------------
PM-001    Bedside Monitor Alpha    Patient Monitor  ICU         No        92.0%   Active
PM-002    Bedside Monitor Beta     Patient Monitor  General     No        75.0%   Active
IP-001    Infusion Pump Gamma      Infusion Pump    ICU         No        88.0%   Active
IP-002    Infusion Pump Delta      Infusion Pump    General     Yes       15.0%   Active
VT-001    Ventilator Prime         Ventilator       ICU         No        95.0%   Active
VT-002    Ventilator Apex          Ventilator       Emergency   Yes       70.0%   Active
HB-001    HybridCare Elite         Hybrid CCD       ICU         Yes       90.0%   Active
```

---

## 4. Test Cases

### 4.1 Normal Test Case

| Scenario | Input | Expected Output |
|---|---|---|
| Add patient | PT-010, Kavya Iyer, 35, ICU, HIGH, HIGH | "Patient Kavya Iyer (PT-010) added." |
| Add ventilator | VT-010, Ventilator X, DragerVent, ICU, 500 | "Equipment Ventilator X (VT-010) added." |
| Allocate | PT-010 -> VT-010 | "SUCCESS: Ventilator X allocated to Kavya Iyer." |
| Release | PT-010 -> VT-010 | "SUCCESS: Ventilator X released from Kavya Iyer." |
| Cost report | After allocation with dailyCost=500 | Patient shows $500.00 treatment cost |
| operator+ | EQ1 (48h) + EQ2 (120h) | Combined = 168 hours |
| Deep copy | Copy EQ1, add record to copy | Original unchanged (separate heap) |

### 4.2 Boundary Test Cases

| Scenario | Input | Expected Behaviour |
|---|---|---|
| Min age | Age = 0 | Accepted (newborn) |
| Max age | Age = 150 | Accepted |
| Battery just above threshold | Battery = 20.1% | Allocation allowed |
| Battery at threshold | Battery = 20.0% | Allocation REJECTED |
| Empty patient list | Display patients, 0 records | "No patients registered." |
| Full history array | Add 11th record (capacity=10) | Array doubles, record added |
| Zero daily cost | Equipment with $0.00/day | Cost report shows $0.00 |

### 4.3 Invalid Test Cases

| Scenario | Input | Expected Output |
|---|---|---|
| Duplicate patient ID | Add PT-001 twice | "ERROR: Patient ID PT-001 already exists." |
| Duplicate equipment ID | Add PM-001 twice | "ERROR: Equipment ID PM-001 already exists." |
| Non-existent patient | PT-999 -> PM-001 | "ERROR: Patient PT-999 not found." |
| Non-existent equipment | PT-001 -> XX-999 | "ERROR: Equipment XX-999 not found." |
| Duplicate allocation | Assign PM-001 to PT-001 twice | "ERROR: Equipment already assigned to patient." |
| Ward mismatch | ICU patient <- General ward device | "REJECT: Ward mismatch." |
| Uncalibrated device | Allocate uncalibrated device | "REJECT: Equipment not calibrated." |
| Low battery allocation | Battery = 15% | "REJECT: Battery too low (15.0%)." |
| Under-maintenance | Status = Under Maintenance | "REJECT: Equipment status is Under Maintenance." |
| Menu out-of-range | Input 0 or 13 | "Invalid input. Enter 1-12." |
| Negative age | Age = -1 | Clamped to 0 |
| Negative cost | Cost = -100 | Clamped to 0 |
| Bad dynamic_cast | Ventilator* on a PatientMonitor* | Returns nullptr, no crash |

---

## 5. Reflection

### Reflection on the Smart Hospital Management System Project

This project provided a comprehensive opportunity to apply the core principles of Object-Oriented Programming in a realistic, domain-rich context. Designing a hospital management system required careful thought not only about syntax but about the real-world responsibilities each class should hold.

**Inheritance and Polymorphism** were the most intellectually stimulating aspects. The diamond problem arising from combining `InfusionPump` and `Ventilator` in `HybridCriticalCareDevice` forced me to deeply understand virtual inheritance. Without `public virtual ResourceIdentity`, the hybrid device would contain two copies of `resourceID`, causing ambiguity errors. Using `virtual` before the base class in each intermediate class resolved this elegantly - a solution that initially seems counterintuitive but is fundamental to professional C++ design.

**Composition** through the three module classes (`BatteryModule`, `CalibrationModule`, `MaintenanceModule`) taught me the power of the "Has-A" relationship over "Is-A". Embedding these modules inside every equipment object allowed shared behaviour to be reused without deep inheritance, keeping the class hierarchy clean.

**Deep Copy and Memory Safety** were reinforced through `MedicalEquipment`. Managing a raw `std::string*` array manually - allocating on construction, performing element-by-element copy in the copy constructor, and releasing in the destructor - made concrete the pitfalls of shallow copying.

**Operator Overloading** made the system more expressive. The `operator<` comparing suitability scores created a natural, readable way to rank equipment for allocation, while `operator<<` provided clean, formatted output.

**Method Chaining** via `return *this` was elegant. The pattern `equipment.updateBattery(90).updateCost(4500).updateWard("ICU")` reads like a fluent specification.

A key lesson was the importance of **virtual destructors** in every polymorphic base class. Without `virtual ~HospitalResource()`, deleting a derived object through a `HospitalResource*` would cause undefined behaviour - only the base destructor would run.

Using `std::unique_ptr` in `Hospital` eliminated memory leaks when equipment is removed. This blend of modern C++ resource management with explicit dynamic allocation in `MedicalEquipment` illustrated why RAII is a best practice.

---

## 6. SDG Relevance

### SDG 3 - Good Health and Well-Being
This system directly supports SDG 3 by improving clinical safety. The allocation engine prevents assigning faulty, uncalibrated, or low-battery devices to patients - reducing medical errors. Priority-based allocation ensures CRITICAL patients receive working equipment first. Maintenance tracking prevents mid-operation device failures.

### SDG 9 - Industry, Innovation, and Infrastructure
The system demonstrates how software infrastructure modernises hospital operations. The OOP design is extensible - new equipment types (dialysis machines, ECG units) can be added without modifying existing classes, following the Open/Closed Principle. The use of C++17, virtual polymorphism, and smart pointers reflects industry-standard practices applicable to real medical information systems.

### SDG 12 - Responsible Consumption and Production
SDG 12 is supported through maintenance and calibration tracking. By alerting administrators when service is overdue, the system prevents premature device failure and costly replacements. Battery-level monitoring ensures devices are not run to failure. These digital stewardship practices extend equipment operational lifetime - especially important in resource-limited healthcare settings.

---

## 7. Compilation Instructions

### GCC (Linux / macOS / MinGW on Windows)
```bash
g++ -std=c++17 -Wall -Wextra -I include \
    src/Modules.cpp src/ResourceIdentity.cpp src/HospitalResource.cpp \
    src/MedicalEquipment.cpp src/Patient.cpp src/PatientMonitor.cpp \
    src/InfusionPump.cpp src/Ventilator.cpp src/HybridCriticalCareDevice.cpp \
    src/Hospital.cpp src/main.cpp -o HospitalManagement
./HospitalManagement
```

### CMake (Cross-platform)
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Visual Studio 2022
1. New Project -> Empty C++ Project
2. Set C++ Language Standard to ISO C++17 (/std:c++17)
3. Add all include/*.h to Header Files
4. Add all src/*.cpp to Source Files
5. Properties -> C/C++ -> Additional Include Directories -> $(ProjectDir)include
6. Build -> Ctrl+Shift+B -> Run -> F5

---

## 8. File Summary

| File | Role | Key OOP Feature |
|---|---|---|
| `Modules.h/cpp` | BatteryModule, CalibrationModule, MaintenanceModule | Composition |
| `ResourceIdentity.h/cpp` | Virtual base class | Virtual inheritance (diamond fix) |
| `HospitalResource.h/cpp` | Abstract base class | Pure virtual functions |
| `MedicalEquipment.h/cpp` | Standalone equipment | Deep copy, operators, this-chaining |
| `Patient.h/cpp` | Patient entity | Encapsulation, enums |
| `PatientMonitor.h/cpp` | Hierarchical derived class | Virtual function override |
| `InfusionPump.h/cpp` | Hierarchical derived class | Virtual function override |
| `Ventilator.h/cpp` | Hierarchical + multilevel base | Ventilator-specific pointer demo |
| `HybridCriticalCareDevice.h/cpp` | Multiple inheritance | Diamond resolution |
| `Hospital.h/cpp` | Management engine, menu | Polymorphism, allocation logic |
| `main.cpp` | Entry point | Exception safety |
| `CMakeLists.txt` | Build system | Cross-platform compilation |
