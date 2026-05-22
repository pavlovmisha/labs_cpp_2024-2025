// MenuController.cpp
#include "MenuController.h"
#include "Renderer.h"

MenuController::MenuController(Renderer& renderer,
                               const std::map<std::string, MenuDefinition>& menus,
                               ActionHandler& actionHandler,
                               int& volume,
                               const std::string& startMenuId)
: renderer_(renderer)
, menus_(menus)
, actionHandler_(actionHandler)
, volume_(volume)
, history_()
, currentMenuId_(startMenuId)
, selectedIndex_(0)
{}

void MenuController::handleEvent(const SDL_Event& ev, bool& running) {
    auto menuIt = menus_.find(currentMenuId_);
    if (menuIt == menus_.end()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                               "Ошибка",
                               ("Меню '" + currentMenuId_ + "' не найдено!").c_str(),
                               nullptr);
        return;
    }
    const auto& menu = menuIt->second;
    int count = static_cast<int>(menu.items.size());
    if (count == 0) return;

    if (ev.type == SDL_MOUSEBUTTONDOWN) {
        if (ev.button.button == SDL_BUTTON_LEFT) {
            int mouseX = ev.button.x;
            int mouseY = ev.button.y;
            
            int windowWidth, windowHeight;
            renderer_.getWindowSize(windowWidth, windowHeight);

            int y = 120;
            const int buttonWidth = 300;
            const int buttonHeight = 40;
            const int buttonX = (windowWidth - buttonWidth) / 2;
            
            for (int i = 0; i < count; ++i) {
                SDL_Rect itemRect{buttonX, y - 5, buttonWidth, buttonHeight};
                if (mouseX >= itemRect.x && mouseX < itemRect.x + itemRect.w &&
                    mouseY >= itemRect.y && mouseY < itemRect.y + itemRect.h) {
                    selectedIndex_ = i;
                    const auto& item = menu.items[i];
                    if (!item.next.empty()) {
                        history_.push_back(currentMenuId_);
                        currentMenuId_ = item.next;
                        selectedIndex_ = 0;
                    } else if (item.action == "back") {
                        if (!history_.empty()) {
                            currentMenuId_ = history_.back();
                            history_.pop_back();
                            selectedIndex_ = 0;
                        } else {
                            running = false;
                        }
                    } else if (!item.action.empty()) {
                        actionHandler_.execute(item.action);
                    }
                    break;
                }
                y += 40;
            }
        }
        return;
    }

    if (ev.type == SDL_KEYDOWN) {
        switch (ev.key.keysym.sym) {
            case SDLK_UP:
                selectedIndex_ = (selectedIndex_ - 1 + count) % count;
                break;
            case SDLK_DOWN:
                selectedIndex_ = (selectedIndex_ + 1) % count;
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER: {
                const auto& item = menu.items[selectedIndex_];
                if (!item.next.empty()) {
                    history_.push_back(currentMenuId_);
                    currentMenuId_ = item.next;
                    selectedIndex_ = 0;
                } else if (item.action == "back") {
                    if (!history_.empty()) {
                        currentMenuId_ = history_.back();
                        history_.pop_back();
                        selectedIndex_ = 0;
                    } else {
                        running = false;
                    }
                } else if (!item.action.empty()) {
                    actionHandler_.execute(item.action);
                }
                break;
            }
            case SDLK_ESCAPE:
                if (!history_.empty()) {
                    currentMenuId_ = history_.back();
                    history_.pop_back();
                    selectedIndex_ = 0;
                } else {
                    running = false;
                }
                break;
            default: break;
        }
    }
}

void MenuController::render() {
    const auto& menu = menus_.at(currentMenuId_);
 
    int windowWidth, windowHeight;
    renderer_.getWindowSize(windowWidth, windowHeight);

    int titleWidth, titleHeight;
    renderer_.getTextSize(menu.title, titleWidth, titleHeight);
    renderer_.drawText(menu.title, (windowWidth - titleWidth) / 2, 50);

    int y = 120;
    const int buttonWidth = 300;
    const int buttonHeight = 40;
    const int buttonX = (windowWidth - buttonWidth) / 2;
    
    for (int i = 0; i < static_cast<int>(menu.items.size()); ++i) {
        const auto& item = menu.items[i];
        if (i == selectedIndex_) {
            renderer_.fillRect(buttonX, y - 5, buttonWidth, buttonHeight);
        }
        renderer_.drawRect(buttonX, y - 5, buttonWidth, buttonHeight);
 
        int textWidth, textHeight;
        renderer_.getTextSize(item.text, textWidth, textHeight);
        int textX = buttonX + (buttonWidth - textWidth) / 2;
        renderer_.drawText(item.text, textX, y);
        
        y += 40;
    }
}



