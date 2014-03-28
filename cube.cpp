#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <functional>
#include <SDL/SDL_ttf.h>

using namespace std;

#include "button.h"
#include "inputbox.h"
#include "shared.h"
#include "matrix.h"

TTF_Font *font = NULL;
SDL_Color textColor = {0,0,0};
InputBox filename = InputBox("hello");
string openfilename = "";
vector<Button> buttons;
SDL_Surface* screen;

vector<vector<float>> vertices;
vector<vector<int>> faces;

Matrix transformation = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
Matrix projection = {{100,0,0,0}, {0, -100, 0, 0}, {0,0,1,0}, {300,200,0,1}};

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    screen = SDL_SetVideoMode(800, 400, 32, SDL_SWSURFACE);
    #ifdef _WIN32
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 14);
    #else
    font = TTF_OpenFont("arial.ttf", 14);
    #endif

    bool running = true;

    if (argc == 1) openfile("cube.obj");
    else openfile(argv[1]);

    buttons.push_back(Button("Quit!", [&](){
        running = false;
    }));
    buttons.push_back(Button("Zoom +", zoomplus));
    buttons.push_back(Button("Zoom -", zoomminus));

    while (running) {
        // INPUT
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    handle_button_input(x, y);
                    filename.handle_mouse(x, y);
                }
            }
            if (event.type == SDL_KEYDOWN) {
                filename.handle_keypress(event.key.keysym);
            }
        }
        // PROCESS

        clear();
        draw_buttons();
        filename.draw(screen);
        for (auto vertex : vertices) draw_vertex(vertex);
        for (auto face : faces) draw_face(face);

        SDL_Flip(screen);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
