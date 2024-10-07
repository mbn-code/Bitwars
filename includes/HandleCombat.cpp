#include "HandleCombat.hpp"

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

    // Remove dead player units
    for (auto it = playerUnits.begin(); it != playerUnits.end(); ) {
        if (it->health <= 0) {
            it = playerUnits.erase(it);
        } else {
            ++it;
        }
    }

    // Remove dead NPC units
    for (auto it = npcUnits.begin(); it != npcUnits.end(); ) {
        if (it->health <= 0) {
            it = npcUnits.erase(it);
        } else {
            ++it;
        }
    }
}