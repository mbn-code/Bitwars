#ifndef MAIN_HPP
#define MAIN_HPP

// Standard library includes
#include <vector>
#include <cstdlib>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define FRAME_RATE 60

#define UNIT_SPEED 2.0f
#define POINTS_PER_TICK 1
#define POINTS_INTERVAL 60
#define PLAYER_BASE_INITIAL_POINTS 80
#define NPC_BASE_INITIAL_POINTS 80

// Platform-specific includes for game logic and Raylib
#ifdef _WIN32  // Windows-specific includes
#include "../includes/game_logic/PointMultiplier.hpp"
#include "../includes/game_logic/GameDataStructures.hpp"
#include "../includes/game_logic/HandleCombat.hpp"
#include "../includes/game_logic/HandleHighScore.hpp"
#include "../includes/game_logic/Constants.hpp"
#include "../includes/cheat_detection/CheatChecker.hpp"
#include "../includes/Audio/AudioInitializer.hpp"
#include "../includes/game_logic/UnitHandler.hpp"
#include "../includes/AI/AIController.hpp"
#include "../lib/windows/raylib.h"
#include "../lib/windows/rlgl.h"
#include <fstream>
#include <algorithm>
// Load rcore and rlgl here for textures to work on windows:
// #include "../
#elif __APPLE__  // macOS-specific includes
#include "../includes/game_logic/PointMultiplier.hpp"
#include "../includes/game_logic/GameDataStructures.hpp"
#include "../includes/game_logic/HandleCombat.hpp"
#include "../includes/game_logic/HandleHighScore.hpp"
#include "../includes/game_logic/UnitHandler.hpp"
#include "../includes/cheat_detection/CheatChecker.hpp"
#include "../includes/Graphics/Renderer.hpp"
#include "../includes/Audio/AudioInitializer.hpp"
#include "../includes/AI/AIController.hpp"
#include "../lib/raylib.h"
#else  // Fallback for other platforms
#include "../includes/PointMultiplier.hpp"
#include "../includes/GameDataStructures.hpp"
#include "../includes/HandleCombat.hpp"
#include "../includes/HandleHighScore.hpp"
#include "../includes/CheatChecker.hpp"
#include "../lib/raylib.h"
#endif


// texture loading paths for different platforms
#ifdef _WIN32
#define BASE_TEXTURE_PATH "../assets/Images/Base_texture_BR.png"
#define BASE2_TEXTURE_PATH "../assets/Images/Base2_texture_BR.png"
#define BACKGROUND_TEXTURE_PATH "../assets/Images/background.png"
#define MUSIC_PATH "../assets/Sound/Default_Sound_Track.wav"
#elif __APPLE__
#define BASE_TEXTURE_PATH "assets/Images/Base_texture_BR.png"
#define BASE2_TEXTURE_PATH "assets/Images/Base2_texture_BR.png"
#define BACKGROUND_TEXTURE_PATH "assets/Images/background.png"
#define MUSIC_PATH "assets/Sound/Default_Sound_Track.wav"
#else
#error "Unknown platform"
#endif


#endif // MAIN_HPP