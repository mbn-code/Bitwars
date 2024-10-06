
# 🎮 War Simulation Game

This is a simple war simulation game developed using C++ and the Raylib library. The game features player and NPC bases, unit spawning, and combat mechanics.

## 📁 Project Structure

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

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (e.g., clang++)
- Raylib library

### Building the Project

To build the project, run the following command:

```sh
make build_osx
```

This will compile the source files and create an executable in the `bin/` directory.

### Running the Game

After building the project, you can run the game using the following command:

```sh
./bin/build_osx
```

## 📄 Project Files

### Source Files

- `src/main.cpp`: Contains the main game loop and game logic.
- `src/main.hpp`: Header file for `main.cpp`.

### Include Files

- `includes/GameDataStructures.hpp`: Defines the data structures used in the game.
- `includes/HandleCombat.cpp`: Implements the combat handling logic.
- `includes/HandleCombat.hpp`: Header file for `HandleCombat.cpp`.
- `includes/PointMultiplier.cpp`: Implements the point multiplier logic.
- `includes/PointMultiplier.hpp`: Header file for `PointMultiplier.cpp`.

### Library Files

- `lib/raylib.h`: Raylib library header.
- `lib/raymath.h`: Raylib math library header.
- `lib/libraylib.a`: Raylib static library.

### Configuration Files

- `.vscode/tasks.json`: VSCode tasks for building the project.
- `makefile`: Makefile for building the project.

## ⚔️ Game Mechanics

### Bases

- Player and NPC bases are represented by the `Base` struct.
- Bases have `health` and `points` attributes.

### Units

- Units are spawned and controlled by the player and NPC.
- Units engage in combat when they collide.

### Combat

- Combat is handled by the `HandleCombat` class.
- Points are awarded based on unit health and combat outcomes.
