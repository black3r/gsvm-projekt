#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <functional>
#include <SDL/SDL_ttf.h>

#define FOREGROUND 0x000000FF
#define BACKGROUND 0xFFFFFFFF

using namespace std;
TTF_Font *font = NULL;
SDL_Color textColor = {0,0,0};

class Button {
public:
    function<void()> callback;
    string text;
    SDL_Surface *rendered = NULL;

    Button(string text, function<void()> callback);
    void setText(string text);

};

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

vector<Button> buttons;

void openfile(string);

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
    void select();
    void deselect();
    void renderText();
    void toggleSelect();
};

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

InputBox filename = InputBox("hello");
string openfilename = "";

vector<vector<float>> vertices;
vector<vector<int>> faces;

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

int main(int argc, char** argv) {    
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Surface* screen = NULL;
    screen = SDL_SetVideoMode(800, 400, 32, SDL_SWSURFACE);
    #ifdef _WIN32
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 14);
    #else
    font = TTF_OpenFont("arial.ttf", 14);
    #endif

    bool running = true;
    
    if (argc == 1) openfile("cube.obj");
    else openfile(argv[1]);

    buttons.push_back(Button("Quit!", [&](){
        running = false;
    }));

    while (running) {
        // INPUT
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;                    
                    if ( x > 610 && x < 790 ) {
                        if ( y <= 350 ) {
                            int s = y / 50;                        
                            if ( y > s*50 + 5 && y < s*50 + 45) {
                                if (buttons.size() > s) {
                                    buttons[s].callback();
                                } else {
                                    filename.deselect();
                                }
                            } else {
                                filename.deselect();
                            }
                        } else if (y > (400 - 30) && y < (400 - 5)) {
                            filename.toggleSelect();
                        } else {
                            filename.deselect();
                        }
                    } else {
                        filename.deselect();
                    }
                } 
            }
            if (event.type == SDL_KEYDOWN) {
                filename.handle_keypress(event.key.keysym);
            }
        }
        // PROCESS

        // OUTPUT
        // CLEAR TO WHITE
        SDL_FillRect(screen, NULL, BACKGROUND);
        // BUTTONS        
        for (int i = 0; i < buttons.size(); i++) {
            rectangleColor(screen, 610, i*50 + 5, 790, i*50 + 45, FOREGROUND);            
            SDL_Rect offset;
            offset.x = 620;
            offset.y = i*50 + 17;
            SDL_BlitSurface(buttons[i].rendered, NULL, screen, &offset);
        }

        // INPUTBOX
        filename.draw(screen);

        // VERTICES
        for (auto vertex : vertices) {
            int pos_x = vertex[0] * 100 + 300;
            int pos_y = vertex[1] * 100 + 200;
            pixelColor(screen, pos_x, 400 - pos_y, FOREGROUND);
        }

        // FACES
        for (auto face : faces) {
            for (int i = 0; i < face.size(); i++) {
                int start_x = vertices[face[i]-1][0] * 100 + 300;
                int start_y = vertices[face[i]-1][1] * 100 + 200;
                int end_x = vertices[face[(i+1)%face.size()] - 1][0] * 100 + 300;
                int end_y = vertices[face[(i+1)%face.size()] - 1][1] * 100 + 200;
                lineColor(screen, start_x, 400 - start_y, end_x, 400 - end_y, FOREGROUND);
            }        
        }                

        SDL_Flip(screen);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

