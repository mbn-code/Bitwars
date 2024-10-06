// includes
#include "main.hpp"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define PLAYER_BASE_HEALTH 100
#define NPC_BASE_HEALTH 100
#define UNIT_SPEED 2.0f  

const int POINTS_PER_TICK = 1; 
const int POINTS_INTERVAL = 60;
int pointTickCounter = 0;

// Function prototypes
void SpawnUnit(std::vector<Unit>& units, Base& base, UnitType unitType, bool isPlayerUnit);
void UpdateUnits(std::vector<Unit>& units, Base& opponentBase);
void HandleCombat(std::vector<Unit>& playerUnits, std::vector<Unit>& npcUnits, Base& playerBase, Base& npcBase);
void DrawUnits(const std::vector<Unit>& units);
void DrawUI(const Base& playerBase, const Base& npcBase);

// Global Variables
GameState currentState = START_SCREEN;
Base playerBase = {{50, SCREEN_HEIGHT / 2 - 50, 50, 100}, PLAYER_BASE_HEALTH, 50};
Base npcBase = {{SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 50, 50, 100}, NPC_BASE_HEALTH, 50};
std::vector<Unit> playerUnits;
std::vector<Unit> npcUnits;

// Function to spawn units
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
            newUnit.hitbox = {isPlayerUnit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 40, base.hitbox.y + base.hitbox.height / 2 - 25, 40, 50};
            newUnit.health = 50;
            newUnit.damage = 20;
            newUnit.speed = UNIT_SPEED - 1.0f;  // Slow speed
            break;
    }

    units.push_back(newUnit);
}

// Function to update unit positions and check for collisions
void UpdateUnits(std::vector<Unit>& units, Base& opponentBase) {
    for (auto& unit : units) {
        // Move units towards the opponent base
        if (unit.isPlayerUnit) {
            unit.hitbox.x += unit.speed;  // Move right
        } else {
            unit.hitbox.x -= unit.speed;  // Move left
        }

        // Check if the unit has reached the opponent's base
        if (CheckCollisionRecs(unit.hitbox, opponentBase.hitbox)) {
            opponentBase.health -= unit.damage;  // Inflict damage to the base
            unit.health = 0;  // Unit dies after attacking the base
        }
    }

    // Remove units with zero or less health
    units.erase(std::remove_if(units.begin(), units.end(), [](const Unit& u) { return u.health <= 0; }), units.end());
}


// Function to draw all units on screen
void DrawUnits(const std::vector<Unit>& units) {
    for (const auto& unit : units) {
        if (unit.isPlayerUnit) {
            DrawRectangleRec(unit.hitbox, BLUE);  // Player units in blue
        } else {
            DrawRectangleRec(unit.hitbox, RED);   // NPC units in red
        }
    }
}

// Function to draw UI elements on screen
void DrawUI(const Base& playerBase, const Base& npcBase) {
    // Draw the player's base health, points, and unit buy options
    DrawText(TextFormat("Player Base Health: %d", playerBase.health), 20, 20, 20, BLUE);
    DrawText(TextFormat("Player Points: %d", playerBase.points), 10, 10, 20, BLACK);

    // Draw the NPC's base health and points
    DrawText(TextFormat("NPC Base Health: %d", npcBase.health), SCREEN_WIDTH - 220, 20, 20, RED);
    DrawText(TextFormat("NPC Points: %d", npcBase.points), SCREEN_WIDTH - 200, 10, 20, BLACK);

    // Draw UI for unit purchase options
    DrawText("Unit Purchase Options:", 20, SCREEN_HEIGHT - 150, 20, DARKGRAY);
    DrawText("[1] Type 1 Soldier - 5 Points", 20, SCREEN_HEIGHT - 120, 20, DARKGRAY);
    DrawText("[2] Type 2 Soldier - 10 Points", 20, SCREEN_HEIGHT - 90, 20, DARKGRAY);
    DrawText("[3] Type 3 Soldier - 30 Points", 20, SCREEN_HEIGHT - 60, 20, DARKGRAY);
    DrawText("[4] Tank - 50 Points", 20, SCREEN_HEIGHT - 30, 20, DARKGRAY);


}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bitwars");
   
        SetTargetFPS(60);

    // Game loop
    while (!WindowShouldClose()) {
        switch (currentState) {
            case START_SCREEN:
                if (IsKeyPressed(KEY_ENTER)) {
                    playerBase.health = PLAYER_BASE_HEALTH;
                    npcBase.health = NPC_BASE_HEALTH;
                    playerBase.points = 50;
                    npcBase.points = 50;
                    playerUnits.clear();
                    npcUnits.clear();
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

                // NPC randomly spawns units based on its points
                if (GetRandomValue(0, 100) < 2) {  // NPC spawns units at random intervals
                    UnitType randomType = static_cast<UnitType>(GetRandomValue(0, 3));  // Randomly select a unit type
                    int cost = (randomType == TYPE_1_SOLDIER) ? 5 : (randomType == TYPE_2_SOLDIER) ? 10 : (randomType == TYPE_3_SOLDIER) ? 30 : 50;
                    if (npcBase.points >= cost) {
                        SpawnUnit(npcUnits, npcBase, randomType, false);
                        npcBase.points -= cost;
                    }
                }

                if (pointTickCounter >= POINTS_INTERVAL) {
                    playerBase.points += static_cast<int>(POINTS_PER_TICK * PointMultiplier::CalculatePointMultiplier(playerBase.health));
                    npcBase.points += POINTS_PER_TICK;
                    pointTickCounter = 0;
                }

                // Update unit positions and handle combat
                UpdateUnits(playerUnits, npcBase);
                UpdateUnits(npcUnits, playerBase);
                HandleCombat::HandleGameCombat(playerUnits, npcUnits, playerBase, npcBase);

                // Check if any base is destroyed
                if (playerBase.health <= 0 || npcBase.health <= 0) {
                    currentState = END_SCREEN;
                }
                break;

            case PAUSE_SCREEN:
                if (IsKeyPressed(KEY_P)) currentState = GAMEPLAY;
                break;

            case END_SCREEN:
                if (IsKeyPressed(KEY_R)) {
                    playerBase.health = PLAYER_BASE_HEALTH;
                    npcBase.health = NPC_BASE_HEALTH;
                    playerBase.points = 50;
                    npcBase.points = 50;
                    playerUnits.clear();
                    npcUnits.clear();
                    currentState = START_SCREEN;
                }
                break;
        }

        // Draw everything
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentState) {
            case START_SCREEN:
                DrawText("Press ENTER to Start", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 30, BLACK);
                break;

            case GAMEPLAY:
                // Draw UI
                DrawUI(playerBase, npcBase);
                
                // Draw bases
                DrawRectangleRec(playerBase.hitbox, BLUE);
                DrawRectangleRec(npcBase.hitbox, RED);

                // Draw units
                DrawUnits(playerUnits);
                DrawUnits(npcUnits);
                break;

            case PAUSE_SCREEN:
                DrawText("Game Paused", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 30, BLACK);
                DrawText("Press 'P' to Resume", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 20, DARKGRAY);
                break;

            case END_SCREEN:
                DrawText(playerBase.health <= 0 ? "NPC Wins!" : "Player Wins!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 30, RED);
                DrawText("Press 'R' to Restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 20, DARKGRAY);
                break;
        }

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}
