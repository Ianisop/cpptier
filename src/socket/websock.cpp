#include "websock.h"

#include <iostream>
bool ctier::WebSock::receive(char* buffer, size_t size)
{
    if (!_socket)
        return false;

    ssize_t bytes = ::recv(_socket, buffer, size, 0);
    if (bytes <= 0)
        return false;  // connection closed or error
    std::cout << "waiting for data!\n";
    buffer[bytes] = '\0';  // null-terminate if you expect string
    return true;
}

bool ctier::WebSock::send(const char* buffer, size_t size)
{
    if (!_socket)
        return false;

    ssize_t bytesSent = ::send(_socket, buffer, size, 0);
    if (bytesSent != (ssize_t) size)
        return false;  // partial send or error

    return true;
}
bool ctier::WebSock::connect(const char* address, const char* port)
{
    u_short port_num = static_cast<u_short>(std::atoi(port));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port_num);

    if (inet_aton(address, &server_addr.sin_addr) == 0)
    {
        std::cerr << "Invalid address: " << address << "\n";
        return false;
    }

    if (::connect(_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        return false;
    }

    return true;
}
// used to set the servers hosting address up, usually 0.0.0.0 but you can be more specific
bool ctier::WebSock::bind(const char* address, const char* port)
{
    u_short port_num = static_cast<u_short>(std::atoi(port));

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port   = htons(port_num);

    if (inet_aton(address, &hint.sin_addr) == 0)
        return false;

    if (::bind(_socket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint)) < 0)
    {
        perror("bind failed");
        return false;
    }

    return true;
}
bool ctier::WebSock::listen(int backlog)
{
    return ::listen(_socket, backlog) == 0;
}

int ctier::WebSock::accept(sockaddr_in* clientAddr)
{
    socklen_t len = clientAddr ? sizeof(*clientAddr) : 0;
    return ::accept(_socket, reinterpret_cast<sockaddr*>(clientAddr), clientAddr ? &len : nullptr);
}
