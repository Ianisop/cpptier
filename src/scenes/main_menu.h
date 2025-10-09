#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "scene.h"
#include "scene_manager.h"
namespace ctier
{
    class MainScene : public Scene
    {
      public:
        MainScene(SceneManager* manager) : manager_(manager) {}

        void on_enter(ftxui::ScreenInteractive* screen) override
        {
            _screen = screen;
        }
        ftxui::Element Render() override
        {
            using namespace ftxui;
            return vbox({
                       text("=== Welcome ==="),
                       text("asdasd"),
                       text("asdasd"),
                   }) |
                   center;
        }

        void OnEvent(const ftxui::Event& e) override
        {
            if (e == ftxui::Event::Character('g'))
            {
                manager_->switch_to("game",_screen);
            }
            else if (e == ftxui::Event::Character('q'))
            {
                exit(0);
            }
        }

      private:
        SceneManager* manager_;
        ftxui::ScreenInteractive* _screen;
    };
}  // namespace ctier
