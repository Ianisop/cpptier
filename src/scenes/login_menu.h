#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "scene.h"
#include "scene_manager.h"
namespace ctier
{
    class LoginScene : public Scene
    {
      public:
        LoginScene(SceneManager* manager) : manager_(manager) {}

        ftxui::Element Render() override
        {
            using namespace ftxui;
            return vbox({
                       text("=== Main Menu ==="),
                       text("Press G to go to Game Scene"),
                       text("Press Q to quit"),
                   }) |
                   center;
        }

        void OnEvent(const ftxui::Event& e) override
        {
            if (e == ftxui::Event::Character('g'))
            {
                manager_->switch_to("game");
            }
            else if (e == ftxui::Event::Character('q'))
            {
                exit(0);
            }
        }

      private:
        SceneManager* manager_;
    };
}  // namespace ctier
