#ifndef HANDLE_COMBAT_HPP
#define HANDLE_COMBAT_HPP

#include <vector>
#include "GameDataStructures.hpp"

class HandleCombat {
public:
    static void HandleGameCombat(std::vector<Unit>& playerUnits, std::vector<Unit>& npcUnits, Base& playerBase, Base& npcBase);
};

#endif // HANDLE_COMBAT_HPP