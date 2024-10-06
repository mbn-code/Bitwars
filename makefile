# Detect the platform from a passed variable or use default (macOS)
PLATFORM ?= macos

# Define the compiler
COMPILER = g++

# Include paths for headers and Raylib
SOURCE_LIBS = -Ilib/ -Iincludes/

# Output binaries
WIN_OUT = -o "bin/build_windows.exe"
MACOS_OUT = -o "bin/build_osx"

# Source files
CFILES = src/*.cpp includes/HandleCombat.cpp includes/PointMultiplier.cpp includes/HandleHighScore.cpp

# Platform-specific compiler and linker options
ifeq ($(PLATFORM), windows)
    COMPILER = g++  # or clang++ if using MinGW's clang
    PLATFORM_OPTS = -Llib/ -lraylib -lopengl32 -lgdi32 -lwinmm
    OUT_FILE = $(WIN_OUT)
    RM = del /f /q
else ifeq ($(PLATFORM), macos)
    COMPILER = clang++  # macOS compiler
    PLATFORM_OPTS = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
    OUT_FILE = $(MACOS_OUT)
    RM = rm -f
endif

# Default target
all: build

# Build rule based on platform
build:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OUT_FILE) $(PLATFORM_OPTS) -std=c++11

# Clean up generated binaries
clean:
	$(RM) bin/build_windows.exe bin/build_osx
