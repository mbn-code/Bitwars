#include "Renderer.hpp"


/* @brief Draw all units on the screen
 *
*/

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

void draw_units(const std::vector<Unit>& units) {
    for (const auto& unit : units) {
        if (unit.type == BIT) {
                DrawRectangle(static_cast<int>(unit.hitbox.x), static_cast<int>(unit.hitbox.y), 5, 5, unit.isPlayerUnit ? BLUE : RED);
        } else {
            DrawRectangle(unit.hitbox.x, unit.hitbox.y, unit.hitbox.width, unit.hitbox.height, unit.isPlayerUnit ? BLUE : RED);
            draw_health_bar(unit);
        }
    }
}


void draw_bases(const Base& player_base, const Base& npc_base, const float player_scale, const float npc_scale, const Vector2 player_position, const Vector2 npc_position) {
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