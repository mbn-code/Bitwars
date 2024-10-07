#include "CheatChecker.hpp"
#include <iostream>

// Constructor
CheatChecker::CheatChecker(Base* playerBase, Base* npcBase, std::vector<Unit>* playerUnits, std::vector<Unit>* npcUnits)
    : playerBase(playerBase), npcBase(npcBase), playerUnits(playerUnits), npcUnits(npcUnits) {}

// Main function to monitor for cheats
void CheatChecker::MonitorForCheats() {
    CheckHealthValues();
    CheckPoints();
    VerifyUnitDataIntegrity();
    VerifyBaseDataIntegrity();
}

// Function to check if health values are within expected range
void CheatChecker::CheckHealthValues() {
    if (playerBase->health > PLAYER_BASE_HEALTH || playerBase->health < 0) {
        std::cerr << "Cheat detected: Player base health value is out of bounds!" << std::endl;
    }
    if (npcBase->health > NPC_BASE_HEALTH || npcBase->health < 0) {
        std::cerr << "Cheat detected: NPC base health value is out of bounds!" << std::endl;
    }

    // Check health values of all units
    for (const auto& unit : *playerUnits) {
        if (unit.health < 0 || unit.health >(unit.type == TYPE_1_SOLDIER ? 10 : unit.type == TYPE_2_SOLDIER ? 20 : unit.type == TYPE_3_SOLDIER ? 30 : 50)) {
            std::cerr << "Cheat detected: Player unit health value is out of bounds!" << std::endl;
        }
    }

    for (const auto& unit : *npcUnits) {
        if (unit.health < 0 || unit.health >(unit.type == TYPE_1_SOLDIER ? 10 : unit.type == TYPE_2_SOLDIER ? 20 : unit.type == TYPE_3_SOLDIER ? 30 : 50)) {
            std::cerr << "Cheat detected: NPC unit health value is out of bounds!" << std::endl;
        }
    }
}

// Function to check if points are being modified illegally
void CheatChecker::CheckPoints() {
    static int lastPlayerPoints = playerBase->points;
    static int lastNpcPoints = npcBase->points;

    if (playerBase->points < 0 || npcBase->points < 0) {
        std::cerr << "Cheat detected: Points cannot be negative!" << std::endl;
    }

    // Check for illegal point modifications
    int playerPointsChange = playerBase->points - lastPlayerPoints;
    int npcPointsChange = npcBase->points - lastNpcPoints;

    // Assuming points should only change by a small amount per tick
    const int maxPointsChangePerTick = 10;

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
        if (unit.hitbox.width <= 0 || unit.hitbox.height <= 0) {
            std::cerr << "Cheat detected: Invalid player unit hitbox dimensions!" << std::endl;
        }
        if (unit.health < 0 || unit.health > 100) { // Assuming 100 is the max health
            std::cerr << "Cheat detected: Invalid player unit health value!" << std::endl;
        }
        if (unit.damage < 0 || unit.damage > 50) { // Assuming 50 is the max damage
            std::cerr << "Cheat detected: Invalid player unit damage value!" << std::endl;
        }
        if (unit.speed < 0 || unit.speed > 10) { // Assuming 10 is the max speed
            std::cerr << "Cheat detected: Invalid player unit speed value!" << std::endl;
        }
        if (unit.type < TYPE_1_SOLDIER || unit.type > TYPE_3_SOLDIER) { // Assuming these are the valid types
            std::cerr << "Cheat detected: Invalid player unit type!" << std::endl;
        }
        if (unit.hitbox.x < 0 || unit.hitbox.y < 0) {
            std::cerr << "Cheat detected: Invalid player unit hitbox!" << std::endl;
        }
        if (unit.hitbox.x < 0 || unit.hitbox.y < 0) {
            std::cerr << "Cheat detected: Invalid player unit hitbox hitbox!" << std::endl;
        }
        if (!unit.isPlayerUnit) {
            std::cerr << "Cheat detected: Player unit ownership mismatch!" << std::endl;
        }
    }

    for (const auto& unit : *npcUnits) {
        if (unit.hitbox.width <= 0 || unit.hitbox.height <= 0) {
            std::cerr << "Cheat detected: Invalid NPC unit hitbox dimensions!" << std::endl;
        }
        if (unit.health < 0 || unit.health > 100) { // Assuming 100 is the max health
            std::cerr << "Cheat detected: Invalid NPC unit health value!" << std::endl;
        }
        if (unit.damage < 0 || unit.damage > 50) { // Assuming 50 is the max damage
            std::cerr << "Cheat detected: Invalid NPC unit damage value!" << std::endl;
        }
        if (unit.speed < 0 || unit.speed > 10) { // Assuming 10 is the max speed
            std::cerr << "Cheat detected: Invalid NPC unit speed value!" << std::endl;
        }
        if (unit.type < TYPE_1_SOLDIER || unit.type > TYPE_3_SOLDIER) { // Assuming these are the valid types
            std::cerr << "Cheat detected: Invalid NPC unit type!" << std::endl;
        }
        if (unit.hitbox.x < 0 || unit.hitbox.y < 0) {
            std::cerr << "Cheat detected: Invalid NPC unit hitbox!" << std::endl;
        }
        if (unit.hitbox.x < 0 || unit.hitbox.y < 0) {
            std::cerr << "Cheat detected: Invalid NPC unit hitbox hitbox!" << std::endl;
        }
        if (unit.isPlayerUnit) {
            std::cerr << "Cheat detected: NPC unit ownership mismatch!" << std::endl;
        }
    }
}

// Function to verify that base data is intact and has not been tampered with
void CheatChecker::VerifyBaseDataIntegrity() {
    if (playerBase->hitbox.x < 0 || playerBase->hitbox.y < 0) {
        std::cerr << "Cheat detected: Invalid player base hitbox!" << std::endl;
    }
    if (npcBase->hitbox.x < 0 || npcBase->hitbox.y < 0) {
        std::cerr << "Cheat detected: Invalid NPC base hitbox!" << std::endl;
    }
    if (playerBase->hitbox.width <= 0 || playerBase->hitbox.height <= 0) {
        std::cerr << "Cheat detected: Invalid player base hitbox dimensions!" << std::endl;
    }
    if (npcBase->hitbox.width <= 0 || npcBase->hitbox.height <= 0) {
        std::cerr << "Cheat detected: Invalid NPC base hitbox dimensions!" << std::endl;
    }
}
