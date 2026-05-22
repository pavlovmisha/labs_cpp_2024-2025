#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Menu.h"
#include "Dialog.h"
#include "Point.h"
#include "Size.h"

using namespace std;

class ResourceLoader {
private:
    string filename;
    map<string, function<void()>>& actions;

public:
    ResourceLoader(const string& filename, map<string, function<void()>>& actions);
    
    map<string, Menu> loadMenus(SDL_Renderer* renderer, TTF_Font* font);
    map<string, Dialog> loadDialogs(SDL_Renderer* renderer, TTF_Font* font);
    
private:
    vector<string> splitString(const string& s, char delimiter);
    map<string, string> parseSection(ifstream& file, const string& headerLine, string& sectionName);
};

#endif