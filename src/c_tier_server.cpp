#include <iostream>
#include <memory>
#include <stdexcept>

#include "server/server.h"
#include "socket/websock.h"
#include "sslwebsock.h"

int main()
{
    try
    {
        // Allocate server safely on the stack
        ctier::Server server("127.0.0.1", "443", AF_INET, SOCK_STREAM, 0);

        // Make sure server socket is ready
        if (!server.getSocket())
        {
            throw std::runtime_error("Server socket initialization failed");
        }

        char buffer[1024];

        SSLWebSock ssl_server(true);
        if (!ssl_server.init("server.crt", "server.key"))
            throw std::runtime_error("SSL init failed");

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

            SSLWebSock session(true);
            session.set_context(ssl_server.get_context());
            if (!session.attach(clientFD))
            {
                std::cerr << "SSL handshake failed\n";
                ::close(clientFD);
                continue;
            }
            if(session.receive(buffer, sizeof(buffer)))
            {
                std::cout << "Received: " << buffer << "\n";
            }

            session.send("OK",2);
            session.close();

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
