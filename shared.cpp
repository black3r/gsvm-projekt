#include "shared.h"
#include "inputbox.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <SDL/SDL.h>

using namespace std;

extern InputBox filename;
extern string openfilename;
extern vector<vector<float>> vertices;
extern vector<vector<int>> faces;

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
