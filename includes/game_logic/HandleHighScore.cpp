#include <fstream>
#include <iostream>

float ReadLastScore() {
    std::ifstream file("last_score.txt");
    float lastScore = 0.0f;
    if (file.is_open()) {
        file >> lastScore;
        file.close();
        std::cout << "Read last score: " << lastScore << std::endl;
    } else {
        std::cout << "Failed to open last_score.txt for reading" << std::endl;
    }
    return lastScore;
}

void WriteLastScore(float score) {
    std::ofstream file("last_score.txt");
    if (file.is_open()) {
        file << score;
        file.close();
        std::cout << "Wrote last score: " << score << std::endl;
    } else {
        std::cout << "Failed to open last_score.txt for writing" << std::endl;
    }
}