#include "ResourceLoader.h"
#include <iostream>
#include <functional>
#include <ios>

using namespace std;

ResourceLoader::ResourceLoader(const string& filename, 
                               map<string, function<void()>>& actions)
    : filename(filename), actions(actions) {}

map<string, Menu> ResourceLoader::loadMenus(SDL_Renderer* renderer, TTF_Font* font) {
    map<string, Menu> menus;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Failed to open resource file: " << filename << endl;
        return menus;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.find("[MENU:") == 0) {
            string sectionName;
            auto section = parseSection(file, line, sectionName);
            
            if (section.find("title") != section.end() && 
                section.find("x") != section.end() &&
                section.find("y") != section.end() &&
                section.find("width") != section.end() &&
                section.find("height") != section.end()) {
                
                string title = section["title"];
                int x = stoi(section["x"]);
                int y = stoi(section["y"]);
                int width = stoi(section["width"]);
                int height = stoi(section["height"]);
                
                Menu menu(title, Point(x, y), Size(width, height), renderer, font);
                
                for (const auto& pair : section) {
                    if (pair.first.find("item") == 0) {
                        size_t colonPos = pair.second.find(':');
                        if (colonPos != string::npos) {
                            string label = pair.second.substr(0, colonPos);
                            string actionName = pair.second.substr(colonPos + 1);
                            
                            if (actions.find(actionName) != actions.end()) {
                                menu.addItem(MenuItem(label, actions[actionName], renderer, font));
                            } else {
                                menu.addItem(MenuItem(label, [](){}, renderer, font));
                            }
                        }
                    }
                }
                
                menus.emplace(sectionName, move(menu));
            }
        }
    }
    
    file.close();
    return menus;
}

map<string, Dialog> ResourceLoader::loadDialogs(SDL_Renderer* renderer, TTF_Font* font) {
    map<string, Dialog> dialogs;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Failed to open resource file: " << filename << endl;
        return dialogs;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.find("[DIALOG:") == 0) {
            string sectionName;
            auto section = parseSection(file, line, sectionName);
            
            if (section.find("title") != section.end() && 
                section.find("message") != section.end() && 
                section.find("x") != section.end() &&
                section.find("y") != section.end() &&
                section.find("width") != section.end() &&
                section.find("height") != section.end()) {
                
                string title = section["title"];
                string message = section["message"];
                int x = stoi(section["x"]);
                int y = stoi(section["y"]);
                int width = stoi(section["width"]);
                int height = stoi(section["height"]);
                
                Dialog dialog(title, message, Point(x, y), Size(width, height), renderer, font);
                
                for (const auto& pair : section) {
                    if (pair.first.find("button") == 0) {
                        size_t colonPos = pair.second.find(':');
                        if (colonPos != string::npos) {
                            string label = pair.second.substr(0, colonPos);
                            string actionName = pair.second.substr(colonPos + 1);
                            
                            if (actions.find(actionName) != actions.end()) {
                                dialog.addButton(label, actions[actionName]);
                            } else {
                                dialog.addButton(label, [](){});
                            }
                        }
                    }
                }
                
                dialogs.emplace(sectionName, move(dialog));
            }
        }
    }
    
    file.close();
    return dialogs;
}

vector<string> ResourceLoader::splitString(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

map<string, string> ResourceLoader::parseSection(ifstream& file, const string& headerLine, string& sectionName) {
    map<string, string> section;
    
    size_t nameStart = headerLine.find(':') + 1;
    size_t nameEnd = headerLine.find(']');
    sectionName = headerLine.substr(nameStart, nameEnd - nameStart);
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '[') {
            file.seekg(-static_cast<int>(line.length()) - 1, ios_base::cur);
            break;
        }
        
        size_t equalsPos = line.find('=');
        if (equalsPos != string::npos) {
            string key = line.substr(0, equalsPos);
            string value = line.substr(equalsPos + 1);
            section[key] = value;
        }
    }
    
    return section;
}