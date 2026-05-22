#include "ResourceLoader.h"
#include <fstream>
#include <sstream>
#include <SDL.h>

bool ResourceLoader::loadFromFile(const std::string& filename,
                                  std::map<std::string, MenuDefinition>& outMenus) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        SDL_Log("Failed to open resource file: %s", filename.c_str());
        return false;
    }

    outMenus.clear();
    MenuDefinition current;
    bool inSection = false;
    std::string line;
    int lineNumber = 0;

    while (std::getline(in, line)) {
        lineNumber++;
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.empty() || line[0]=='#' || line[0]==';') continue;

        if (line.front()=='[' && line.back()==']') {
            if (inSection && !current.id.empty()) {
                outMenus.emplace(current.id, current);
                current = MenuDefinition{};
            }
            inSection = (line=="[Menu]" || line=="[Dialog]");
            continue;
        }
        if (!inSection) continue;

        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0,pos);
        std::string val = line.substr(pos+1);

        if (key=="id") {
            current.id = val;
        }
        else if (key=="title") {
            current.title = val;
        }
        else if (key=="item") {
            MenuItem it;
            if (val.substr(0, 3) == "id=") {
                val = val.substr(3);
            }
            auto attrs = parseAttributes(val);
            it.id = attrs["id"];
            it.text = attrs["text"];
            it.next = attrs.count("next") ? attrs["next"] : "";
            it.action = attrs.count("action") ? attrs["action"] : "";

            if (it.id.empty()) {
                SDL_Log("Warning: missing item id at line %d", lineNumber);
            }
            current.items.push_back(std::move(it));
        }
    }
    if (inSection && !current.id.empty()) {
        outMenus.emplace(current.id, current);
    }
    return true;
}

std::map<std::string,std::string> ResourceLoader::parseAttributes(const std::string& s) {
    std::map<std::string,std::string> res;
    std::istringstream ss(s);
    std::string part;
    while (std::getline(ss, part, ';')) {
        auto p = part.find('=');
        if (p==std::string::npos) continue;
        std::string key = part.substr(0,p);
        std::string value = part.substr(p+1);
        res[key] = value;
    }
    return res;
}

