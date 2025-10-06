#if defined(_WIN32) || defined(_WIN64)
#define IS_WINDOWS 1
#else
#define IS_WINDOWS 0
#endif

#include "websock.h"

#if IS_WINDOWS
#include <BaseTsd.h>

#endif

#include <signal.h>

#include <iostream>

bool ctier::WebSock::receive(char* buffer, size_t size)
{
    if (!_socket)
        return false;

    auto bytes = ::recv(_socket, buffer, size, 0);
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

    auto bytesSent = ::send(_socket, buffer, size, 0);
    if (bytesSent != size)
        return false;  // partial send or error

    return true;
}
bool ctier::WebSock::connect(const char* address, const char* port)
{
#if IS_WINDOWS
    const sockaddr* addr = nullptr;
    addrinfo        hints{};
    addrinfo*       result = nullptr;

    auto iResult = getaddrinfo(address, port, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return false;
    }

    iResult = ::connect(_socket, result->ai_addr, (int) result->ai_addrlen);

    if (iResult != 0)
    {
        printf("connect failed: %d\n", iResult);
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    return true;
#else
    u_short     port_num = static_cast<u_short>(std::atoi(port));
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port_num);

    if (inet_aton(address, &server_addr.sin_addr) == 0)
    {
        std::cerr << "Invalid address: " << address << "\n";
        return false;
    }
    std::cout << "adress validated!\n";

    if (::connect(_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
    {
        perror("connection failed!");
        return false;
    }
    return true;
#endif
}

bool ctier::WebSock::disconnect()
{
#if IS_WINDOWS
    closesocket(_socket);
#endif
    return true;
}
#if IS_WINDOWS
bool ctier::WebSock::suspend()
{
    iResult = shutdown(_socket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(_socket);
        WSACleanup();
        return false;
    }
    return true;
}
#endif
// used to set the servers hosting address up, usually 0.0.0.0 but you can be more specific
bool ctier::WebSock::bind(const char* address, const char* port, bool server = true)
{
#if IS_WINDOWS
    addrinfo hints{};
    hints.ai_family   = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    if (server)
        hints.ai_flags = AI_PASSIVE;  // For server

    addrinfo* result = nullptr;
    iResult          = getaddrinfo(address, port, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d\n", iResult);
        return false;
    }

    iResult = ::bind(_socket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        return false;
    }

    freeaddrinfo(result);
#else
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port   = htons(static_cast<u_short>(std::atoi(port)));

    if (inet_pton(AF_INET, address, &hint.sin_addr) <= 0)
        return false;

    if (::bind(_socket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint)) < 0)
    {
        perror("bind failed");
        return false;
    }
#endif

    std::cout << "Bound to " << address << ":" << port << "\n";
    return true;
}

int ctier::WebSock::accept(sockaddr_in* clientAddr)
{
#if IS_WINDOWS
    if (_socket == INVALID_SOCKET)
        return INVALID_SOCKET;

    int    len = clientAddr ? sizeof(sockaddr_in) : 0;
    SOCKET clientSock =
        ::accept(_socket, clientAddr ? reinterpret_cast<sockaddr*>(clientAddr) : nullptr, clientAddr ? &len : nullptr);

    if (clientSock == INVALID_SOCKET)
        printf("Accept failed: %d\n", WSAGetLastError());

    return static_cast<int>(clientSock);

#else
    if (_socket < 0)
        return -1;

    socklen_t len = clientAddr ? sizeof(sockaddr_in) : 0;
    int       clientSock =
        ::accept(_socket, clientAddr ? reinterpret_cast<sockaddr*>(clientAddr) : nullptr, clientAddr ? &len : nullptr);

    if (clientSock < 0)
        perror("Accept failed");

    return clientSock;
#endif
}

bool ctier::WebSock::listen(int backlog)
{
    if (::listen(_socket, backlog) < 0)
    {
#if IS_WINDOWS
        printf("Listen failed with error: %ld\n", WSAGetLastError());
#else
        perror("Listen failed");
#endif
        return false;
    }
    return true;
}

void ctier::WebSock::close_socket(bool reuse)
{
#if IS_WINDOWS
    closesocket(_socket);
    WSACleanup();
#else
    close(_socket);
    signal(SIGCHLD, SIG_IGN);
#endif
}
