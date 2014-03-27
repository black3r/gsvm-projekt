#include "button.h"
#include <SDL/SDL_ttf.h>
extern TTF_Font* font;
extern SDL_Color textColor;

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
