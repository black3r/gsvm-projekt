#ifndef SHARED_H_
#define SHARED_H_

#include <string>
#include <vector>
#include <functional>
#include <SDL/SDL.h>
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
void zoomplus();
void zoomminus();
function<void()> get_translate_lambda(float x, float y, float z);
void deinit();

function<void()> rotate_x(float d);
function<void()> rotate_y(float d);
function<void()> rotate_z(float d);

#endif
