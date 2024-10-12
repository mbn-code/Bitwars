#include "main.hpp"
#include "../lib/GLFW/glfw3.h"
#include "../assets/generated/src/generated/background_png.h"
#include "../assets/generated/src/generated/Base2_texture_BR_png.h"
#include "../assets/generated/src/generated/Base_texture_BR_png.h"
#include "../assets/generated/src/generated/Default_Sound_Track_wav.h"

// Function Prototypes
void spawn_unit(std::vector<Unit>& units, Base& base, UnitType unit_type, bool is_player_unit);
void update_units(std::vector<Unit>& units, Base& opponent_base);
void draw_units(const std::vector<Unit>& units);
void draw_health_bar(const Unit& unit);
void draw_improved_ui(const Base& player_base, const Base& npc_base);
void draw_bases(const Base& player_base, const Base& npc_base, float player_scale, float npc_scale, Vector2 player_position, Vector2 npc_position);
void draw_health_bar_for_base(const Base& base, int base_x, Color color);
void draw_timer(float elapse_time);

void spawn_unit(std::vector<Unit>& units, Base& base, UnitType unit_type, bool is_player_unit) {
    Unit new_unit;
    new_unit.type = unit_type;
    new_unit.isPlayerUnit = is_player_unit;
    new_unit.hitbox = { base.hitbox.x, base.hitbox.y, 20, 20 }; // Example hitbox size
    new_unit.health = 100; // Example health value
    new_unit.damage = 10; // Example damage value
    new_unit.speed = 5; // Example speed value

    units.push_back(new_unit);
}

void update_units(std::vector<Unit>& units, Base& opponent_base) {
    for (auto& unit : units) {
        // Move unit towards the opponent base
        if (unit.isPlayerUnit) {
            unit.hitbox.x += unit.speed * GetFrameTime();
        }
        else {
            unit.hitbox.x -= unit.speed * GetFrameTime();
        }

        // Check for collision with opponent base
        if (CheckCollisionRecs(unit.hitbox, opponent_base.hitbox)) {
            opponent_base.health -= unit.damage;
            unit.health = 0; // Unit is destroyed after attacking
        }
    }

    // Remove destroyed units
    units.erase(std::remove_if(units.begin(), units.end(), [](const Unit& unit) {
        return unit.health <= 0;
        }), units.end());
}


// Global Variables
GameState current_state = START_SCREEN;
Base player_base = { {50, SCREEN_HEIGHT / 2 - 50, 50, 100}, PLAYER_BASE_HEALTH, PLAYER_BASE_INITIAL_POINTS };
Base npc_base = { {SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 50, 50, 100}, NPC_BASE_HEALTH, NPC_BASE_INITIAL_POINTS };
std::vector<Unit> player_units;
std::vector<Unit> npc_units;
int point_tick_counter = 0;

float elapse_time = 0.0f;
bool game_won = false;
float win_time = 0.0f;

Texture2D background_texture;

int main() {


	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bitwars");
    SetTargetFPS(FRAME_RATE);

    // Load textures from memory
    Image baseTextureImage = LoadImageFromMemory(".png", Base_texture_BR_png, Base_texture_BR_png_len);
    if (baseTextureImage.data == NULL) {
        std::cerr << "Failed to load Base_texture_BR.png" << std::endl;
        return -1;
    }
    Texture2D baseTexture = LoadTextureFromImage(baseTextureImage);

    Image base2TextureImage = LoadImageFromMemory(".png", Base2_texture_BR_png, Base2_texture_BR_png_len);
    if (base2TextureImage.data == NULL) {
        std::cerr << "Failed to load Base2_texture_BR.png" << std::endl;
        return -1;
    }
    Texture2D base2Texture = LoadTextureFromImage(base2TextureImage);

    Image backgroundTextureImage = LoadImageFromMemory(".png", background_png, background_png_len);
    if (backgroundTextureImage.data == NULL) {
        std::cerr << "Failed to load background.png" << std::endl;
        return -1;
    }
    background_texture = LoadTextureFromImage(backgroundTextureImage);

    // Assign textures to bases
    player_base.texture = baseTexture;
    npc_base.texture = base2Texture;

    while (!WindowShouldClose()) {

        // Define scale and position for bases
        float playerScale = 0.10f;
        float npcScale = 0.10f;
        Vector2 playerPosition = {player_base.hitbox.x, player_base.hitbox.y};
        Vector2 npcPosition = {npc_base.hitbox.x, npc_base.hitbox.y};

        //UpdateMusicStream(background_music);  // Update music stream

        if (current_state == GAMEPLAY && !game_won) {
            elapse_time += GetFrameTime();
        }
        
        bool isOffensive = npc_base.health > player_base.health;  // Go offensive if NPC base has more health

        // logical switch case statements based on game logic and player input
        switch (current_state) {
            case START_SCREEN:
                if (IsKeyPressed(KEY_ENTER)) {

                    player_base.health = PLAYER_BASE_HEALTH;
                    npc_base.health = NPC_BASE_HEALTH;
                    player_base.points = PLAYER_BASE_INITIAL_POINTS;
                    npc_base.points = NPC_BASE_INITIAL_POINTS;
                    player_units.clear();
                    npc_units.clear();
                    
                    elapse_time = 0.0f;
                    game_won = false;
                    current_state = GAMEPLAY;

                }
                break;

            case GAMEPLAY:
                point_tick_counter++;

                // Player input for buying units
                if (IsKeyPressed(KEY_ONE) && player_base.points >= 5) {
                    SpawnUnit(player_units, player_base, TYPE_1_SOLDIER, true);
                    player_base.points -= 5;
                }
                if (IsKeyPressed(KEY_TWO) && player_base.points >= 10) {
                    SpawnUnit(player_units, player_base, TYPE_2_SOLDIER, true);
                    player_base.points -= 10;
                }
                if (IsKeyPressed(KEY_THREE) && player_base.points >= 30) {
                    SpawnUnit(player_units, player_base, TYPE_3_SOLDIER, true);
                    player_base.points -= 30;
                }
                if (IsKeyPressed(KEY_FOUR) && player_base.points >= 50) {
                    SpawnUnit(player_units, player_base, TANK, true);
                    player_base.points -= 50;
                }
                if (IsKeyPressed(KEY_FIVE) && player_base.points >= 2) {
                    SpawnUnit(player_units, player_base, BIT, true);
                    player_base.points -= 2;
                }


                npc_logic_update(npc_units, npc_base, player_units, isOffensive);

                /* 
                @brief Award points to both players over time
                
                @detail 
                The point_tick_counter variable is used to keep track of the number of frames that have passed since the last point award.
                The POINTS_INTERVAL constant is used to determine the number of frames that must pass before points are awarded.
                The POINTS_PER_TICK constant is used to determine the number of points awarded to each player per tick.
                The player_base and npc_base points are incremented by POINTS_PER_TICK if the point_tick_counter is greater than or equal to POINTS_INTERVAL.

                Basically every 60 frames (1 second), the player and NPC bases are awarded POINTS_PER_TICK points. Because when the point_tick_counter reaches 60, the points are awarded and the counter is reset to 0. This is done to prevent points from being awarded every frame, which would be too fast. This is a simple way to implement a time-based point award system.
                */

                // Update units and check collisions
                HandleCombat::HandleGameCombat(player_units, npc_units, player_base, npc_base);
                HandleCombat::HandleGameCombat(npc_units, player_units, npc_base, player_base);

                // move the units towards the base 
                UpdateUnits(player_units, npc_base);
                UpdateUnits(npc_units, player_base);

                if (point_tick_counter >= POINTS_INTERVAL) {
                    player_base.points += POINTS_PER_TICK;
                    npc_base.points += POINTS_PER_TICK;
                    point_tick_counter = 0;
                }

                // Check if any base is destroyed
                /* 
                @brief Check if either base has been destroyed and update game state

                @details
                If either the player's or NPC's base health reaches zero, the game state is updated to END_SCREEN.
                The game is won by the player if the NPC's base health reaches zero, and the game is won by the NPC if the player's base health reaches zero.
                The win_time variable is used to store the time at which the game was won, which is displayed on the end screen.
                */
                if (player_base.health <= 0 || npc_base.health <= 0) {
                    current_state = END_SCREEN;
                    if (!game_won) {
                        game_won = true;
                        win_time = elapse_time;
                    }
                }
                
                break;

            case PAUSE_SCREEN:
                if (IsKeyPressed(KEY_P)) {
                    current_state = GAMEPLAY;
                }
                break;

            case END_SCREEN:
                if (IsKeyPressed(KEY_ENTER)) {
                    current_state = START_SCREEN;
                }
                break;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(background_texture, 0, 0, WHITE);


        // Draw game elements based on the current state. Seperated from the switch case above for better readability.
        // And everything here is pretty straight forward.
        switch (current_state) {
            case START_SCREEN:
                // Draw a semi-transparent green background for better readability
                DrawRectangle(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 60, 300, 120, Fade(LIGHTGRAY, 0.5f));
                
                // Draw welcome text
                DrawText("Welcome to Bitwars!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, BLACK);
                DrawText("Press ENTER to start", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, 20, BLACK);

                DrawRectangle(500, SCREEN_HEIGHT - 220, SCREEN_WIDTH / 2, 250, LIGHTGRAY);

                // Draw the text within the box
                DrawText("Controls:", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 220 + 20, 20, BLACK);
                DrawText("Key 1 through 5 - Send Units", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 220 + 40, 20, BLACK);
                DrawText("Description:", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 220 + 120, 20, BLACK);
                DrawText("Bitwars is a strategy game where you control units", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 220 + 140, 20, BLACK);
                DrawText("to defeat the enemy base. Use your resources wisely", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 220 + 160, 20, BLACK);
                DrawText("and outsmart your opponent!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 220 + 180, 20, BLACK);

        	break;

            case GAMEPLAY:
                DrawTexture(background_texture, 0, 0, WHITE);
                draw_bases(player_base, npc_base, playerScale, npcScale, playerPosition, npcPosition);
                draw_units(player_units);
                draw_units(npc_units);
                draw_improved_ui(player_base, npc_base);
                draw_timer(elapse_time);
                break;

            case PAUSE_SCREEN:
                DrawText("Game Paused", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, 20, GRAY);
                break;

            case END_SCREEN:
                if (IsKeyPressed(KEY_ENTER)) {
                    current_state = START_SCREEN;
                }
                
                if (player_base.health <= 0) {
                    DrawText("Game Over! You Lost!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, GRAY);
                } else if (npc_base.health <= 0) {
                    DrawText("Congratulations! You Won!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, BLACK);
                }
                DrawText(TextFormat("Time: %.2f seconds", win_time), SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 20, BLACK);
                DrawText("Press ENTER to restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 20, BLACK);

                // Check if the game is won and write the score
                if (!game_won) {
                    game_won = true;
                    win_time = elapse_time; // Capture the win time
                    WriteLastScore(win_time); // Write score only when game is won
                }
            break;
            }

    	EndDrawing();
    }
    //UnloadMusicStream(background_music);
    // CloseAudioDevice();
    
    // Unload textures
    UnloadTexture(baseTexture);
    UnloadTexture(base2Texture);
    UnloadTexture(background_texture);
    
    CloseWindow();
    
    return 0;
}