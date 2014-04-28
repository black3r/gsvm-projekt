#include "shared.h"
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


extern string openfilename;
extern vector<vector<float>> vertices;
extern vector<vector<int>> faces;
extern SDL_Surface* screen;
extern Matrix transformation;
extern Matrix scaling;
extern Matrix translation;
extern Matrix rotation;
extern Matrix projection;
extern vector<float> light;
extern TTF_Font* font;
extern bool running;
extern uint8_t fr,fg,fb;

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
    //filename.setText(fname);

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

vector<float> get_transformed_coords(vector<float> vertex) {
    vertex.push_back(1);
    Matrix t = vertex * transformation;
    return {t[0][0], t[0][1], t[0][2]};
}

void draw_point(SDL_Surface* screen, vector<float> draw_coords) {
    pixelColor(screen, draw_coords[0], draw_coords[1], FOREGROUND);
}

void draw_line(SDL_Surface* screen, vector<float> draw_coords_from, vector<float> draw_coords_to) {
    lineColor(screen, draw_coords_from[0], draw_coords_from[1], draw_coords_to[0], draw_coords_to[1], FOREGROUND);
}

void draw_polygon(SDL_Surface* screen, vector<vector<float>> points, float intensity) {
    short x[points.size()];
    short y[points.size()];
    for (int i = 0; i < points.size(); i++) {
        x[i] = points[i][0];
        y[i] = points[i][1];
    }
    //polygonColor(screen,x,y,points.size(), FOREGROUND);
    filledPolygonRGBA(screen, x, y, points.size(), intensity * fr, intensity * fg, intensity * fb, 255);
}

void draw_vertex(vector<float> vertex) {
    draw_point(screen, get_draw_coords(vertex));
}

void draw_face(vector<int> face) {
    vector<vector<float>> points;
    vector<vector<float>> draw_points;
    for (auto x : face) {
        points.push_back(get_transformed_coords(vertices[x-1]));
        draw_points.push_back(get_draw_coords(vertices[x-1]));
    }
    if (should_draw(points)) {
        draw_polygon(screen, draw_points, get_intensity(points));
    }
}

bool should_draw(vector<vector<float>> points) {
    vector<float> normal = get_normal(points);
    return (normal[2] <= 0);
}

float get_intensity(vector<vector<float>> points) {
    float x, y, z;
    for (auto t : points) {
        x += t[0];
        y += t[1];
        z += t[2];
    }
    x /= points.size();
    y /= points.size();
    z /= points.size();

    vector<float> vertex = {x-light[0], y-light[1], z-light[2]};
    vector<float> normal = get_normal(points);
    float dot_product = vertex[0]*normal[0] + vertex[1]*normal[1] + vertex[2]*normal[2];
    float cosine = dot_product / (get_norm(vertex) * get_norm(normal));
    if (cosine < 0) cosine = 0;
    if (cosine > 1) cerr << "Si chudak!" << endl;
    return cosine;
}

vector<float> get_normal(vector<vector<float>> points) {
    if (points.size() < 3) return {};
    else {
        vector<float> p1 = points[0];
        vector<float> p2 = points[1];
        vector<float> p3 = points[2];
        vector<float> v = {p3[0]-p2[0], p3[1]-p2[1], p3[2]-p2[2]};
        vector<float> u = {p1[0]-p2[0], p1[1]-p2[1], p1[2]-p2[2]};
        vector<float> result = {u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]};
        return result;
    }
}

float get_norm(vector<float> points) {
    return sqrt(points[0]*points[0] + points[1]*points[1] + points[2]*points[2]);
}

void clear() {
    SDL_FillRect(screen, NULL, BACKGROUND);
}

void init(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
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
            }
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
