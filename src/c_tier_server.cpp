#include <iostream>
#include <memory>
#include <stdexcept>

#include "server/server.h"


int main()
{
    try
    {
        // Allocate server safely on the stack
        ctier::Server server("127.0.0.1", "1234", AF_INET, SOCK_STREAM, 0);

        // Make sure server socket is ready
        if (!server.getSocket())
        {
            throw std::runtime_error("Server socket initialization failed");
        }

        char buffer[1024];

        while (true)
        {
            sockaddr_in client_addr{};
            int         clientFD = server.getSocket()->accept(&client_addr);

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

            // Wrap client socket in a unique_ptr
            auto clientSock = std::make_unique<ctier::WebSock>(clientFD);

            // Receive data from client
            if (clientSock->receive(buffer, sizeof(buffer)))
            {
                std::cout << "Received data: " << buffer << std::endl;

                // Respond to client
                clientSock->send("received", 8);  // send length includes null?
            }

            // Socket will be closed automatically when clientSock goes out of scope
        }

        server.stop();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server initialization failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
