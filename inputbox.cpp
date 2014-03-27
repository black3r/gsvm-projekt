#include "shared.h"
#include "inputbox.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

extern TTF_Font *font;
extern SDL_Color textColor;

InputBox::InputBox() {
    this->setText("");
}

InputBox::InputBox(string text) {
    this->setText(text);
}

void InputBox::setText(string t) {
    this->text = t;
    this->renderText();
}

void InputBox::renderText() {
    if (this->rendered != NULL) {
        SDL_FreeSurface(this->rendered);
    }
    if (this->selected) {
        string temp = this->text + '_';
        this->rendered = TTF_RenderText_Solid(font, temp.c_str(), textColor);
    } else {
        this->rendered = TTF_RenderText_Solid(font, this->text.c_str(), textColor);
    }
}

string InputBox::getText() {
    return this->text;
}

void InputBox::draw(SDL_Surface* screen) {
    rectangleColor(screen, 610, 400 - 30, 790, 400 - 5, FOREGROUND);
    SDL_Rect offset;
    offset.x = 620;
    offset.y = 400 - 25;
    if (this->rendered != NULL) {
        SDL_BlitSurface(this->rendered, NULL, screen, &offset);
    }
}

void InputBox::handle_keypress(SDL_keysym keysym) {
    if (!this->selected) return;
    SDLKey key = keysym.sym;
    if (key == SDLK_BACKSPACE) {
        if (this->text.length() > 0) {
            this->setText(this->text.substr(0, this->text.length() - 1));
        }
    } else if (key == SDLK_ESCAPE) {
        this->deselect();
    } else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
        openfile(this->getText());
    } else {
        this->setText(this->text + (char)key);
    }
}

void InputBox::select() {
    this->selected = true;
    this->renderText();
}

void InputBox::deselect() {
    this->selected = false;
    this->renderText();
}

void InputBox::toggleSelect() {
    if (this->selected) this->deselect();
    else this->select();
}
