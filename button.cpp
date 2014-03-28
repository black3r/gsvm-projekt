#include "shared.h"
#include "button.h"
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>

#define BUTTON_HEIGHT 23

using namespace std;
extern TTF_Font* font;
extern SDL_Color textColor;
extern vector<Button> buttons;
extern SDL_Surface* screen;

Button::Button(string text, function<void()> callback) {
    this->callback = callback;
    this->setText(text);
}

void Button::setText(string text) {
    this->text = text;
    if (this->rendered != NULL) {
        SDL_FreeSurface(this->rendered);
    }
    this->rendered = TTF_RenderText_Solid(font, text.c_str(), textColor);
}

void handle_button_input(int x, int y) {
    if ( x > 610 && x < 790 ) {
        if ( y <= 350 ) {
            int s = y / BUTTON_HEIGHT;
            if ( y > s*BUTTON_HEIGHT + 5 && y < (s+1)*BUTTON_HEIGHT - 5) {
                if (buttons.size() > s) {
                    buttons[s].callback();
                }
            }
        }
    }
}

void draw_buttons() {
    for (int i = 0; i < buttons.size(); i++) {
        rectangleColor(screen, 610, i*BUTTON_HEIGHT + 5, 790, (i+1)*BUTTON_HEIGHT - 5, FOREGROUND);
        SDL_Rect offset;
        offset.x = 620;
        offset.y = i*BUTTON_HEIGHT + BUTTON_HEIGHT / 2 - 6;
        SDL_BlitSurface(buttons[i].rendered, NULL, screen, &offset);
    }
}
