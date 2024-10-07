#ifndef GAMEDATASTRUCTURES_HPP
#define GAMEDATASTRUCTURES_HPP

#ifdef _WIN32
#include "../lib/windows/raylib.h"
#else
#ifdef _APPLE_
#include "raylib.h"
#endif
#endif // _APPLE_
// raylib for rectangle simplification in structures

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
    TANK,
    BIT
};

// Struct for Unit
struct Unit {
    Rectangle hitbox;
    int health;
    int damage;
    UnitType type;
    int speed;
    bool isPlayerUnit;
    

    bool operator==(const Unit& other) const {
        return this->hitbox.x == other.hitbox.x &&
               this->hitbox.y == other.hitbox.y &&
               this->hitbox.width == other.hitbox.width &&
               this->hitbox.height == other.hitbox.height &&
               this->health == other.health &&
               this->damage == other.damage &&
               this->type == other.type;
    }
};

// Struct for Base
struct Base {
    Rectangle hitbox;
    int health;
    int points;
    Vector2 position;
};

#endif // GAMEDATASTRUCTURES_HPP