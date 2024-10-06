
# Bitwars - cpp raylib project

The reason I made this is to challenge myself, to make something that is similar to a game, or a game structure without using a game engine like I have before.

## Overview

A simple war simulation game developed using C++ and Raylib. Features include player and NPC bases, unit spawning, and combat mechanics.

## Project Structure

```plaintext
.vscode/
├── c_cpp_properties.json
├── launch.json
├── settings.json
└── tasks.json

assets/
bin/
build_osx/
includes/
├── GameDataStructures.hpp
├── HandleCombat.cpp
├── HandleCombat.hpp
├── PointMultiplier.cpp
└── PointMultiplier.hpp

lib/
├── libraylib.a
├── raylib.h
└── raymath.h

makefile
README.md
src/
├── main.cpp
└── main.hpp
```

## Prerequisites

- C++17 compatible compiler (e.g., clang++, g++)
- Raylib library

## Building the Project

### macOS

```sh
make PLATFORM=macos
```

### Windows

```sh
make PLATFORM=windows
```

This will compile the source files and spit out a executable for either macOS arm or windows x64

## 🚀 Getting Started

### Running the Game

After building the project, you can run the game using the following command:

```sh
./bin/build_osx
```

```sh
./bin/windows
```

## 📄 Project Files

### Source Files

- `src/main.cpp`: Contains the main game loop and game logic.
- `src/main.hpp`: Header file for `main.cpp`.

### Include Files

- `includes/GameDataStructures.hpp`: Defines the data structures used in the game.
- `includes/HandleCombat.hpp`: Header file for `HandleCombat.cpp`.
- `includes/PointMultiplier.hpp`: Header file for `PointMultiplier.cpp`.
- `includes/HandleHighScore.hpp`: Header file for handling high scores.
- `lib/raylib.h`: Raylib library header.

### Library Files

- `lib/raylib.h`: Raylib library header.
- `lib/raymath.h`: Raylib math library header.
- `lib/libraylib.a`: Raylib static library.

### Configuration Files

- `.vscode/tasks.json`: VSCode tasks for building the project.
- `makefile`: Makefile for building the project.

## Game Mechanics

### Bases

- **Player and NPC Bases**: Represented by the `Base` struct.
  - **Attributes**:
    - `health`: The health of the base.
    - `points`: Points accumulated by the base.

### Units

- **Spawning**: Units are spawned by both the player and NPC.
- **Control**: Units are controlled by the player and NPC.
- **Combat**: Units engage in combat when they collide with enemy units.

### Combat

- **Handling**: Managed by the `HandleCombat` class.
- **Points System**: Points are awarded based on the health of units and the outcomes of combat encounters.

### High Scores

- **Handling**: Managed by the `HandleHighScore` class.
- **Storage**: High scores are stored and retrieved to track player performance.

### Point Multiplier

- **Mechanics**: The `PointMultiplier` class handles the logic for multiplying points based on specific conditions or achievements within the game.

- Player and NPC bases are represented by the `Base` struct.
- Bases have `health` and `points` attributes.
