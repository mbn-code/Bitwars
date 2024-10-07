#ifndef CHEAT_CHECKER_HPP
#define CHEAT_CHECKER_HPP

#include <vector>
#include "../src/main.hpp" // Include this to access game types like `Base`, `Unit`, etc.

// Define a class to handle cheat detection
class CheatChecker {
public:
    // Constructor
    CheatChecker(Base* playerBase, Base* npcBase, std::vector<Unit>* playerUnits, std::vector<Unit>* npcUnits);

    // Method to check for cheats in game state
    void MonitorForCheats();

private:
    // Reference to game elements to monitor
    Base* playerBase;
    Base* npcBase;
    std::vector<Unit>* playerUnits;
    std::vector<Unit>* npcUnits;

    // Helper functions to detect anomalies
    void CheckHealthValues();
    void CheckPoints();
    void VerifyUnitDataIntegrity();
    void VerifyBaseDataIntegrity(); // New function to verify base data integrity
};

#endif // CHEAT_CHECKER_HPP
