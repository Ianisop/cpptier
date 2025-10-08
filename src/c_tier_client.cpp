#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <iostream>

#include "client/client.h"

using namespace ftxui;

int main()
{
    /*
    auto screen = ScreenInteractive::Fullscreen();

    int y = 0;

    auto ui = Renderer([&] {
        return vbox({
            yflex_shrink( text("FTXUI full terminal example") | center),
            separator(),
            text("Resize your terminal to see it adapt.") | center,
        }) | border;
    });

    std::thread([&] {
        while(true)
        {
            y++;
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }).detach();

    screen.Loop(ui);
    */
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


    return 0;
}
