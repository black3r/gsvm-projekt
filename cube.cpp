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

TTF_Font *font = NULL;
SDL_Color textColor = {0,0,0};
InputBox filename = InputBox("hello");
string openfilename = "";
vector<Button> buttons;

vector<vector<float>> vertices;
vector<vector<int>> faces;

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Surface* screen = NULL;
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

    while (running) {
        // INPUT
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    if ( x > 610 && x < 790 ) {
                        if ( y <= 350 ) {
                            int s = y / 50;
                            if ( y > s*50 + 5 && y < s*50 + 45) {
                                if (buttons.size() > s) {
                                    buttons[s].callback();
                                } else {
                                    filename.deselect();
                                }
                            } else {
                                filename.deselect();
                            }
                        } else if (y > (400 - 30) && y < (400 - 5)) {
                            filename.toggleSelect();
                        } else {
                            filename.deselect();
                        }
                    } else {
                        filename.deselect();
                    }
                }
            }
            if (event.type == SDL_KEYDOWN) {
                filename.handle_keypress(event.key.keysym);
            }
        }
        // PROCESS

        // OUTPUT
        // CLEAR TO WHITE
        SDL_FillRect(screen, NULL, BACKGROUND);
        // BUTTONS
        for (int i = 0; i < buttons.size(); i++) {
            rectangleColor(screen, 610, i*50 + 5, 790, i*50 + 45, FOREGROUND);
            SDL_Rect offset;
            offset.x = 620;
            offset.y = i*50 + 17;
            SDL_BlitSurface(buttons[i].rendered, NULL, screen, &offset);
        }

        // INPUTBOX
        filename.draw(screen);

        // VERTICES
        for (auto vertex : vertices) {
            int pos_x = vertex[0] * 100 + 300;
            int pos_y = vertex[1] * 100 + 200;
            pixelColor(screen, pos_x, 400 - pos_y, FOREGROUND);
        }

        // FACES
        for (auto face : faces) {
            for (int i = 0; i < face.size(); i++) {
                int start_x = vertices[face[i]-1][0] * 100 + 300;
                int start_y = vertices[face[i]-1][1] * 100 + 200;
                int end_x = vertices[face[(i+1)%face.size()] - 1][0] * 100 + 300;
                int end_y = vertices[face[(i+1)%face.size()] - 1][1] * 100 + 200;
                lineColor(screen, start_x, 400 - start_y, end_x, 400 - end_y, FOREGROUND);
            }
        }

        SDL_Flip(screen);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
