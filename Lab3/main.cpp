
#include "MenuSystem.h"
#include <iostream>
#include <memory>

using namespace std;

int main(int argc, char* argv[]) {
    
    unique_ptr<MenuSystem> menuSystem(new MenuSystem());
    
    if (!menuSystem->initialize()) {
        cerr << "Failed to initialize menu system!" << std::endl;
        return 1;
    }
    
    menuSystem->loadResources("menu_resources.txt");
    menuSystem->run();
    
    return 0;
}