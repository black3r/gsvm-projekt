#include "button.h"
#include <vector>
#include <SDL/SDL_ttf.h>

using namespace std;
extern TTF_Font* font;
extern SDL_Color textColor;
extern vector<Button> buttons;

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
            int s = y / 50;
            if ( y > s*50 + 5 && y < s*50 + 45) {
                if (buttons.size() > s) {
                    buttons[s].callback();
                }
            }
        }
     }
}
