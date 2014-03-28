#ifndef BUTTON_H_
#define BUTTON_H_

#include <string>
#include <SDL/SDL.h>
#include <functional>
using namespace std;

class Button {
public:
    function<void()> callback;
    string text;
    SDL_Surface *rendered = NULL;

    Button(string text, function<void()> callback);
    void setText(string text);

};

void handle_button_input(int x, int y);

#endif
