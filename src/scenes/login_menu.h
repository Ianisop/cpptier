#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

#include "database/database.h"
#include "scene.h"
#include "scene_manager.h"

namespace ctier
{

    class LoginScene : public Scene
    {
      public:
        explicit LoginScene(SceneManager* manager) :
            _manager(manager),
            _username_input(ftxui::Input(&_username, "Username")),
            _password_input(ftxui::Input(&_password, "Password")),
            _container(ftxui::Container::Vertical({
                _username_input,
                _password_input,
            }))
        {
            // You can add buttons or other components later here.
        }
        void           on_enter(ftxui::ScreenInteractive* screen) override { _screen = screen; }
        ftxui::Element Render() override
        {
            using namespace ftxui;
            return vbox({
                       text("c-tier Login") | bold | center | bgcolor(_current_color),
                       separator(),
                       _username_input->Render() | bgcolor(_current_color),
                       _password_input->Render() | bgcolor(_current_color),
                       separator(),
                   }) |
                   border | center;
        }

        void OnEvent(const ftxui::Event& e) override
        {
            if (e == ftxui::Event::Return)
            {
                if (_username.empty() || _password.empty())
                {
                    _current_color = ftxui::Color::Red;
                    _screen->Post(ftxui::Event::Custom);
                }
                else
                {
                    if(!try_login()) return;
                    _current_color = ftxui::Color::DarkGreen;
                    _screen->Post(ftxui::Event::Custom);

                    // Use async to delay without blocking
                    std::thread(
                        [this]()
                        {
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            _screen->Post(ftxui::Event::Custom);  // Ensure we're on main thread
                            _manager->switch_to("main", _screen);
                        })
                        .detach();
                }
                return;
            }
            if (e == ftxui::Event::Special("\x1B"))
            {
                _screen->Exit();
            }

            _container->OnEvent(e);
        }

        bool try_login()
        {
            UserDatabase user_db = UserDatabase("");
            if (user_db.validate_user(_username, _password))
            {
                return true;
            }
            else
            {
                user_db.create_user(_username, _password);
                return true;
            }
            return false;
        }

      private:
        ftxui::ScreenInteractive* _screen;  // TODO: Handle the potential dangling pointer
        SceneManager*             _manager;
        std::string               _username;
        std::string               _password;

        ftxui::Color _current_color = ftxui::Color::Black;

        ftxui::Component _username_input;
        ftxui::Component _password_input;
        ftxui::Component _container;
    };

}  // namespace ctier
