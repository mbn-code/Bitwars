#ifndef AUDIOINITIALIZER_HPP
#define AUDIOINITIALIZER_HPP

#ifdef _WIN32
#include "../src/main.hpp"
#elif __APPLE__
#include "src/main.hpp"
#endif


Music init_and_play_background_music(const char* file_name);

#endif // AUDIOINITIALIZER_HPP