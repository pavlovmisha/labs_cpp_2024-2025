#pragma once

#include <string>
#include <map>
#include <vector>

struct MenuItem {
    std::string id;
    std::string text;
    std::string next;    
    std::string action;  
};


struct MenuDefinition {
    std::string id;
    std::string title;
    std::vector<MenuItem> items;
};

class ResourceLoader {
    public:
        bool loadFromFile(const std::string& filename,
                          std::map<std::string, MenuDefinition>& outMenus);

private:
    std::map<std::string, MenuDefinition> menus_;

    static std::map<std::string,std::string> parseAttributes(const std::string& s);
};
