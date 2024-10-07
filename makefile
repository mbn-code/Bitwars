# Detect the platform from a passed variable or use default (macOS)
PLATFORM ?= macos

# Compiler definitions
COMPILER = g++
STD = -std=c++11

# Include paths for headers and Raylib
SOURCE_LIBS = -Ilib/ -Iincludes

# Output binaries
WIN_OUT = -o "bin/Bitwars.exe"
MACOS_OUT = -o "bin/Bitwars"

# Source files
CFILES = src/*.cpp includes/HandleCombat.cpp includes/PointMultiplier.cpp includes/HandleHighScore.cpp includes/CheatChecker.cpp

# Platform-specific compiler and linker options
ifeq ($(PLATFORM), windows)
    # Windows-specific settings
    COMPILER = g++
    # Anti-debugging and security flags for Windows
    CFLAGS = -O3 -s -fno-stack-protector -fvisibility=hidden -fvisibility-inlines-hidden \
             -fno-rtti -fno-exceptions -mwindows -Wl,--no-insert-timestamp
    PLATFORM_OPTS = -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++ -lwinmm -lole32 -ldsound
    OUT_FILE = $(WIN_OUT)
    RM = del /f /q
else ifeq ($(PLATFORM), macos)
    # macOS-specific settings
    COMPILER = clang++
    # Anti-debugging and security flags for macOS
    CFLAGS = -O3 -s -fno-stack-protector -fvisibility=hidden -fvisibility-inlines-hidden \
             -fno-rtti -fno-exceptions -mmacosx-version-min=15 \
             -Wl,-sectcreate,__RESTRICT,__restrict,/dev/null \
             -Wl,-dead_strip -Wl,-no_pie
    PLATFORM_OPTS = -Llib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -framework OpenAL -framework AudioToolbox lib/libraylib.a 
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

