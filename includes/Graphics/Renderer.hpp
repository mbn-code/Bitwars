#ifndef RENDERER_HPP
#define RENDERER_HPP

#ifdef _WIN32
#include "../game_logic/GameDataStructures.hpp"
#include "../game_logic/Constants.hpp"
#include "../../src/main.hpp"
#include "../../lib/windows/raylib.h"
#elif __APPLE__
#include "src/main.hpp"
#include "game_logic/GameDataStructures.hpp"
#include "game_logic/Constants.hpp"
#endif

#include <vector>

void draw_units(const std::vector<Unit>& units);
void draw_health_bar(const Unit& unit);
void draw_improved_ui(const Base& player_base, const Base& npc_base);
void draw_bases(const Base& player_base, const Base& npc_base);
void draw_health_bar_for_base(const Base& base, int base_x, Color color);
void draw_timer(float elapse_time);

#endif // RENDERER_HPP