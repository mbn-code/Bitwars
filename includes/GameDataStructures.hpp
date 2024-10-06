#ifndef GAMEDATASTRUCTURES_HPP
#define GAMEDATASTRUCTURES_HPP

#include <raylib.h>

// Define Game States
enum GameState {
    START_SCREEN,
    GAMEPLAY,
    PAUSE_SCREEN,
    END_SCREEN
};

// Define Unit Types
enum UnitType {
    TYPE_1_SOLDIER,
    TYPE_2_SOLDIER,
    TYPE_3_SOLDIER,
    TANK
};

// Struct for Unit
struct Unit {
    Rectangle hitbox;  // Hitbox representing the unit
    int health;        // Health of the unit
    int damage;        // Damage the unit deals
    UnitType type;     // Type of unit (Type 1, 2, 3, Tank)
    float speed;       // Speed of the unit
    bool isPlayerUnit; // True if the unit belongs to the player, false for NPC
};

// Struct for Base
struct Base {
    Rectangle hitbox;  // Hitbox representing the base
    int health;        // Health of the base
    int points;        // Points the player or NPC has
};

#endif // GAMEDATASTRUCTURES_HPP