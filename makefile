# Detect the platform from a passed variable or use default (macOS)
PLATFORM ?= macos

# Compiler definitions
ifeq ($(PLATFORM), windows)
    COMPILER = x86_64-w64-mingw32-g++
else
    COMPILER = g++
endif

STD = -std=c++17

# Include paths for headers and Raylib
SOURCE_LIBS = -Ilib/ -Iincludes

# Output binaries
WIN_OUT = -o "bin/Bitwars.exe"
MACOS_OUT = -o "bin/Bitwars"

# Source files
CFILES = src/*.cpp includes/AI/AIController.cpp includes/Audio/AudioInitializer.cpp includes/Graphics/Renderer.cpp includes/game_logic/UnitHandler.cpp includes/game_logic/HandleCombat.cpp includes/game_logic/PointMultiplier.cpp includes/game_logic/HandleHighScore.cpp includes/cheat_detection/CheatChecker.cpp

# Platform-specific compiler and linker options
ifeq ($(PLATFORM), windows)
    # Windows-specific settings
    CFLAGS = -O3 -s -mtune=generic -flto -fno-stack-protector \
             -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti -fno-exceptions \
             -funroll-loops -falign-functions -ffast-math
    PLATFORM_OPTS = -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm \
                    -static -static-libgcc -static-libstdc++ -lole32 -ldsound 
    OUT_FILE = $(WIN_OUT)
    RM = del /f /q
else ifeq ($(PLATFORM), macos)
    # macOS-specific settings
    CFLAGS = -O3 -march=native -mtune=native -flto -fno-stack-protector \
             -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti -fno-exceptions \
             -funroll-loops -falign-functions -ffast-math -mmacosx-version-min=15 \
             -Wl,-sectcreate,__RESTRICT,__restrict,/dev/null \
             -Wl,-dead_strip
    PLATFORM_OPTS = -Llib -framework CoreVideo -framework IOKit -framework Cocoa \
                    -framework GLUT -framework OpenGL -framework OpenAL \
                    -framework AudioToolbox lib/libraylib.a 
    OUT_FILE = $(MACOS_OUT)
    RM = rm -f
endif

# Default target
all: build

# Build rule based on platform
build:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OUT_FILE) $(PLATFORM_OPTS) $(STD) $(CFLAGS)

# Clean up generated binaries
clean:
	$(RM) bin/Bitwars.exe bin/Bitwars
