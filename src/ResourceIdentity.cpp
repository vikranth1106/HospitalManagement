// ============================================================
// ResourceIdentity.cpp
// Implementation of the ResourceIdentity virtual base class.
// ============================================================

#include "ResourceIdentity.h"
#include <iostream>

ResourceIdentity::ResourceIdentity()
    : resourceID("UNSET"), resourceName("Unknown"), manufacturer("Unknown") {}

ResourceIdentity::ResourceIdentity(const std::string& id,
                                   const std::string& name,
                                   const std::string& mfr)
    : resourceID(id), resourceName(name), manufacturer(mfr) {}

std::string ResourceIdentity::getResourceID()   const { return resourceID; }
std::string ResourceIdentity::getResourceName() const { return resourceName; }
std::string ResourceIdentity::getManufacturer() const { return manufacturer; }

void ResourceIdentity::setResourceID(const std::string& id)     { resourceID   = id; }
void ResourceIdentity::setResourceName(const std::string& name) { resourceName = name; }
void ResourceIdentity::setManufacturer(const std::string& mfr)  { manufacturer = mfr; }

void ResourceIdentity::displayIdentity() const {
    std::cout << "  Resource ID   : " << resourceID   << "\n"
              << "  Resource Name : " << resourceName << "\n"
              << "  Manufacturer  : " << manufacturer << "\n";
}
