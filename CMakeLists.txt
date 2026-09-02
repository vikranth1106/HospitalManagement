cmake_minimum_required(VERSION 3.16)
project(SmartHospitalManagement CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable strict warnings for both GCC and MSVC
if (MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directory
include_directories(include)

# All source files
set(SOURCES
    src/main.cpp
    src/Modules.cpp
    src/ResourceIdentity.cpp
    src/HospitalResource.cpp
    src/MedicalEquipment.cpp
    src/Patient.cpp
    src/PatientMonitor.cpp
    src/InfusionPump.cpp
    src/Ventilator.cpp
    src/HybridCriticalCareDevice.cpp
    src/Hospital.cpp
)

add_executable(HospitalManagement ${SOURCES})

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
