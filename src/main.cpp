#include "main.hpp"

// Function Prototypes
void SpawnUnit(std::vector<Unit>& units, Base& base, UnitType unitType, bool isPlayerUnit);
void UpdateUnits(std::vector<Unit>& units, Base& opponentBase);
void DrawUnits(const std::vector<Unit>& units);
void DrawHealthBar(const Unit& unit);
void DrawImprovedUI(const Base& playerBase, const Base& npcBase);
void DrawBases(const Base& playerBase, const Base& npcBase);
void DrawHealthBarForBase(const Base& base, int baseX, Color color);

// Global Variables
GameState currentState = START_SCREEN;
Base playerBase = {{50, SCREEN_HEIGHT / 2 - 50, 50, 100}, PLAYER_BASE_HEALTH, PLAYER_BASE_INITIAL_POINTS};
Base npcBase = {{SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 50, 50, 100}, NPC_BASE_HEALTH, NPC_BASE_INITIAL_POINTS};
std::vector<Unit> playerUnits;
std::vector<Unit> npcUnits;
int pointTickCounter = 0;

float elapsedTime = 0.0f;
bool gameWon = false;
float winTime = 0.0f;

float lastScore = ReadLastScore();

// Function to initialize and play background music
Music InitAndPlayBackgroundMusic(const char* fileName) {
    InitAudioDevice();  // Initialize audio device
    Music backgroundMusic = LoadMusicStream(fileName);  // Load music
    backgroundMusic.looping = true;  // Enable looping
    SetMusicVolume(backgroundMusic, 0.5f);  // Set volume to half
    PlayMusicStream(backgroundMusic);  // Play music
    return backgroundMusic;
}

// Function Implementations
void SpawnUnit(std::vector<Unit>& units, Base& base, UnitType unitType, bool isPlayerUnit) {
    Unit newUnit;
    newUnit.type = unitType;
    newUnit.isPlayerUnit = isPlayerUnit;

    // Set unit properties based on type
    switch (unitType) {
        case TYPE_1_SOLDIER:
            newUnit.hitbox = {isPlayerUnit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 20, base.hitbox.y + base.hitbox.height / 2 - 10, 20, 20};
            newUnit.health = 10;
            newUnit.damage = 5;
            newUnit.speed = UNIT_SPEED;
            break;
        case TYPE_2_SOLDIER:
            newUnit.hitbox = {isPlayerUnit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 20, base.hitbox.y + base.hitbox.height / 2 - 15, 20, 30};
            newUnit.health = 20;
            newUnit.damage = 10;
            newUnit.speed = UNIT_SPEED;
            break;
        case TYPE_3_SOLDIER:
            newUnit.hitbox = {isPlayerUnit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 30, base.hitbox.y + base.hitbox.height / 2 - 20, 30, 40};
            newUnit.health = 30;
            newUnit.damage = 15;
            newUnit.speed = UNIT_SPEED - 0.5f;  // Slightly slower speed
            break;
        case TANK:
            newUnit.hitbox = {isPlayerUnit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 40, base.hitbox.y + base.hitbox.height / 2 - 20, 40, 40};
            newUnit.health = 50;
            newUnit.damage = 20;
            newUnit.speed = UNIT_SPEED - 1.0f;  // Slow speed
            break;
        case BIT:
            newUnit.hitbox = {isPlayerUnit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 20, base.hitbox.y + base.hitbox.height / 2 - 10, 20, 20};
            newUnit.health = 1;
            newUnit.damage = GetRandomValue(0, 1);
            newUnit.speed = UNIT_SPEED + 1.5f; // Fast speed 
            break;
    }

    units.push_back(newUnit);
}

// Function Definitions
void UpdateUnits(std::vector<Unit>& units, Base& opponentBase) {
    for (auto it = units.begin(); it != units.end(); ) {
        Unit& unit = *it;
    	// Move unit towards the opponent base
        HandleCombat::MoveUnitTowards(unit, {opponentBase.hitbox.x, opponentBase.hitbox.y});

        // Check if the unit has reached the opponent base
        if (CheckCollisionRecs(unit.hitbox, opponentBase.hitbox)) {
            // Deal damage to the opponent base
            opponentBase.health -= unit.damage;

            // Remove the unit
            unit.health = 0;
        }

        // Update unit health
        if (unit.health <= 0) {
            it = units.erase(it); // Correctly erase the unit
        }
        else {
            ++it;
        }

    }
}

void DrawHealthBar(const Unit& unit) {
    float healthBarWidth = unit.hitbox.width;
    float healthPercentage = (float)unit.health / (float)(unit.type == TYPE_1_SOLDIER ? 10 : unit.type == TYPE_2_SOLDIER ? 20 : unit.type == TYPE_3_SOLDIER ? 30 : 50);
    Color healthColor = healthPercentage > 0.5 ? GREEN : healthPercentage > 0.25 ? YELLOW : RED;

    // Draw health bar above the unit
    DrawRectangle(unit.hitbox.x, unit.hitbox.y - 10, healthBarWidth * healthPercentage, 5, healthColor);

    // Draw health bar border
    DrawRectangleLines(unit.hitbox.x, unit.hitbox.y - 10, healthBarWidth, 5, BLACK);

    // draw health bar for tank specifically
    if (unit.type == TANK) {
        DrawRectangle(unit.hitbox.x, unit.hitbox.y - 15, healthBarWidth * healthPercentage, 5, healthColor);
        DrawRectangleLines(unit.hitbox.x, unit.hitbox.y - 15, healthBarWidth, 5, BLACK);
    } else if (unit.type == BIT) {
        DrawRectangle(unit.hitbox.x, unit.hitbox.y - 5, healthBarWidth * healthPercentage, 5, healthColor);
        DrawRectangleLines(unit.hitbox.x, unit.hitbox.y - 5, healthBarWidth, 5, BLACK);
    }

}

void DrawBit(const Unit& unit) {
    // Implement drawing logic for bits
    DrawRectangle(unit.hitbox.x, unit.hitbox.y, 5, 5, unit.isPlayerUnit ? BLUE : RED);
}

void DrawUnits(const std::vector<Unit>& units) {
    for (const auto& unit : units) {
        if (unit.type == BIT) {
            DrawBit(unit);
        } else {
            DrawRectangle(unit.hitbox.x, unit.hitbox.y, unit.hitbox.width, unit.hitbox.height, unit.isPlayerUnit ? BLUE : RED);
            DrawHealthBar(unit);
        }
    }
}


void DrawBases(const Base& playerBase, const Base& npcBase) {
    // Draw player and NPC bases with distinct colors
    DrawRectangleRec(playerBase.hitbox, BLUE);
    DrawRectangleRec(npcBase.hitbox, RED);
}

void DrawHealthBarForBase(const Base& base, int baseX, Color color) {
    DrawRectangle(baseX, 20, 200, 30, GRAY);  // Background bar
    DrawRectangle(baseX, 20, 200 * (float)base.health / PLAYER_BASE_HEALTH, 30, color);  // Health bar
}

void DrawImprovedUI(const Base& playerBase, const Base& npcBase) {
    DrawHealthBarForBase(playerBase, 20, BLUE);
    DrawHealthBarForBase(npcBase, SCREEN_WIDTH - 220, RED);

    DrawText(TextFormat("Player Points: %d", playerBase.points), 20, 100, 20, BLACK);
    DrawText(TextFormat("NPC Points: %d", npcBase.points), SCREEN_WIDTH - 220, 100, 20, BLACK);

    DrawText("Unit Purchase Options:", 20, SCREEN_HEIGHT - 200, 20, DARKGRAY);
    DrawText("[1] Type 1 Soldier - 5 Points", 20, SCREEN_HEIGHT - 170, 20, DARKGRAY);
    DrawText("[2] Type 2 Soldier - 10 Points", 20, SCREEN_HEIGHT - 140, 20, DARKGRAY);
    DrawText("[3] Type 3 Soldier - 30 Points", 20, SCREEN_HEIGHT - 110, 20, DARKGRAY);
    DrawText("[4] Tank - 50 Points", 20, SCREEN_HEIGHT - 80, 20, DARKGRAY);
    DrawText("[5] Bit - 2 Points", 20, SCREEN_HEIGHT - 50, 20, DARKGRAY);
}

void DrawCenteredText(const char* text, int posX, int posY, int fontSize, Color color) {
    DrawText(text, posX - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}

void DrawTimer(float elapsedTime) {
    DrawCenteredText(TextFormat("Time: %.2f", elapsedTime), SCREEN_WIDTH/2, 30, 20, BLACK);
}

int main() {

	CheatChecker cheatChecker(&playerBase, &npcBase, &playerUnits, &npcUnits);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bitwars");

    SetTargetFPS(FRAME_RATE);

    Music backgroundMusic = InitAndPlayBackgroundMusic("assets/Sound/Default_Sound_Track.wav");

    while (!WindowShouldClose()) {

        cheatChecker.MonitorForCheats();

        UpdateMusicStream(backgroundMusic);  // Update music stream

        if (currentState == GAMEPLAY && !gameWon) {
            elapsedTime += GetFrameTime();
        }
        
        bool isOffensive = npcBase.health > playerBase.health;  // Go offensive if NPC base has more health

        // logical switch case statements based on game logic and player input
        switch (currentState) {
            case START_SCREEN:
                if (IsKeyPressed(KEY_ENTER)) {


                    playerBase.health = PLAYER_BASE_HEALTH;
                    npcBase.health = NPC_BASE_HEALTH;
                    playerBase.points = PLAYER_BASE_INITIAL_POINTS;
                    npcBase.points = NPC_BASE_INITIAL_POINTS;
                    playerUnits.clear();
                    npcUnits.clear();
                    
                    elapsedTime = 0.0f;
                    gameWon = false;
                    currentState = GAMEPLAY;
                }
                break;

            case GAMEPLAY:
                pointTickCounter++;

                // Player input for buying units
                if (IsKeyPressed(KEY_ONE) && playerBase.points >= 5) {
                    SpawnUnit(playerUnits, playerBase, TYPE_1_SOLDIER, true);
                    playerBase.points -= 5;
                }
                if (IsKeyPressed(KEY_TWO) && playerBase.points >= 10) {
                    SpawnUnit(playerUnits, playerBase, TYPE_2_SOLDIER, true);
                    playerBase.points -= 10;
                }
                if (IsKeyPressed(KEY_THREE) && playerBase.points >= 30) {
                    SpawnUnit(playerUnits, playerBase, TYPE_3_SOLDIER, true);
                    playerBase.points -= 30;
                }
                if (IsKeyPressed(KEY_FOUR) && playerBase.points >= 50) {
                    SpawnUnit(playerUnits, playerBase, TANK, true);
                    playerBase.points -= 50;
                }
                if (IsKeyPressed(KEY_FIVE) && playerBase.points >= 2) {
                    SpawnUnit(playerUnits, playerBase, BIT, true);
                    playerBase.points -= 2;
                }


                // NPC logic for buying units based on offensive or defensive mode and available points
                if (isOffensive) {
                    // Offensive mode: NPC spawns more aggressive units
                    if (npcBase.points >= 50 && GetRandomValue(0, 100) < 10) {  // Higher chance to spawn tanks
                        SpawnUnit(npcUnits, npcBase, TANK, false);
                        npcBase.points -= 50;
                    } else if (npcBase.points >= 30 && GetRandomValue(0, 100) < 20) {  // Chance to spawn Type 3 soldiers
                        SpawnUnit(npcUnits, npcBase, TYPE_3_SOLDIER, false);
                        npcBase.points -= 30;
                    } else if (npcBase.points >= 10 && GetRandomValue(0, 100) < 30) {  // Chance to spawn Type 2 soldiers
                        SpawnUnit(npcUnits, npcBase, TYPE_2_SOLDIER, false);
                        npcBase.points -= 10;
                    } else if (npcBase.points >= 5 && GetRandomValue(0, 100) < 40) {  // Chance to spawn Type 1 soldiers
                        SpawnUnit(npcUnits, npcBase, TYPE_1_SOLDIER, false);
                        npcBase.points -= 5;
                    } else if (npcBase.points >= 12 && GetRandomValue(0, 100) < 50) {  // Chance to spawn Bits
                        SpawnUnit(npcUnits, npcBase, BIT, false);
                        npcBase.points -= 12;
                    }
                } else {
                    // Defensive mode: NPC spawns units less frequently but focuses on stronger units
                    if (npcBase.points >= 50 && GetRandomValue(0, 100) < 5) {  // Lower chance to spawn tanks
                        SpawnUnit(npcUnits, npcBase, TANK, false);
                        npcBase.points -= 50;
                    } else if (npcBase.points >= 30 && GetRandomValue(0, 100) < 10) {  // Chance to spawn Type 3 soldiers
                        SpawnUnit(npcUnits, npcBase, TYPE_3_SOLDIER, false);
                        npcBase.points -= 30;
                    } else if (npcBase.points >= 10 && GetRandomValue(0, 100) < 15) {  // Chance to spawn Type 2 soldiers
                        SpawnUnit(npcUnits, npcBase, TYPE_2_SOLDIER, false);
                        npcBase.points -= 10;
                    } else if (npcBase.points >= 5 && GetRandomValue(0, 100) < 20) {  // Chance to spawn Type 1 soldiers
                        SpawnUnit(npcUnits, npcBase, TYPE_1_SOLDIER, false);
                        npcBase.points -= 5;
                    } else if (npcBase.points >= 12 && GetRandomValue(0, 100) < 25) {  // Chance to spawn Bits
                        SpawnUnit(npcUnits, npcBase, BIT, false);
                        npcBase.points -= 12;
                    }
                }

                /* 
                @brief Award points to both players over time
                
                @detail 
                The pointTickCounter variable is used to keep track of the number of frames that have passed since the last point award.
                The POINTS_INTERVAL constant is used to determine the number of frames that must pass before points are awarded.
                The POINTS_PER_TICK constant is used to determine the number of points awarded to each player per tick.
                The playerBase and npcBase points are incremented by POINTS_PER_TICK if the pointTickCounter is greater than or equal to POINTS_INTERVAL.

                Basically every 60 frames (1 second), the player and NPC bases are awarded POINTS_PER_TICK points. Because when the pointTickCounter reaches 60, the points are awarded and the counter is reset to 0. This is done to prevent points from being awarded every frame, which would be too fast. This is a simple way to implement a time-based point award system.

                @flow-chart
                +----------------------------+
                |   System Checkpoint Logic  |
                +----------------------------+
                            |
                            V
                +---------------------------+
                |  Increment Tick Counter   |
                +---------------------------+
                            |
                            V
                +---------------------------+
                |  Check if pointTickCounter|
                |   >= POINTS_INTERVAL      |
                +---------------------------+
                            |
                        +----+----+
                        |         |
                        | Yes     | No
                        |         |
                        V         |
                +---------------------------+
                |   Add POINTS_PER_TICK to  |
                |   playerBase.points       |
                |   Add POINTS_PER_TICK to  |
                |   npcBase.points          |
                +---------------------------+
                            |
                            V
                +---------------------------+
                |  Reset pointTickCounter to |
                |           0               |
                +---------------------------+
                            |
                            V
                +---------------------------+
                |       Repeat Every 60     |
                |          Ticks (1 sec)    |
                +---------------------------+
                */

                // Update units and check collisions
                HandleCombat::HandleGameCombat(playerUnits, npcUnits, playerBase, npcBase);
                HandleCombat::HandleGameCombat(npcUnits, playerUnits, npcBase, playerBase);

                // move the units towards the base 
                UpdateUnits(playerUnits, npcBase);
                UpdateUnits(npcUnits, playerBase);


                if (pointTickCounter >= POINTS_INTERVAL) {
                    playerBase.points += POINTS_PER_TICK;
                    npcBase.points += POINTS_PER_TICK;
                    pointTickCounter = 0;
                }

                // Check if any base is destroyed
                /* 
                @brief Check if either base has been destroyed and update game state

                @details
                If either the player's or NPC's base health reaches zero, the game state is updated to END_SCREEN.
                The game is won by the player if the NPC's base health reaches zero, and the game is won by the NPC if the player's base health reaches zero.
                The winTime variable is used to store the time at which the game was won, which is displayed on the end screen.
                */
                if (playerBase.health <= 0 || npcBase.health <= 0) {
                    currentState = END_SCREEN;
                    if (!gameWon) {
                        gameWon = true;
                        winTime = elapsedTime;
                    }
                }
                
                break;

            case PAUSE_SCREEN:
                if (IsKeyPressed(KEY_P)) {
                    currentState = GAMEPLAY;
                }
                break;

            case END_SCREEN:
                if (IsKeyPressed(KEY_ENTER)) {
                    currentState = START_SCREEN;
                }
                break;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw game elements based on the current state. Seperated from the switch case above for better readability.
        // And everything here is pretty straight forward.
        switch (currentState) {
            case START_SCREEN:
                DrawText("Welcome to Bitwars!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, BLACK);
                DrawText("Press ENTER to start", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, 20, DARKGRAY);
                break;

            case GAMEPLAY:
                DrawBases(playerBase, npcBase);
                DrawImprovedUI(playerBase, npcBase);
                DrawUnits(playerUnits);
                DrawUnits(npcUnits);
                DrawTimer(elapsedTime);
                break;

            case PAUSE_SCREEN:
                DrawText("Game Paused", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, 20, GRAY);
                break;

            case END_SCREEN:
                if (IsKeyPressed(KEY_ENTER)) {
                    currentState = START_SCREEN;
                }
                
                if (playerBase.health <= 0) {
                    DrawText("Game Over! You Lost!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, GRAY);
                } else if (npcBase.health <= 0) {
                    DrawText("Congratulations! You Won!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, GRAY);
                }
                DrawText(TextFormat("Time: %.2f seconds", winTime), SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 20, GRAY);
                DrawText("Press ENTER to restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 20, GRAY);

                // Check if the game is won and write the score
                if (!gameWon) {
                    gameWon = true;
                    winTime = elapsedTime; // Capture the win time
                    WriteLastScore(winTime); // Write score only when game is won
                }
            break;
            }
        EndDrawing();
    }
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}