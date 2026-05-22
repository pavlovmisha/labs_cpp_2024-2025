#include "ActionHandler.h"
#include <SDL.h>

void ActionHandler::registerAction(const std::string& id, std::unique_ptr<IAction> action) {
    actions_[id] = std::move(action);
}

void ActionHandler::execute(const std::string& id) {
    auto it = actions_.find(id);
    if (it != actions_.end() && it->second) {
        it->second->execute();
    } else {
        SDL_Log("Warning: action '%s' not found\n", id.c_str());
    }
}
