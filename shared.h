#ifndef SHARED_H_
#define SHARED_H_

#include <string>
#include <vector>
#include <functional>
#include <SDL/SDL.h>
#include <cstdint>
#include "matrix.h"
using namespace std;


#define FOREGROUND 0x000000FF
#define BACKGROUND 0xFFFFFFFF

void init(int argc, char** argv);
void handle_events();
void clear();
void openfile(string);
vector<float> get_draw_coords(vector<float> vertex);
void draw_point(SDL_Surface* screen, vector<float> draw_coords);
void draw_line(SDL_Surface* screen, vector<float> draw_coords_from, vector<float> draw_coords_to);
void draw_vertex(vector<float> vertex);
void draw_face(vector<int> face);
void zoom(float ratio);
function<void()> get_translate_lambda(float x, float y, float z);
void translate(float x, float y, float z);
void deinit();

void rotate_x(float d);
void rotate_y(float d);
void rotate_z(float d);

float get_intensity(vector<vector<float>> points);
vector<float> get_normal(vector<vector<float>> points);
float get_norm(vector<float> points);
bool should_draw(vector<vector<float>> points);

const Matrix camera = {{0,0,1}};

#endif
