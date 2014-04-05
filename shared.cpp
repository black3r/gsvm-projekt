#include "shared.h"
#include "inputbox.h"
#include "matrix.h"
#include "button.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <cmath>

using namespace std;
#define PI 3.141592653

extern InputBox filename;
extern string openfilename;
extern vector<vector<float>> vertices;
extern vector<vector<int>> faces;
extern SDL_Surface* screen;
extern Matrix transformation;
extern Matrix scaling;
extern Matrix translation;
extern Matrix rotation;
extern Matrix projection;
extern TTF_Font* font;
extern bool running;

void openfile(string fname) {
    vertices.clear();
    faces.clear();
    ifstream f(fname);
    if (f.good()) {
        openfilename = fname;
    } else {
        openfilename = "** ERROR **";
    }
    SDL_WM_SetCaption(openfilename.c_str(), NULL);
    filename.setText(fname);

    string s;
    while (getline(f, s)) {
        stringstream in(s);
        char t; in >> t;
        if (t == 'v') {
            float a,b,c;
            in >> a >> b >> c;
            vertices.push_back({a,b,c});
        } else if (t == 'f') {
            vector<int> face;
            int a;
            while (in >> a) face.push_back(a);
            faces.push_back(face);
        }
    }
    f.close();
}

vector<float> get_draw_coords(vector<float> vertex) {
    vertex.push_back(1);
    Matrix t = vertex * transformation * projection;
    return {t[0][0], t[0][1]};
}

void draw_point(SDL_Surface* screen, vector<float> draw_coords) {
    pixelColor(screen, draw_coords[0], draw_coords[1], FOREGROUND);
}

void draw_line(SDL_Surface* screen, vector<float> draw_coords_from, vector<float> draw_coords_to) {
    lineColor(screen, draw_coords_from[0], draw_coords_from[1], draw_coords_to[0], draw_coords_to[1], FOREGROUND);
}

void draw_vertex(vector<float> vertex) {
    draw_point(screen, get_draw_coords(vertex));
}

void draw_face(vector<int> face) {
    for (int i = 0; i < face.size(); i++) {
        draw_line(screen, get_draw_coords(vertices[face[i]-1]), get_draw_coords(vertices[face[(i+1)%face.size()] - 1]));
    }
}

void clear() {
    SDL_FillRect(screen, NULL, BACKGROUND);
}

void init(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    screen = SDL_SetVideoMode(800, 400, 32, SDL_SWSURFACE);
    #ifdef _WIN32
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 12);
    #else
    font = TTF_OpenFont("arial.ttf", 12);
    #endif

    if (argc == 1) openfile("cube.obj");
    else openfile(argv[1]);
}

void deinit() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void handle_events() {
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
}

void zoom(float ratio) {
    transformation *= {{ratio,0,0,0},{0,ratio,0,0},{0,0,ratio,0},{0,0,0,1}};
}

void translate(float x, float y, float z) {
    transformation *= {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {x, y, z, 1}};
}

void rotate_x(float d) {
    float x = (d / 180) * PI;
    transformation *= {{1,0,0,0}, {0,cos(x),-sin(x),0}, {0, sin(x), cos(x), 0}, {0,0,0,1}};
}

void rotate_y(float d) {
    float x = (d / 180) * PI;
    transformation *= {{cos(x),0,sin(x),0}, {0,1,0,0}, {-sin(x),0,cos(x),0}, {0,0,0,1}};
}

void rotate_z(float d) {
    float x = (d / 180) * PI;
    transformation *= {{cos(x), sin(x), 0,0}, {-sin(x), cos(x), 0, 0}, {0,0,1,0}, {0,0,0,1}};
}
