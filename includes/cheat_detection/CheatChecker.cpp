#include "CheatChecker.hpp"
#include "game_logic/Constants.hpp"
#include <iostream>

// Constructor with initialization of last points
CheatChecker::CheatChecker(Base* playerBase, Base* npcBase, std::vector<Unit>* playerUnits, std::vector<Unit>* npcUnits)
    : playerBase(playerBase), npcBase(npcBase), playerUnits(playerUnits), npcUnits(npcUnits),
      lastPlayerPoints(playerBase->points), lastNpcPoints(npcBase->points) {}

// Main function to monitor for cheats
void CheatChecker::MonitorForCheats() {
    CheckHealthValues();
    CheckPoints();
    VerifyUnitDataIntegrity();
    VerifyBaseDataIntegrity();
}

// Function to check if health values are within expected range
void CheatChecker::CheckHealthValues() {
    // Check base health values using defined macros
    if (playerBase->health > PLAYER_BASE_HEALTH || playerBase->health < 0) {
        std::cerr << "Cheat detected: Player base health value is out of bounds!" << std::endl;
    }
    if (npcBase->health > NPC_BASE_HEALTH || npcBase->health < 0) {
        std::cerr << "Cheat detected: NPC base health value is out of bounds!" << std::endl;
    }

    // Check health values of all player and NPC units
    for (const auto& unit : *playerUnits) {
        VerifyUnitHealth(unit, true);
    }

    for (const auto& unit : *npcUnits) {
        VerifyUnitHealth(unit, false);
    }
}

// Helper function to check unit health values
void CheatChecker::VerifyUnitHealth(const Unit& unit, bool isPlayerUnit) {
    // Define max health for each type
    int maxHealth = 0;
    switch (unit.type) {
        case TYPE_1_SOLDIER: maxHealth = 10; break;
        case TYPE_2_SOLDIER: maxHealth = 20; break;
        case TYPE_3_SOLDIER: maxHealth = 30; break;
        case TANK: maxHealth = 50; break;
        case BIT: maxHealth = 5; break;  // Assuming 5 as max health for BIT units
        default: maxHealth = 10;  // Default to 10 if type is unknown
    }

    if (unit.health < 0 || unit.health > maxHealth) {
        std::cerr << "Cheat detected: " << (isPlayerUnit ? "Player" : "NPC") << " unit health value is out of bounds!" << std::endl;
    }
}

// Function to check if points are being modified illegally
void CheatChecker::CheckPoints() {
    const int maxPointsChangePerTick = 10;  // Define the maximum allowed points change per tick

    // Check for illegal point modifications
    int playerPointsChange = playerBase->points - lastPlayerPoints;
    int npcPointsChange = npcBase->points - lastNpcPoints;

    if (playerBase->points < 0) {
        std::cerr << "Cheat detected: Player points cannot be negative!" << std::endl;
    }
    if (npcBase->points < 0) {
        std::cerr << "Cheat detected: NPC points cannot be negative!" << std::endl;
    }
    if (abs(playerPointsChange) > maxPointsChangePerTick) {
        std::cerr << "Cheat detected: Player points changed by an illegal amount!" << std::endl;
    }
    if (abs(npcPointsChange) > maxPointsChangePerTick) {
        std::cerr << "Cheat detected: NPC points changed by an illegal amount!" << std::endl;
    }

    // Update last points for next check
    lastPlayerPoints = playerBase->points;
    lastNpcPoints = npcBase->points;
}

// Function to verify that unit data is intact and has not been tampered with
void CheatChecker::VerifyUnitDataIntegrity() {
    for (const auto& unit : *playerUnits) {
        VerifySingleUnitIntegrity(unit, true);
    }

    for (const auto& unit : *npcUnits) {
        VerifySingleUnitIntegrity(unit, false);
    }
}

// Helper function to verify a single unit's integrity
void CheatChecker::VerifySingleUnitIntegrity(const Unit& unit, bool isPlayerUnit) {
    // Validate hitbox dimensions
    if (unit.hitbox.width <= 0 || unit.hitbox.height <= 0) {
        std::cerr << "Cheat detected: Invalid " << (isPlayerUnit ? "player" : "NPC") << " unit hitbox dimensions!" << std::endl;
    }

    // Validate health, damage, speed, type, and position
    if (unit.health < 0 || unit.health > 100) { // Assuming 100 is the max health for all units
        std::cerr << "Cheat detected: Invalid " << (isPlayerUnit ? "player" : "NPC") << " unit health value!" << std::endl;
    }
    if (unit.damage < 0 || unit.damage > 50) { // Assuming 50 is the max damage
        std::cerr << "Cheat detected: Invalid " << (isPlayerUnit ? "player" : "NPC") << " unit damage value!" << std::endl;
    }
    if (unit.speed < 0 || unit.speed > static_cast<int>(UNIT_SPEED)) { // Ensure speed is within limits
        std::cerr << "Cheat detected: Invalid " << (isPlayerUnit ? "player" : "NPC") << " unit speed value!" << std::endl;
    }
    if (unit.type < TYPE_1_SOLDIER || unit.type > BIT) { // Verify that the unit type is within the defined enum range
        std::cerr << "Cheat detected: Invalid " << (isPlayerUnit ? "player" : "NPC") << " unit type!" << std::endl;
    }
    if (unit.hitbox.x < 0 || unit.hitbox.y < 0) {
        std::cerr << "Cheat detected: Invalid " << (isPlayerUnit ? "player" : "NPC") << " unit hitbox position!" << std::endl;
    }

    // Ownership validation
    if (isPlayerUnit && !unit.isPlayerUnit) {
        std::cerr << "Cheat detected: Unit ownership mismatch for player unit!" << std::endl;
    }
    if (!isPlayerUnit && unit.isPlayerUnit) {
        std::cerr << "Cheat detected: Unit ownership mismatch for NPC unit!" << std::endl;
    }
}

// Function to verify that base data is intact and has not been tampered with
void CheatChecker::VerifyBaseDataIntegrity() {
    // Validate player base hitbox using defined hitbox dimensions
    if (playerBase->hitbox.x < 0 || playerBase->hitbox.y < 0 || playerBase->hitbox.width <= 0 || playerBase->hitbox.height <= 0) {
        std::cerr << "Cheat detected: Invalid player base hitbox!" << std::endl;
    }
    if (npcBase->hitbox.x < 0 || npcBase->hitbox.y < 0 || npcBase->hitbox.width <= 0 || npcBase->hitbox.height <= 0) {
        std::cerr << "Cheat detected: Invalid NPC base hitbox!" << std::endl;
    }
}
