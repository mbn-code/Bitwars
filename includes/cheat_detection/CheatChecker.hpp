#ifndef CHEATCHECKER_HPP
#define CHEATCHECKER_HPP

#include "../game_logic/GameDataStructures.hpp"  // Include the file where Base and Unit are defined
#include <vector>

// Define the CheatChecker class
class CheatChecker {
public:
    // Constructor
    CheatChecker(Base* playerBase, Base* npcBase, std::vector<Unit>* playerUnits, std::vector<Unit>* npcUnits);

    // Function to monitor for cheats
    void MonitorForCheats();

private:
    // Function to check health values of units and bases
    void CheckHealthValues();

    // Helper function to verify a single unit's health
    void VerifyUnitHealth(const Unit& unit, bool isPlayerUnit); // Add this function declaration

    // Function to check if points are being modified illegally
    void CheckPoints();

    // Function to verify that unit data is intact and has not been tampered with
    void VerifyUnitDataIntegrity();

    // Helper function to verify a single unit's data integrity
    void VerifySingleUnitIntegrity(const Unit& unit, bool isPlayerUnit); // Add this function declaration

    // Function to verify that base data is intact and has not been tampered with
    void VerifyBaseDataIntegrity();

    // Data members
    Base* playerBase;
    Base* npcBase;
    std::vector<Unit>* playerUnits;
    std::vector<Unit>* npcUnits;

    int lastPlayerPoints;  // Declare this member
    int lastNpcPoints;     // Declare this member
};

#endif // CHEATCHECKER_HPP
