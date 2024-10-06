#ifndef MAIN_HPP
#define MAIN_HPP

// Standard library includes
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>  // For std::remove_if

// Platform-specific includes for game logic and Raylib
#ifdef _WIN32  // Windows-specific includes
#include "../includes/PointMultiplier.hpp"
#include "../includes/GameDataStructures.hpp"
#include "../includes/HandleCombat.hpp"
#include "../includes/HandleHighScore.hpp"
#include "../lib/windows/raylib.h"
#elif __APPLE__  // macOS-specific includes
#include "includes/PointMultiplier.hpp"
#include "includes/GameDataStructures.hpp"
#include "includes/HandleCombat.hpp"
#include "includes/HandleHighScore.hpp"
#include "lib/raylib.h"
#else  // Fallback for other platforms
#include "includes/PointMultiplier.hpp"
#include "includes/GameDataStructures.hpp"
#include "includes/HandleCombat.hpp"
#include "includes/HandleHighScore.hpp"
#include "raylib.h"
#endif

#endif // MAIN_HPP
