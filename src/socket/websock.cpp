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
    if (bytesSent !=  size)
        return false;  // partial send or error

    return true;
}
bool ctier::WebSock::connect(const char* address, const char* port)
{
    #if IS_WINDOWS
    const sockaddr* addr = nullptr;
    addrinfo        hints{};
    addrinfo*       result  = nullptr;

    auto iResult              = getaddrinfo(address, port, &hints, &result);
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

    if (::connect(_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        return false;
    }
    #endif
    return true;
}

bool ctier::WebSock::disconnect()
{
    closesocket(_socket);
    WSACleanup();
    return true;
}

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
// used to set the servers hosting address up, usually 0.0.0.0 but you can be more specific
bool ctier::WebSock::bind(const char* address, const char* port)
{
    #if IS_WINDOWS
    const sockaddr* addr = nullptr; 
    addrinfo        hints{};
    addrinfo*       result  = nullptr;
    getaddrinfo(address, port, &hints, &result);

    iResult = ::bind(_socket, result->ai_addr, (int) result->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(_socket);
        WSACleanup();
        return false;
    }
    #else
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
    #endif

    return true;
}

int ctier::WebSock::accept(sockaddr_in* clientAddr)
{
#if IS_WINDOWS
    if (_socket == INVALID_SOCKET)
        return INVALID_SOCKET;

    int    len = clientAddr ? sizeof(*clientAddr) : 0;
    SOCKET clientSock =
        ::accept(_socket, clientAddr ? reinterpret_cast<sockaddr*>(clientAddr) : nullptr, clientAddr ? &len : nullptr);
    if (clientSock == INVALID_SOCKET)
    {
        printf("Accept failed: %d\n", WSAGetLastError());
    }
    return static_cast<int>(clientSock);  // convert SOCKET to int
#else
    if (_socket < 0)
        return -1;

    socklen_t len = clientAddr ? sizeof(*clientAddr) : 0;
    int       clientSock =
        ::accept(_socket, clientAddr ? reinterpret_cast<sockaddr*>(clientAddr) : nullptr, clientAddr ? &len : nullptr);
    if (clientSock < 0)
    {
        perror("Accept failed");
    }
    return clientSock;
#endif
}


bool ctier::WebSock::listen(int backlog)
{
    if (::listen(_socket, backlog) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        closesocket(_socket);
        WSACleanup();
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

