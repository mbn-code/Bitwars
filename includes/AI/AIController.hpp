#ifndef AI_CONTROLLER_HPP
#define AI_CONTROLLER_HPP

#ifdef _WIN32
#include "../src/main.hpp"
#elif __APPLE__
#include "src/main.hpp"
#endif

void npc_logic_update(std::vector<Unit>& npc_units, Base& npc_base, std::vector<Unit>& player_units,  bool isOffensive);

#endif // AI_CONTROLLER_HPP
