// ============================================================
// ResourceIdentity.h
//
// Virtual base class used to avoid the "diamond problem".
// Both HospitalResource and any sibling branch inherit from
// ResourceIdentity using the 'virtual' keyword, so
// HybridCriticalCareDevice receives exactly ONE copy of the
// identity data members.
//
// Inheritance role: VIRTUAL BASE CLASS
// ============================================================

#pragma once

#include <string>
#include <iostream>

// ============================================================
// ResourceIdentity
// Holds the unique identity of any hospital resource.
// Declared as a virtual base to prevent duplicate sub-objects
// in multiple-inheritance scenarios.
// ============================================================
class ResourceIdentity {
protected:
    std::string resourceID;    // Unique identifier (e.g., "EQ-001")
    std::string resourceName;  // Human-readable name
    std::string manufacturer;  // Equipment manufacturer

public:
    // Default constructor
    ResourceIdentity();

    // Parameterised constructor
    ResourceIdentity(const std::string& id,
                     const std::string& name,
                     const std::string& mfr);

    // Virtual destructor – required because we use polymorphism
    virtual ~ResourceIdentity() = default;

    // ----- Accessors -----
    std::string getResourceID()   const;
    std::string getResourceName() const;
    std::string getManufacturer() const;

    // ----- Mutators -----
    void setResourceID(const std::string& id);
    void setResourceName(const std::string& name);
    void setManufacturer(const std::string& mfr);

    // Display identity info
    virtual void displayIdentity() const;
};
