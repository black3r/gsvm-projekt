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
Matrix translation = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
Matrix rotation = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
Matrix scaling = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

int main(int argc, char** argv) {
    init(argc, argv);

    buttons.push_back(Button("Quit!", [&](){
        running = false;
    }));
    buttons.push_back(Button("Zoom +", bind(zoom, 1.1)));
    buttons.push_back(Button("Zoom -", bind(zoom, 0.9)));
    buttons.push_back(Button("X +", bind(translate,0.1f,0,0)));
    buttons.push_back(Button("Y +", bind(translate,0,0.1f,0)));
    buttons.push_back(Button("Z +", bind(translate,0,0,0.1f)));
    buttons.push_back(Button("X -", bind(translate,-0.1f,0,0)));
    buttons.push_back(Button("Y -", bind(translate,0,-0.1f,0)));
    buttons.push_back(Button("Z -", bind(translate,0,0,-0.1f)));
    buttons.push_back(Button("Rotate X +", bind(rotate_x,5)));
    buttons.push_back(Button("Rotate Y +", bind(rotate_y,5)));
    buttons.push_back(Button("Rotate Z +", bind(rotate_z,5)));
    buttons.push_back(Button("Rotate X -", bind(rotate_x,-5)));
    buttons.push_back(Button("Rotate Y -", bind(rotate_y,-5)));
    buttons.push_back(Button("Rotate Z -", bind(rotate_z,-5)));


    while (running) {
        handle_events();

        clear();
        for (auto vertex : vertices) draw_vertex(vertex);
        for (auto face : faces) draw_face(face);
        draw_buttons();
        filename.draw(screen);

        SDL_Flip(screen);
    }

    deinit();
    return 0;
}
