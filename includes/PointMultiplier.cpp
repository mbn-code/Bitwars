#include "PointMultiplier.hpp"
#include <cmath>

float PointMultiplier::CalculatePointMultiplier(int health) {
    // Exponential implementation
    return std::exp((100 - health) / 100.0f);
}