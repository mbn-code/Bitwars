#include "AudioInitializer.hpp"

////////////////////////////////////////////////////////////
//
//  @brief Function to initialize and play background music
//
//  @def Music init_and_play_background_music(const char* file_name)
//
//  @details Function to initialize and play background music   
//

Music init_and_play_background_music(const char* file_name) {
    InitAudioDevice();  // Initialize audio device
    Music background_music = LoadMusicStream(file_name);  // Load music
    background_music.looping = true;  // Enable looping
    SetMusicVolume(background_music, 0.2f);  // Set volume to half
    PlayMusicStream(background_music);  // Play music
    return background_music;
}

