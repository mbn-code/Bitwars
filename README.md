# ⚔️ Bitwars - War Simulation Game

**Bitwars** is a simple war simulation game developed using C++ and the **Raylib** graphics library. In this game, players engage in combat by spawning units and managing a base, trying to outlast the opposing NPC forces. The game includes mechanics like combat handling, points accumulation, and a high score system.

---

## 🗂️ Project Structure

The project is organized into several directories and files, each serving a specific purpose. Here's a brief overview:

### 📁 Directories

- **`.vscode/`**: Contains Visual Studio Code configuration files.
  - `c_cpp_properties.json`, `launch.json`, `settings.json`, `tasks.json`: Configuration files for setting up the C++ environment and build tasks.

- **`assets/`**: Contains game assets such as images and sounds.
  - `generated/`: Holds dynamically generated assets.
  - `main.py`: Script for asset generation.

- **`bin/`**: Contains the compiled binaries for the game.
  - `Bitwars/`, `Bitwars.exe`: The game executable files.

- **`includes/`**: Contains header files for various game components.
  - `GameDataStructures.hpp`: Defines the data structures used in the game.
  - `HandleCombat.hpp`: Header for combat-related functionality.
  - `PointMultiplier.hpp`: Header for point multiplier logic.

- **`lib/`**: External libraries and headers used by the game.
  - `GLFW/`: GLFW library files.
  - `libraylib.a`: Static Raylib library.
  - `raylib.h`, `raymath.h`: Raylib header files.
  - `windows/`: Windows-specific Raylib files.

- **`src/`**: Contains source code files.
  - `main.cpp`: Main game loop and logic.
  - `main.hpp`: Header for the main source file.

### 📄 Key Files

- **`makefile`**: Defines how the project is built. Supports building for macOS and Windows.
- **`README.md`**: The main documentation for the project.
- **`.gitignore`**: Specifies files and directories to be ignored by Git (e.g., build artifacts).

---

## ⚙️ Building the Project

### macOS

To build the game on macOS, run:

```bash
make PLATFORM=macos
```

### Windows

To build the game on Windows, run:

```bash
make PLATFORM=windows
```

---

## 🕹️ Running the Game

Once the project is built, you can run the game using the compiled executable.

### macOS Run

```bash
./Bitwars
```

### Windows Run

```bash
Bitwars.exe
```

---

## 🎮 Game Mechanics

### 🏠 Bases

- **Player and NPC Bases**: Each side (player and NPC) has a base represented by the `Base` struct.
- **Attributes**:
  - `health`: Represents the base's health.
  - `points`: Accumulated points based on game progress.

### 👥 Units

- **Spawning**: Both the player and NPC can spawn units to attack the opposing base.
- **Control**: Units are controlled by both the player and NPC.
- **Combat**: Units engage in combat automatically when they collide with enemy units.

### ⚔️ Combat

- **Handling**: Combat is managed by the `HandleCombat` class, which handles logic for units fighting each other.
- **Points System**: Points are awarded based on unit health and combat outcomes.

### 🏆 High Scores

- **Handling**: Managed by the `HandleHighScore` class.
- **Storage**: High scores are stored and retrieved to track the player's best performance.

### 🔢 Point Multiplier

- **Mechanics**: The `PointMultiplier` class manages conditions for increasing the player's points under certain conditions, adding a strategic layer to gameplay.

---

## 🤝 Contributing

Contributions are welcome! Please follow the standard [GitHub flow](https://guides.github.com/introduction/flow/) when contributing:

1. **Fork** the repository.
2. **Create** a new branch for your feature or bugfix.
3. **Commit** your changes and **push** them to your fork.
4. **Submit** a pull request and provide a clear description of the changes.
