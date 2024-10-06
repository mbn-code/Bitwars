#ifndef HANDLEHIGHSCORE_HPP
#define HANDLEHIGHSCORE_HPP

#include <fstream>
#include <string>

// Function to read the last score from a file
float ReadLastScore();

// Function to write the last score to a file
void WriteLastScore(float score);

#endif // HANDLEHIGHSCORE_HPP