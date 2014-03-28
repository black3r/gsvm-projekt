#ifndef INPUTBOX_H_
#define INPUTBOX_H_

#include <string>
#include <SDL/SDL.h>
using namespace std;

class InputBox {
public:
    string text;
    SDL_Surface *rendered = NULL;
    bool selected = false;

    InputBox();
    InputBox(string text);
    void setText(string text);
    string getText();
    void draw(SDL_Surface* screen);
    void handle_keypress(SDL_keysym keysym);
    void handle_mouse(int x, int y);
    void select();
    void deselect();
    void renderText();
    void toggleSelect();
};

#endif
