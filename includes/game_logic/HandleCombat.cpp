#include "HandleCombat.hpp"
#include <cmath>
#include <algorithm>

// Helper function to calculate the distance between two points
float CalculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Move unit towards the target
void HandleCombat::MoveUnitTowards(Unit& unit, const Vector2& target) {
    // Calculate the direction vector
    float dx = target.x - unit.hitbox.x;
    float distance = CalculateDistance(unit.hitbox.x, unit.hitbox.y, target.x, target.y);
    
    // Normalize the direction
    if (distance > 0) {
        dx /= distance;
    }

    // Move the unit
    unit.hitbox.x += dx * UNIT_SPEED;
}

void HandleCombat::HandleGameCombat(std::vector<Unit>& playerUnits, std::vector<Unit>& npcUnits, Base& playerBase, Base& npcBase) {
    // Move player units towards NPC base
    for (auto& playerUnit : playerUnits) {
        MoveUnitTowards(playerUnit, {npcBase.hitbox.x, npcBase.hitbox.y});
    }

    // Move NPC units towards the nearest player unit or player base
    for (auto& npcUnit : npcUnits) {
        if (!playerUnits.empty()) {
            // Find the nearest player unit
            Unit* nearestPlayerUnit = nullptr;
            float minDistance = std::numeric_limits<float>::max();
            for (auto& playerUnit : playerUnits) {
                float distance = CalculateDistance(npcUnit.hitbox.x, npcUnit.hitbox.y, playerUnit.hitbox.x, playerUnit.hitbox.y);
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestPlayerUnit = &playerUnit;
                }
            }
            if (nearestPlayerUnit) {
                MoveUnitTowards(npcUnit, {nearestPlayerUnit->hitbox.x, nearestPlayerUnit->hitbox.y});
            }
        } else {
            MoveUnitTowards(npcUnit, {playerBase.hitbox.x, playerBase.hitbox.y});
        }
    }

    // Check for collisions and apply damage
    for (auto& playerUnit : playerUnits) {
        for (auto& npcUnit : npcUnits) {
            if (CheckCollisionRecs(playerUnit.hitbox, npcUnit.hitbox)) {
                // Both units take damage
                playerUnit.health -= npcUnit.damage;
                npcUnit.health -= playerUnit.damage;

                // Award points if a unit is killed
                if (playerUnit.health <= 0) {
                    npcBase.points += 10;  // NPC earns points for killing a player unit
                }
                if (npcUnit.health <= 0) {
                    playerBase.points += 10;  // Player earns points for killing an NPC unit
                }
            }
        }
    }

    // Remove dead player units
    playerUnits.erase(std::remove_if(playerUnits.begin(), playerUnits.end(),
                                     [](const Unit& unit) { return unit.health <= 0; }),
                      playerUnits.end());

    // Remove dead NPC units
    npcUnits.erase(std::remove_if(npcUnits.begin(), npcUnits.end(),
                                  [](const Unit& unit) { return unit.health <= 0; }),
                   npcUnits.end());
}