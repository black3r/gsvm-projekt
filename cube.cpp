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
#include "shared.h"
#include "matrix.h"

TTF_Font *font = NULL;
SDL_Color textColor = {0,0,0};
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
vector<float> light = {0,0,0};
uint8_t fr,fg,fb;

int main(int argc, char** argv) {
    fr = 255;
    fg = 255;
    fb = 255;
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
    buttons.push_back(Button("Light X +", [](){light[0]++;}));
    buttons.push_back(Button("Light Y +", [](){light[1]++;}));
    buttons.push_back(Button("Light Z +", [](){light[2]++;}));
    buttons.push_back(Button("Light X -", [](){light[0]--;}));
    buttons.push_back(Button("Light Y -", [](){light[1]--;}));
    buttons.push_back(Button("Light Z -", [](){light[2]--;}));
    buttons.push_back(Button("Color pick", []() {
        FILE* pipe = popen("zenity --color-selection", "r");
        char buffer[16];
        fgets(buffer, sizeof(buffer), pipe);
        pclose(pipe);
        unsigned int red, green, blue;
        char rc[3], rg[3], rb[3];
        rc[0] = buffer[1];
        rc[1] = buffer[2];
        rc[2] = ' ';
        rg[0] = buffer[5];
        rg[1] = buffer[6];
        rg[2] = ' ';
        rb[0] = buffer[9];
        rb[1] = buffer[10];
        rb[2] = ' ';
        cout << rc << endl;
        cout << rg << endl;
        cout << rb << endl;
        sscanf(rc, "%x", &red);
        sscanf(rg, "%x", &green);
        sscanf(rb, "%x", &blue);
        cout << "Red: " << red << endl;
        cout << "Green: " << green << endl;
        cout << "Blue: " << blue << endl;
        fr = red;
        fg = green;
        fb = blue;
    }));
    buttons.push_back(Button("Reset", []() {
        transformation = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
        light = {0,0,0};
        fr = 255;
        fg = 255;
        fb = 255;
    }));
    buttons.push_back(Button("Open file", []() {
        FILE* pipe = popen("zenity --file-selection", "r");
        char buffer[16000];
        fgets(buffer, sizeof(buffer), pipe);
        for (int i = 0; i < sizeof(buffer); i++) {
            if (buffer[i] == ' ' || buffer[i] == '\n') buffer[i] = 0;
        }
        pclose(pipe);
        openfile(buffer);
        cout << buffer << endl;
    }));


    while (running) {
        handle_events();

        clear();
        for (auto vertex : vertices) draw_vertex(vertex);
        for (auto face : faces) draw_face(face);
        draw_buttons();

        SDL_Flip(screen);
    }

    deinit();
    return 0;
}
