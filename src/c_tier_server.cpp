#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "SQLiteCpp/Database.h"
#include "database.h"
#include "server/server.h"
#include "socket/websock.h"
#include "sslwebsock.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

int main()
{
    try
    {
        // Allocate server safely on the stack
        ctier::Server server("127.0.0.1", "443", AF_INET, SOCK_STREAM, 0, true);

        // Make sure server socket is ready
        if (!server.get_socket())
        {
            throw std::runtime_error("Server socket initialization failed");
        }

        char buffer[1024];

        ctier::UserDatabase* user_db = new ctier::UserDatabase("");
        user_db->initialize();

        while (true)
        {
            sockaddr_in client_addr{};
            int         clientFD = server.get_socket()->accept(&client_addr);
            std::cout << "Accepted: " << clientFD << "\n";
            if (clientFD < 0)
            {
#if IS_WINDOWS
                int err = WSAGetLastError();
                std::cerr << "Accept failed, error: " << err << std::endl;
#else
                perror("Accept failed");
#endif
                continue;  // skip this iteration
            }

            // Create a new SSL session for this client
            auto session = std::make_unique<ctier::SSLWebSock>(true);
            if (session)
            {
                std::cout << "session created for " << clientFD << "\n";
            }
            try
            {
                session->set_context(server.get_socket()->get_ssl()->get_context());
            }
            catch (std::exception ex)
            {
                std::cout << ex.what() << "\n";
            }
            if (!session->attach(clientFD))
            {
                std::cerr << "SSL handshake failed\n";
                ::close(clientFD);
                continue;
            }
            if (session->receive(buffer, sizeof(buffer)))
            {
                std::cout << "Received: " << buffer << "\n";
            }

            session->send("OK", 2);
            session->close();
            ctier::WebSock::cleanup();

            // Socket will be closed automatically when clientSock goes out of scope
        }

        server.stop();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server initialization failed: " << e.what() << std::endl;
        return 1;
    }
    ctier::WebSock::cleanup();
    return 0;
}
