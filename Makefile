SOURCES=cube.cpp shared.cpp inputbox.cpp button.cpp
TARGET=cube
TARGET_WIN=cube.exe
LIBS=-lSDL -lSDL_gfx -lSDL_ttf
CXXFLAGS=-Wall -Wextra -O2 -g -std=gnu++11
LIBS_WIN= -lmingw32 -luser32 -lgdi32 -lSDLmain -lSDL -lSDL_gfx -lwinmm -ldxguid -lSDL_ttf -lfreetype -lz -lbz2 -mwindows
CXX=g++
CXX_WIN=i686-w64-mingw32-g++

all: $(TARGET) $(TARGET_WIN)

run: cube
	./cube

$(TARGET): $(SOURCES)
	g++ $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

$(TARGET_WIN): $(SOURCES)
	$(CXX_WIN) -static $(CXXFLAGS) -o $(TARGET_WIN) $(SOURCES) $(LIBS_WIN)

.PHONY: run
