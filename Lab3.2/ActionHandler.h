#pragma once

#include <SDL.h>
#include <string>
#include <map>
#include <memory>
#include <algorithm>

struct IAction {
    virtual ~IAction() = default;
    virtual void execute() = 0;
};

class ActionHandler {
public:
    void registerAction(const std::string& id, std::unique_ptr<IAction> action);
    void execute(const std::string& id);
private:
    std::map<std::string, std::unique_ptr<IAction>> actions_;
};

// ExitAppAction
class ExitAppAction : public IAction {
public:
    explicit ExitAppAction(bool& running) : running_(running) {}
    void execute() override { running_ = false; }
private:
    bool& running_;
};

// StartGameAction (исправлен, теперь выводит простое диалоговое окно SDL)
class StartGameAction : public IAction {
public:
    void execute() override {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Игра",
                                 "Игра успешно запущена!",
                                 nullptr);
    }
};

// IncreaseVolumeAction
class IncreaseVolumeAction : public IAction {
public:
    explicit IncreaseVolumeAction(int& volume) : volume_(volume) {}
    void execute() override {
        volume_ = std::min(volume_ + 10, 100);
    }
private:
    int& volume_;
};

// DecreaseVolumeAction
class DecreaseVolumeAction : public IAction {
public:
    explicit DecreaseVolumeAction(int& volume) : volume_(volume) {}
    void execute() override {
        volume_ = std::max(volume_ - 10, 0);
    }
private:
    int& volume_;
};

// ConfigureKeyboardAction
class ConfigureKeyboardAction : public IAction {
public:
    void execute() override {
    }
};

// ConfigureMouseAction
class ConfigureMouseAction : public IAction {
public:
    void execute() override {
    }
};


