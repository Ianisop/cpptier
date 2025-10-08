#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <iostream>

#include "client/client.h"
#include "sslwebsock.h"
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
    auto client = ctier::Client(AF_INET, SOCK_STREAM, 0);

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

    SSLWebSock SSL_client(false);
    if(!SSL_client.init())
        throw std::runtime_error("SSL INIT FAILED!\n");

    if(!SSL_client.attach((int)client.get_web_socket()->get_socket()))
        throw std::runtime_error("SSL Handshake failed\n");

    SSL_client.send("hello", 5);
    while (true)
    {
        if(SSL_client.receive(buffer, sizeof(buffer)))
        {
            std::cout << "received: " << buffer << "\n";
        }
    }
    SSL_client.close();
    std::cout << "connection closed!\n";
    return 0;
}
