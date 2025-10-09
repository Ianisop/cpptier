#pragma once
#include "scene.h"
#include <memory>
#include <unordered_map>
#include <string>

using namespace ctier;

class SceneManager {
public:
    void register_scene(const std::string& name, std::unique_ptr<Scene> scene) {
        scenes_[name] = std::move(scene);
    }

    void switch_to(const std::string& name,ftxui::ScreenInteractive* screen) {
        if (current_) current_->on_exit();
        current_ = scenes_[name].get();
        if (current_) current_->on_enter(screen);
    }

    Scene* current() { return current_; }

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
    Scene* current_ = nullptr;
};
