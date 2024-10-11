#include "main.hpp"

// Function Prototypes
void spawn_unit(std::vector<Unit>& units, Base& base, UnitType unit_type, bool is_player_unit);
void update_units(std::vector<Unit>& units, Base& opponent_base);
void draw_units(const std::vector<Unit>& units);
void draw_health_bar(const Unit& unit);
void draw_improved_ui(const Base& player_base, const Base& npc_base);
void draw_bases(const Base& player_base, const Base& npc_base);
void draw_health_bar_for_base(const Base& base, int base_x, Color color);
void draw_timer(float elapse_time);

// Global Variables
GameState current_state = START_SCREEN;
Base player_base = {{50, SCREEN_HEIGHT / 2 - 50, 50, 100}, PLAYER_BASE_HEALTH, PLAYER_BASE_INITIAL_POINTS};
Base npc_base = {{SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 50, 50, 100}, NPC_BASE_HEALTH, NPC_BASE_INITIAL_POINTS};
std::vector<Unit> player_units;
std::vector<Unit> npc_units;
int point_tick_counter = 0;

float elapse_time = 0.0f;
bool game_won = false;
float win_time = 0.0f;

float lastScore = ReadLastScore();
Texture2D backgroundTexture;

// Function to initialize and play background music
Music init_and_play_background_music(const char* file_name) {
    InitAudioDevice();  // Initialize audio device
    Music background_music = LoadMusicStream(file_name);  // Load music
    background_music.looping = true;  // Enable looping
    SetMusicVolume(background_music, 0.5f);  // Set volume to half
    PlayMusicStream(background_music);  // Play music
    return background_music;
}

bool is_first_time() {
	if (const std::ifstream file("first_time_flag.txt"); file.good()) {
        return false;
    }
    else {
        std::ofstream outfile("first_time_flag.txt");
        outfile << "This file indicates the game has been opened before.";
        outfile.close();
        return true;
    }
}

// Function Implementations             // base can't be const because it's health is being modified in the function
void SpawnUnit(std::vector<Unit>& units, Base& base, UnitType unit_type, bool is_player_unit) {
    Unit newUnit;
    newUnit.type = unit_type;
    newUnit.isPlayerUnit = is_player_unit;

    // Set unit properties based on type
    switch (unit_type) {
        case TYPE_1_SOLDIER:
            newUnit.hitbox = {is_player_unit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 20, base.hitbox.y + base.hitbox.height / 2 - 10, 20, 20};
            newUnit.health = 10;
            newUnit.damage = 5;
            newUnit.speed = UNIT_SPEED;
            break;
        case TYPE_2_SOLDIER:
            newUnit.hitbox = {is_player_unit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 20, base.hitbox.y + base.hitbox.height / 2 - 15, 20, 30};
            newUnit.health = 20;
            newUnit.damage = 10;
            newUnit.speed = UNIT_SPEED;
            break;
        case TYPE_3_SOLDIER:
            newUnit.hitbox = {is_player_unit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 30, base.hitbox.y + base.hitbox.height / 2 - 20, 30, 40};
            newUnit.health = 30;
            newUnit.damage = 15;
            newUnit.speed = UNIT_SPEED - 0.5f;  // Slightly slower speed
            break;
        case TANK:
            newUnit.hitbox = {is_player_unit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 40, base.hitbox.y + base.hitbox.height / 2 - 20, 40, 40};
            newUnit.health = 50;
            newUnit.damage = 20;
            newUnit.speed = UNIT_SPEED - 1.0f;  // Slow speed
            break;
        case BIT:
            newUnit.hitbox = {is_player_unit ? base.hitbox.x + base.hitbox.width : base.hitbox.x - 20, base.hitbox.y + base.hitbox.height / 2 - 10, 20, 20};
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
            it = units.erase(it); 
        }
        else {
            ++it;
        }

    }
}

void draw_health_bar(const Unit& unit) {
    const float health_bar_width = static_cast<float>(unit.hitbox.width);  // Ensure width is treated as float
    const float health_percentage = static_cast<float>(unit.health) /
        static_cast<float>(unit.type == TYPE_1_SOLDIER ? 10 :
            unit.type == TYPE_2_SOLDIER ? 20 :
            unit.type == TYPE_3_SOLDIER ? 30 : 50); // Explicit casting to avoid narrowing

    Color health_color = health_percentage > 0.5f ? GREEN :
        health_percentage > 0.25f ? YELLOW : RED; // Use floating-point literals

    // Draw health bar above the unit
    DrawRectangle(static_cast<int>(unit.hitbox.x), static_cast<int>(unit.hitbox.y - 10),
        static_cast<int>(health_bar_width * health_percentage), 5, health_color);

    // Draw health bar border
    DrawRectangleLines(static_cast<int>(unit.hitbox.x), static_cast<int>(unit.hitbox.y - 10),
        static_cast<int>(health_bar_width), 5, BLACK);

    // Draw health bar for tank specifically
    if (unit.type == TANK) {
        DrawRectangle(static_cast<int>(unit.hitbox.x), static_cast<int>(unit.hitbox.y - 15),
            static_cast<int>(health_bar_width * health_percentage), 5, health_color);
        DrawRectangleLines(static_cast<int>(unit.hitbox.x), static_cast<int>(unit.hitbox.y - 15),
            static_cast<int>(health_bar_width), 5, BLACK);
    }
    else if (unit.type == BIT) {
        DrawRectangle(static_cast<int>(unit.hitbox.x), static_cast<int>(unit.hitbox.y - 5),
            static_cast<int>(health_bar_width * health_percentage), 5, health_color);
        DrawRectangleLines(static_cast<int>(unit.hitbox.x), static_cast<int>(unit.hitbox.y - 5),
            static_cast<int>(health_bar_width), 5, BLACK);
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
            draw_health_bar(unit);
        }
    }
}


void DrawBases(const Base& player_base, const Base& npc_base, const float player_scale, const float npc_scale, const Vector2 player_position, const Vector2 npc_position) {
    // Draw player and NPC bases with textures, scale, and position
    DrawTextureEx(player_base.texture, player_position, 0.0f, player_scale, WHITE);
    DrawTextureEx(npc_base.texture, npc_position, 0.0f, npc_scale, WHITE);
}

void draw_health_bar_for_base(const Base& base, const int base_x, const Color color) {
    DrawRectangle(base_x, 20, 200, 30, BLACK);  // Background bar
    DrawRectangle(base_x, 20, 200 * static_cast<float>(base.health) / PLAYER_BASE_HEALTH, 30, color);  // Health bar
}

void draw_improved_ui(const Base& player_base, const Base& npc_base) {
    draw_health_bar_for_base(player_base, 20, BLUE);
    draw_health_bar_for_base(npc_base, SCREEN_WIDTH - 220, RED);

	// NPC POINTS AND PLAYER POINTS
    DrawRectangle(15, 95, 200, 30, LIGHTGRAY); // Box for Player Points
    DrawRectangle(SCREEN_WIDTH - 225, 95, 200, 30, LIGHTGRAY); // Box for NPC Points

    DrawText(TextFormat("Player Points: %d", player_base.points), 20, 100, 20, BLACK);
    DrawText(TextFormat("NPC Points: %d", npc_base.points), SCREEN_WIDTH - 220, 100, 20, BLACK);

    // Draw a semi-transparent box behind the text for better readability
    DrawRectangle(10, SCREEN_HEIGHT - 210, 320, 180, Fade(LIGHTGRAY, 0.7f));

    DrawText("Unit Purchase Options:", 20, SCREEN_HEIGHT - 200, 20, BLACK);
    DrawText("[1] Type 1 Soldier - 5 Points", 20, SCREEN_HEIGHT - 170, 20, BLACK);
    DrawText("[2] Type 2 Soldier - 10 Points", 20, SCREEN_HEIGHT - 140, 20, BLACK);  
    DrawText("[3] Type 3 Soldier - 30 Points", 20, SCREEN_HEIGHT - 110, 20, BLACK);
    DrawText("[4] Tank - 50 Points", 20, SCREEN_HEIGHT - 80, 20, BLACK);
    DrawText("[5] Bit - 2 Points", 20, SCREEN_HEIGHT - 50, 20, BLACK);
}

void draw_centered_text(const char* text, int posX, int posY, int fontSize, Color color) {
    DrawText(text, posX - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}

void draw_timer(const float elapse_time) {
    draw_centered_text(TextFormat("Time: %.2f", elapse_time), SCREEN_WIDTH/2, 30, 20, BLACK);
}

int main() {

	CheatChecker cheatChecker(&player_base, &npc_base, &player_units, &npc_units);

	bool first_time = is_first_time();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bitwars");

    SetTargetFPS(FRAME_RATE);

    // Load textures based on platform-specific paths
    npc_base.texture = LoadTexture(BASE_TEXTURE_PATH);
    if (npc_base.texture.id == 0) {
        std::cerr << "Failed to load NPC base texture! Path might be wrong." << std::endl;
        return -1;
    }

    player_base.texture = LoadTexture(BASE2_TEXTURE_PATH);
    if (player_base.texture.id == 0) {
        std::cerr << "Failed to load Player base texture! Path might be wrong." << std::endl;
        return -1;
    }

    backgroundTexture = LoadTexture(BACKGROUND_TEXTURE_PATH);
    if (backgroundTexture.id == 0) {
        std::cerr << "Failed to load background texture! Path might be wrong." << std::endl;
        return -1;
    }


	#ifdef _WIN32
    Music background_music = init_and_play_background_music("../assets/Sound/Default_Sound_Track.wav");
	#elif __APPLE__
	Music background_music = InitAndPlayBackgroundMusic("assets/Sound/Mac_Default_Sound_Track.wav");
	#else
	Music background_music = InitAndPlayBackgroundMusic("MUSIC_PATH");
	#endif


    while (!WindowShouldClose()) {


        // Define scale and position for bases
        float playerScale = 0.10f;
        float npcScale = 0.10f;
        Vector2 playerPosition = {player_base.hitbox.x, player_base.hitbox.y};
        Vector2 npcPosition = {npc_base.hitbox.x, npc_base.hitbox.y};


        UpdateMusicStream(background_music);  // Update music stream

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


                // NPC logic for buying units based on player's actions and available points
                if (isOffensive) {
                    // Offensive mode: NPC spawns more aggressive units
                    if (player_units.size() > npc_units.size() && npc_base.points >= 50) {  // If player has more units, spawn tanks
                        SpawnUnit(npc_units, npc_base, TANK, false);
                        npc_base.points -= 50;
                    } else if (player_units.size() > npc_units.size() && npc_base.points >= 30) {  // If player has more units, spawn Type 3 soldiers
                        SpawnUnit(npc_units, npc_base, TYPE_3_SOLDIER, false);
                        npc_base.points -= 30;
                    } else if (npc_base.points >= 10) {  // Default to spawning Type 2 soldiers
                        SpawnUnit(npc_units, npc_base, TYPE_2_SOLDIER, false);
                        npc_base.points -= 10;
                    } else if (npc_base.points >= 5) {  // Default to spawning Type 1 soldiers
                        SpawnUnit(npc_units, npc_base, TYPE_1_SOLDIER, false);
                        npc_base.points -= 5;
                    } else if (npc_base.points >= 2) {  // Default to spawning Bits
                        SpawnUnit(npc_units, npc_base, BIT, false);
                        npc_base.points -= 2;
                    }
                } else {
                    // Defensive mode: NPC spawns units based on player's unit types
                    if (std::count_if(player_units.begin(), player_units.end(), [](const Unit& unit) { return unit.type == TANK; }) > 0 && npc_base.points >= 50) {  // If player has tanks, spawn tanks
                        SpawnUnit(npc_units, npc_base, TANK, false);
                        npc_base.points -= 50;
                    } else if (std::count_if(player_units.begin(), player_units.end(), [](const Unit& unit) { return unit.type == TYPE_3_SOLDIER; }) > 0 && npc_base.points >= 30) {  // If player has Type 3 soldiers, spawn Type 3 soldiers
                        SpawnUnit(npc_units, npc_base, TYPE_3_SOLDIER, false);
                        npc_base.points -= 30;
                    } else if (npc_base.points >= 10) {  // Default to spawning Type 2 soldiers
                        SpawnUnit(npc_units, npc_base, TYPE_2_SOLDIER, false);
                        npc_base.points -= 10;
                    } else if (npc_base.points >= 5) {  // Default to spawning Type 1 soldiers
                        SpawnUnit(npc_units, npc_base, TYPE_1_SOLDIER, false);
                        npc_base.points -= 5;
                    } else if (npc_base.points >= 2) {  // Default to spawning Bits
                        SpawnUnit(npc_units, npc_base, BIT, false);
                        npc_base.points -= 2;
                    }
                }

                /* 
                @brief Award points to both players over time
                
                @detail 
                The point_tick_counter variable is used to keep track of the number of frames that have passed since the last point award.
                The POINTS_INTERVAL constant is used to determine the number of frames that must pass before points are awarded.
                The POINTS_PER_TICK constant is used to determine the number of points awarded to each player per tick.
                The player_base and npc_base points are incremented by POINTS_PER_TICK if the point_tick_counter is greater than or equal to POINTS_INTERVAL.

                Basically every 60 frames (1 second), the player and NPC bases are awarded POINTS_PER_TICK points. Because when the point_tick_counter reaches 60, the points are awarded and the counter is reset to 0. This is done to prevent points from being awarded every frame, which would be too fast. This is a simple way to implement a time-based point award system.

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
                |  Check if point_tick_counter|
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
                |   player_base.points       |
                |   Add POINTS_PER_TICK to  |
                |   npc_base.points          |
                +---------------------------+
                            |
                            V
                +---------------------------+
                |  Reset point_tick_counter to |
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

        DrawTexture(backgroundTexture, 0, 0, WHITE);


        // Draw game elements based on the current state. Seperated from the switch case above for better readability.
        // And everything here is pretty straight forward.
        switch (current_state) {
	        case START_SCREEN:
	            // Draw a semi-transparent green background for better readability
	            DrawRectangle(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 60, 300, 120, Fade(LIGHTGRAY, 0.5f));

	            // Draw welcome text
	            DrawText("Welcome to Bitwars!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 20, BLACK);
	            DrawText("Press ENTER to start", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, 20, BLACK);

	            // If it's the first time, show the tutorial
	            if (first_time) {
	                DrawText("Controls:", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 60, 20, BLACK);
	                DrawText("WASD - Move", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 80, 20, BLACK);
	                DrawText("Mouse - Aim", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, 20, BLACK);
	                DrawText("Left Click - Shoot", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 120, 20, BLACK);
	                DrawText("Description:", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 160, 20, BLACK);
	                DrawText("Bitwars is a strategy game where you control units", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 180, 20, BLACK);
	                DrawText("to defeat the enemy base. Use your resources wisely", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 200, 20, BLACK);
	                DrawText("and outsmart your opponent!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 220, 20, BLACK);
	            }

	            if (IsKeyPressed(KEY_ENTER)) {
	                current_state = GAMEPLAY;
	            }
	            break;


            case GAMEPLAY:
                DrawTexture(backgroundTexture, 0, 0, WHITE);
                DrawBases(player_base, npc_base, playerScale, npcScale, playerPosition, npcPosition);
                DrawUnits(player_units);
                DrawUnits(npc_units);
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
    UnloadMusicStream(background_music);
    CloseAudioDevice();
    
    // Unload textures
    UnloadTexture(player_base.texture);
    UnloadTexture(npc_base.texture);
    UnloadTexture(backgroundTexture);
    
    CloseWindow();
    
    return 0;
}