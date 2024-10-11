#include "UnitHandler.hpp"

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