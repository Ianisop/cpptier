#pragma once
#if defined(_WIN32) || defined(_WIN64)
#define IS_WINDOWS 1
#else
#define IS_WINDOWS 0
#endif

#if IS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cstdio>

namespace ctier
{

    class WebSock
    {
      private:
#if IS_WINDOWS
        SOCKET _socket = INVALID_SOCKET;
        int    iResult = 0;
#else
        int _socket = -1;
#endif

      public:
        WebSock(int domain, int type, int protocol)
        {
#if IS_WINDOWS

            _socket = socket(domain, type, protocol);
#else
            _socket = socket(domain, type, protocol);
#endif
        }

        // Fix for E0109: _socket is a variable, not a function.
        // The correct way to assign the socket handle is direct assignment.
        explicit WebSock(int clientFD)
        {
#if IS_WINDOWS
            iResult = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
            if (iResult != 0)
            {
                printf("WSAStartup failed: %d\n", iResult);
            }
            _socket = clientFD;
#else
            _socket = clientFD;
#endif
        }

        ~WebSock()
        {
#if IS_WINDOWS
            if (_socket != INVALID_SOCKET)
                closesocket(_socket);
            WSACleanup();
#else
            if (_socket >= 0)
                close(_socket);
#endif
        }

        bool connect(const char* address, const char* port);
        bool bind(const char* address, const char* port, bool server);
        bool send(const char* data, size_t size);
        bool receive(char* buffer, size_t size);
        bool listen(int backlog = 5);
        void close_socket(bool reuse = false);
        int  accept(sockaddr_in* clientAddr = nullptr);
        bool disconnect();
        bool suspend();
    };

}  // namespace ctier
