#include "server.h"

#include <netinet/in.h>

#include <iostream>

int main()
{
    auto server = ctier::Server("127.0.0.1", "1234", AF_INET, SOCK_STREAM, 0);  // adress any = 0
    char buffer[1024];

    while (true)
    {
        sockaddr_in client_addr{};
        int         clientFD = server.getSocket()->accept(&client_addr);
        if (clientFD < 0)
            throw std::runtime_error("Accept failed");

        // Wrap client socket
        auto clientSock = std::make_unique<ctier::WebSock>(clientFD);

        if (clientSock->receive(buffer, sizeof(buffer)))
        {
            std::cout << "Received data\n";
            std::cout << buffer << std::endl;
        }
    }

    return 0;
}
