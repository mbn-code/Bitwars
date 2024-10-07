#include "PointMultiplier.hpp"

float PointMultiplier::CalculatePointMultiplier(int health) {
    // Linear inverse implementation for higher multiplier with lower health
    return 1.0f + (100 - health) / 100.0f;
}
