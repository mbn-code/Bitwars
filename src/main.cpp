#include "main.hpp"


// Constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 900
#define PLAYER_BASE_HEALTH 100
#define NPC_BASE_HEALTH 100
#define UNIT_SPEED 2.0f
#define POINTS_PER_TICK 1
#define POINTS_INTERVAL 60
#define PLAYER_BASE_INITIAL_POINTS 50
#define NPC_BASE_INITIAL_POINTS 50

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
bool winTime = 0.0f;

float lastScore = ReadLastScore();

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
    }

    units.push_back(newUnit);
}

void UpdateUnits(std::vector<Unit>& units, Base& opponentBase, std::vector<Unit>& opponentUnits) {
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

        // Check for collisions with opponent units
        for (auto& opponentUnit : opponentUnits) {
            if (CheckCollisionRecs(unit.hitbox, opponentUnit.hitbox)) {
                // Inflict damage on both units
                unit.health -= opponentUnit.damage;
                opponentUnit.health -= unit.damage;

                // If either unit's health is less than or equal to zero, they are removed
                if (unit.health <= 0) {
                    unit.health = 0; // Ensure health doesn't go negative
                    if (opponentUnit.type == TANK) {
                        opponentBase.points += 10;  // Award points to the opponent for killing a player unit
                    } else {
                        opponentBase.points += 5;  // Award points to the opponent for killing a player unit
                    }
                }
                if (opponentUnit.health <= 0) {
                    opponentUnit.health = 0; // Ensure health doesn't go negative
                    if (unit.type == TANK) {
                        playerBase.points += 10;  // Award points to the player for killing an opponent unit
                    } else {
                        playerBase.points += 5;  // Award points to the player for killing an opponent unit
                    }
                }
            }
        }
    }

    // Remove units with zero or less health
    units.erase(std::remove_if(units.begin(), units.end(), [](const Unit& u) { return u.health <= 0; }), units.end());
    opponentUnits.erase(std::remove_if(opponentUnits.begin(), opponentUnits.end(), [](const Unit& u) { return u.health <= 0; }), opponentUnits.end());
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
    }

}

void DrawUnits(const std::vector<Unit>& units) {
    for (const auto& unit : units) {
        Color unitColor = unit.isPlayerUnit ? BLUE : RED;
        DrawRectangleRec(unit.hitbox, unitColor);
        DrawHealthBar(unit);
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

    DrawText("Unit Purchase Options:", 20, SCREEN_HEIGHT - 150, 20, DARKGRAY);
    DrawText("[1] Type 1 Soldier - 5 Points", 20, SCREEN_HEIGHT - 120, 20, DARKGRAY);
    DrawText("[2] Type 2 Soldier - 10 Points", 20, SCREEN_HEIGHT - 90, 20, DARKGRAY);
    DrawText("[3] Type 3 Soldier - 30 Points", 20, SCREEN_HEIGHT - 60, 20, DARKGRAY);
    DrawText("[4] Tank - 50 Points", 20, SCREEN_HEIGHT - 30, 20, DARKGRAY);
}

void DrawCenteredText(const char* text, int posX, int posY, int fontSize, Color color) {
    DrawText(text, posX - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}

void DrawTimer(float elapsedTime) {
    DrawCenteredText(TextFormat("Time: %.2f", elapsedTime), SCREEN_WIDTH/2, SCREEN_WIDTH/2, 20, BLACK);
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bitwars");

    SetTargetFPS(60);

while (!WindowShouldClose()) {
    if (currentState == GAMEPLAY && !gameWon) {
        elapsedTime += GetFrameTime();
    }
    
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

            // NPC randomly spawns units based on its points
            if (GetRandomValue(0, 100) < 2) {  // NPC spawns units at random intervals
                UnitType randomType = static_cast<UnitType>(GetRandomValue(0, 3));  // Randomly select a unit type
                int cost = (randomType == TYPE_1_SOLDIER) ? 5 : (randomType == TYPE_2_SOLDIER) ? 10 : (randomType == TYPE_3_SOLDIER) ? 30 : 50;
                if (npcBase.points >= cost) {
                    SpawnUnit(npcUnits, npcBase, randomType, false);
                    npcBase.points -= cost;
                }
            }

            /* 
            @brief Award points to both players over time
            */
            if (pointTickCounter >= POINTS_INTERVAL) {
                playerBase.points += POINTS_PER_TICK;
                npcBase.points += POINTS_PER_TICK;
                pointTickCounter = 0;
            }


            // Update units and check collisions
            UpdateUnits(playerUnits, npcBase, npcUnits);
            UpdateUnits(npcUnits, playerBase, playerUnits);

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

    switch (currentState) {
        case START_SCREEN:
            DrawText("Welcome to Bitwars!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, BLACK);
            
            DrawText("Press ENTER to start", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, 20, DARKGRAY);

            DrawText(TextFormat("Last Score: %.2f seconds", lastScore), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 60, 20, DARKGRAY);            break;


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
            DrawText(playerBase.health <= 0 ? "You Lost!" : "You Won!", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, 20, BLACK);
            DrawText("Press ENTER to return to start", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20, 20, DARKGRAY);
            
            if (!gameWon) {
                gameWon = true;
                winTime = elapsedTime;
                WriteLastScore(winTime);
            }
            
            break;
    }

    EndDrawing();

    }

    CloseWindow();
    return 0;
}
