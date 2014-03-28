#include "shared.h"
#include "inputbox.h"
#include "matrix.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

using namespace std;

extern InputBox filename;
extern string openfilename;
extern vector<vector<float>> vertices;
extern vector<vector<int>> faces;
extern SDL_Surface* screen;
extern Matrix transformation;
extern Matrix projection;

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
    // first apply transformation matrix, then return first two coordinates.
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

void zoomplus() {
    transformation *= {{1.1,0,0,0},{0,1.1,0,0},{0,0,1.1,0},{0,0,0,1}};
}

void zoomminus() {
    transformation *= {{0.9,0,0,0},{0,0.9,0,0},{0,0,0.9,0},{0,0,0,1}};
}

