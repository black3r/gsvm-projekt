all: cube cube-static.exe

run: cube
	./cube

cube: cube.cpp
	g++ -Wall -Wextra -O2 -std=gnu++11 -o cube cube.cpp -lSDL -lSDL_gfx -lSDL_ttf -g

cube-static.exe: cube.cpp
	x86_64-w64-mingw32-g++ -static -Wall -Wextra -O2 -std=gnu++11 -o cube-static.exe cube.cpp -lmingw32 -luser32 -lgdi32  -lSDLmain -lSDL -lSDL_gfx -lwinmm -ldxguid -lSDL_ttf -lfreetype -lz -lbz2 -mwindows

.PHONY: run
