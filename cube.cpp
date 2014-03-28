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
bool running = true;

Matrix transformation = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
Matrix projection = {{100,0,0,0}, {0, -100, 0, 0}, {0,0,1,0}, {300,200,0,1}};

int main(int argc, char** argv) {
    init(argc, argv);

    buttons.push_back(Button("Quit!", [&](){
        running = false;
    }));
    buttons.push_back(Button("Zoom +", zoomplus));
    buttons.push_back(Button("Zoom -", zoomminus));
    buttons.push_back(Button("X +", get_translate_lambda(0.1f,0,0)));
    buttons.push_back(Button("Y +", get_translate_lambda(0,0.1f,0)));
    buttons.push_back(Button("Z +", get_translate_lambda(0,0,0.1f)));
    buttons.push_back(Button("X -", get_translate_lambda(-0.1f,0,0)));
    buttons.push_back(Button("Y -", get_translate_lambda(0,-0.1f,0)));
    buttons.push_back(Button("Z -", get_translate_lambda(0,0,-0.1f)));


    while (running) {
        handle_events();

        clear();
        draw_buttons();
        filename.draw(screen);
        for (auto vertex : vertices) draw_vertex(vertex);
        for (auto face : faces) draw_face(face);

        SDL_Flip(screen);
    }

    deinit();
    return 0;
}
