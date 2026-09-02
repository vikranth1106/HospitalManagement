// ============================================================
// main.cpp
//
// Entry point for the Smart Hospital Patient and Medical
// Equipment Management System.
//
// Demonstrates:
//  - All OOP concepts (inheritance, polymorphism, composition)
//  - Deep copy, operator overloading, method chaining
//  - Runtime polymorphism via HospitalResource* pointers
//  - Safe downcast with dynamic_cast (Ventilator*)
//  - Menu-driven interface
//  - Proper memory management (unique_ptr, no leaks)
// ============================================================

#include "Hospital.h"
#include <iostream>
#include <stdexcept>
#include <windows.h>   // For SetConsoleOutputCP / SetConsoleCP

int main() {
    // ── Fix garbled Unicode box-drawing characters ──────────────
    // Switch the Windows console to UTF-8 (code page 65001) so that
    // characters like ╔, ║, ╚, ╗, ╝ render correctly.
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    // ────────────────────────────────────────────────────────────

    try {
        // Create the hospital system
        Hospital hospital("City Central Hospital",
                          "12 Medical Plaza, Hyderabad, India");

        // Populate with realistic demonstration data
        hospital.seedDemoData();

        // Launch the interactive menu loop
        hospital.run();

    } catch (const std::bad_alloc& e) {
        // Catch memory allocation failures
        std::cerr << "\n[FATAL] Memory allocation failed: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        // Catch any other standard exceptions
        std::cerr << "\n[ERROR] " << e.what() << "\n";
        return 2;
    } catch (...) {
        // Catch all other exceptions
        std::cerr << "\n[UNKNOWN ERROR] An unexpected error occurred.\n";
        return 3;
    }

    return 0;
}
