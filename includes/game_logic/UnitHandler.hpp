#ifndef UNIT_HANDLER_HPP
#define UNIT_HANDLER_HPP

#ifdef _WIN32
#include "../../src/main.hpp"
#elif __APPLE__
#include "src/main.hpp"
#endif


// Function to spawn a unit and add it to the units vector
void SpawnUnit(std::vector<Unit>& units, Base& base, UnitType unit_type, bool is_player_unit);
void UpdateUnits(std::vector<Unit>& units, Base& opponentBase);


#endif // UNIT_HANDLER_HPP