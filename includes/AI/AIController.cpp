#include "AIController.hpp"

 // NPC logic for buying units based on player's actions and available points
void npc_logic_update(std::vector<Unit>& npc_units, Base& npc_base, std::vector<Unit>& player_units,  bool isOffensive) {
    if (isOffensive) {
        // Offensive mode: NPC spawns more aggressive units
        if (player_units.size() > npc_units.size() && npc_base.points >= 50) {  // If player has more units, spawn tanks
            SpawnUnit(npc_units, npc_base, TANK, false);
            npc_base.points -= 50;
        } else if (player_units.size() > npc_units.size() && npc_base.points >= 30) {  // If player has more units, spawn Type 3 soldiers
            SpawnUnit(npc_units, npc_base, TYPE_3_SOLDIER, false);
            npc_base.points -= 30;
        } else if (npc_base.points >= 10) {  // Default to spawning Type 2 soldiers
            SpawnUnit(npc_units, npc_base, TYPE_2_SOLDIER, false);
            npc_base.points -= 10;
        } else if (npc_base.points >= 5) {  // Default to spawning Type 1 soldiers
            SpawnUnit(npc_units, npc_base, TYPE_1_SOLDIER, false);
            npc_base.points -= 5;
        } else if (npc_base.points >= 2) {  // Default to spawning Bits
            SpawnUnit(npc_units, npc_base, BIT, false);
            npc_base.points -= 2;
        }
    } else {
        // Defensive mode: NPC spawns units based on player's unit types
        if (std::count_if(player_units.begin(), player_units.end(), [](const Unit& unit) { return unit.type == TANK; }) > 0 && npc_base.points >= 50) {  // If player has tanks, spawn tanks
            SpawnUnit(npc_units, npc_base, TANK, false);
            npc_base.points -= 50;
        } else if (std::count_if(player_units.begin(), player_units.end(), [](const Unit& unit) { return unit.type == TYPE_3_SOLDIER; }) > 0 && npc_base.points >= 30) {  // If player has Type 3 soldiers, spawn Type 3 soldiers
            SpawnUnit(npc_units, npc_base, TYPE_3_SOLDIER, false);
            npc_base.points -= 30;
        } else if (npc_base.points >= 10) {  // Default to spawning Type 2 soldiers
            SpawnUnit(npc_units, npc_base, TYPE_2_SOLDIER, false);
            npc_base.points -= 10;
        } else if (npc_base.points >= 5) {  // Default to spawning Type 1 soldiers
            SpawnUnit(npc_units, npc_base, TYPE_1_SOLDIER, false);
            npc_base.points -= 5;
        } else if (npc_base.points >= 2) {  // Default to spawning Bits
            SpawnUnit(npc_units, npc_base, BIT, false);
            npc_base.points -= 2;
        }
    }
}
