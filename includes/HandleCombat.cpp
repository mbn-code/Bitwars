#include "HandleCombat.hpp"
#include <algorithm>  // For std::remove_if

void HandleCombat::HandleGameCombat(std::vector<Unit>& playerUnits, std::vector<Unit>& npcUnits, Base& playerBase, Base& npcBase) {
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

    // Remove dead units
    playerUnits.erase(std::remove_if(playerUnits.begin(), playerUnits.end(), [](const Unit& u) { return u.health <= 0; }), playerUnits.end());
    npcUnits.erase(std::remove_if(npcUnits.begin(), npcUnits.end(), [](const Unit& u) { return u.health <= 0; }), npcUnits.end());
}