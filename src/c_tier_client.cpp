#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <iostream>

#include "client/client.h"
#include "scenes/scene.h"
#include "scenes/login_menu.h"
#include "scenes/scene_manager.h"

using namespace ftxui;

int main()
{
    auto screen = ScreenInteractive::Fullscreen();

    SceneManager manager;
    manager.register_scene("login", std::make_unique<ctier::LoginScene>(&manager));
    //manager.register_scene("main", std::make_unique<MainScene>(&manager));

    manager.switch_to("login");

    auto app = CatchEvent(
        Renderer([&] {
            if (auto scene = manager.current())
                return scene->Render();
            return text("No scene loaded");
        }),
        [&](Event e) {
            if (auto scene = manager.current())
                scene->OnEvent(e);
            return true;
        });

    screen.Loop(app);

}
/*
auto client = ctier::Client(AF_INET, SOCK_STREAM, 0, true);

std::cout << "client ready!\n";
if (client.connect("127.0.0.1", "443"))
{
    std::cout << "connected!\n";
}
else
{
    std::cout << "connection failed!\n";
}
char buffer[1024];


client.send_data("hello", 5);
while (true)
{
    if(client.receive(buffer, sizeof(buffer)))
    {
        std::cout << "received: " << buffer << "\n";
        client.close();
        return 0;
    }
}

*/
