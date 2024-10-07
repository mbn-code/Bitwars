#ifndef HANDLE_COMBAT_HPP
#define HANDLE_COMBAT_HPP

#include <vector>
#include "GameDataStructures.hpp"

#define UNIT_SPEED 2.0f

void UpdateUnits(std::vector<Unit>& units, Base& opponentBase);

class HandleCombat {
public:
    static void MoveUnitTowards(Unit& unit, const Vector2& target);
    static void HandleGameCombat(std::vector<Unit>& playerUnits, std::vector<Unit>& npcUnits, Base& playerBase, Base& npcBase);
};

#endif // HANDLE_COMBAT_HPP