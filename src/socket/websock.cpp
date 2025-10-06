#include "websock.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace ctier
{
    static int g_wsa_initialized = 0;
    WebSock::WebSock(int domain, int type, int protocol) : _socket(INVALID_SOCKET_VALUE)
    {
#if IS_WINDOWS
        if (g_wsa_initialized == 0)
        {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
                throw std::runtime_error("WSAStartup failed");
#endif 
            g_wsa_initialized = 1;
        }
        create(domain, type, protocol);
    }
    
    WebSock::WebSock(int fd) : _socket(static_cast<socket_t>(fd)) {}
    WebSock::~WebSock()
    {
        close_socket();
    }

    bool WebSock::create(int domain, int type, int protocol)
    {
#if IS_WINDOWS
        _socket = ::socket(domain, type, protocol);
        return _socket != INVALID_SOCKET;
#else
        _socket = ::socket(domain, type, protocol);
        return _socket >= 0;
#endif
    }

    bool WebSock::bind(const char* address, const char* port)
    {
        struct addrinfo hints{}, *res = nullptr;
        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags    = AI_PASSIVE;

        if (getaddrinfo(address, port, &hints, &res) != 0)
            return false;

        bool ok = (::bind(_socket, res->ai_addr, (socklen_t) res->ai_addrlen) == 0);
        freeaddrinfo(res);
        return ok;
    }

    bool WebSock::listen(int backlog)
    {
        return (::listen(_socket, backlog) == 0);
    }

    int WebSock::accept(sockaddr_in* clientAddr)
    {
        socklen_t len    = clientAddr ? sizeof(*clientAddr) : 0;
        auto      client = ::accept(_socket,
                               clientAddr ? reinterpret_cast<sockaddr*>(clientAddr) : nullptr,
                               clientAddr ? &len : nullptr);
#if IS_WINDOWS
        return (client == INVALID_SOCKET) ? -1 : static_cast<int>(client);
#else
        return client;
#endif
    }

    bool WebSock::connect(const char* address, const char* port)
    {
        struct addrinfo hints{}, *res = nullptr;
        hints.ai_family   = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(address, port, &hints, &res) != 0)
            return false;

        bool ok = (::connect(_socket, res->ai_addr, (socklen_t) res->ai_addrlen) == 0);
        freeaddrinfo(res);
        return ok;
    }

    bool WebSock::send(const char* buffer, size_t size)
    {
        if (!valid())
            return false;
        int sent = ::send(_socket, buffer, (int) size, 0);
        return (sent == (int) size);
    }

    int WebSock::receive(char* buffer, size_t size)
    {
        if (!valid())
            return -1;
        return ::recv(_socket, buffer, (int) size, 0);
    }

    void WebSock::close_socket()
    {
        if (!valid())
            return;
#if IS_WINDOWS
        ::closesocket(_socket);
#else
        ::close(_socket);
#endif
        _socket = INVALID_SOCKET_VALUE;
    }

    void WebSock::cleanup()
    {
#if IS_WINDOWS
        WSACleanup();
#endif
    }
    bool WebSock::valid() const
    {
#if IS_WINDOWS
        return _socket != INVALID_SOCKET;
#else
        return _socket >= 0;
#endif
    }

}  // namespace ctier
