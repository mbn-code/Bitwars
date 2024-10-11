#ifndef MAIN_HPP
#define MAIN_HPP

// Standard library includes
#include <vector>
#include <cstdlib>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define FRAME_RATE 60

#define PLAYER_BASE_HEALTH 100
#define NPC_BASE_HEALTH 100
#define UNIT_SPEED 2.0f
#define POINTS_PER_TICK 1
#define POINTS_INTERVAL 60
#define PLAYER_BASE_INITIAL_POINTS 50
#define NPC_BASE_INITIAL_POINTS 50

// Platform-specific includes for game logic and Raylib
#ifdef _WIN32  // Windows-specific includes
#include "../includes/PointMultiplier.hpp"
#include "../includes/GameDataStructures.hpp"
#include "../includes/HandleCombat.hpp"
#include "../includes/HandleHighScore.hpp"
#include "../includes/CheatChecker.hpp"
#include "../lib/windows/raylib.h"
#elif __APPLE__  // macOS-specific includes
#include "../includes/PointMultiplier.hpp"
#include "../includes/GameDataStructures.hpp"
#include "../includes/HandleCombat.hpp"
#include "../includes/HandleHighScore.hpp"
#include "../includes/CheatChecker.hpp"
#include "../lib/raylib.h"
#else  // Fallback for other platforms
#include "../includes/PointMultiplier.hpp"
#include "../includes/GameDataStructures.hpp"
#include "../includes/HandleCombat.hpp"
#include "../includes/HandleHighScore.hpp"
#include "../includes/CheatChecker.hpp"
#include "../lib/raylib.h"
#endif

#endif // MAIN_HPP